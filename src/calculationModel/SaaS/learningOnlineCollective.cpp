#include "calculationModel/SaaS/learningOnlineCollective.hpp"

template<class SOL> 
LearningOnlineCollective<SOL>::LearningOnlineCollective(std::unique_ptr<ParameterSelection> parameterSelection,
							std::unique_ptr<RewardComputation<SOL>> rewardComputation,
							std::unique_ptr<SolutionSelection<SOL>> selection,
							unsigned int number_of_nodes)
	: _parameterSelection(std::move(parameterSelection)),
		_rewardComputation(std::move(rewardComputation)), _selection(std::move(selection)),
		_number_of_nodes(number_of_nodes) {
	solution_star_init = false;
	history_fitness.set_capacity(number_of_nodes);
}

template<class SOL>
LearningOnlineCollective<SOL>::~LearningOnlineCollective() {}

template<class SOL>
std::pair<SOL, unsigned int> LearningOnlineCollective<SOL>::initial_solution(const SOL& s) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " init Collective ";
	if(solution_star_init == false) {
		solution_star = s;
		solution_star_init = true;
	} else {
		if(_selection->operator()(s, solution_star)) { solution_star = s; }
	}
	std::cout << parameters.size() << std::endl;
	if(parameters.empty()) {
		std::cout << "Empty" << std::endl;
		parameters = _parameterSelection->getParameter(_number_of_nodes);
	}
	unsigned int new_parameter = parameters.back();
	parameters.pop_back();

	return std::pair<SOL, unsigned int>(solution_star, new_parameter);
}

template<class SOL>
std::pair<SOL, unsigned int> LearningOnlineCollective<SOL>::run(const SOL& s_t0, const SOL s_t1, unsigned int parameter) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " run Collective ";

	std::pair<double, unsigned int> rewardOp =
		_rewardComputation->operator()(s_t0, s_t1, parameter);
	std::pair<double, unsigned int> delta_f =
		_rewardComputation->operator()(s_t1, solution_star, parameter);
	if(delta_f.first < 0) { // Negatif
		double average = 0;
		if(history_fitness.size() != 0)
			average = std::accumulate(history_fitness.begin(), history_fitness.end(), 0.0) /
						history_fitness.size();
		if(average < s_t1.getFitness()) {
			_parameterSelection->update(rewardOp);
			history_fitness.push_front(s_t1.getFitness());
		}
	} else { // Positif
		// update
		_parameterSelection->update(rewardOp);
		history_fitness.push_front(s_t1.getFitness());
	}

	if(_selection->operator()(s_t1, solution_star)) { solution_star = s_t1; }

	if(parameters.empty()) parameters = _parameterSelection->getParameter(_number_of_nodes);

	unsigned int new_parameter = parameters.back();
	parameters.pop_back();
	return std::pair<SOL, unsigned int>(solution_star, new_parameter);
}