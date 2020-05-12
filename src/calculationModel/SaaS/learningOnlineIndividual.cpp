#include "calculationModel/SaaS/learningOnlineIndividual.hpp"

template<class SOL>
LearningOnlineIndividual<SOL>::LearningOnlineIndividual(std::unique_ptr<ParameterSelection> parameterSelection,
							std::unique_ptr<RewardComputation<SOL>> rewardComputation,
							std::unique_ptr<SolutionSelection<SOL>> selection)
	: _parameterSelection(std::move(parameterSelection)),
		_rewardComputation(std::move(rewardComputation)), _selection(std::move(selection)) {}

template<class SOL>
LearningOnlineIndividual<SOL>::~LearningOnlineIndividual() {}

template<class SOL>
std::pair<SOL, unsigned int> LearningOnlineIndividual<SOL>::initial_solution(const SOL& s) {
	solution_t0 = s;
	return std::pair<SOL, unsigned int>(solution_t0, 0);
}

template<class SOL>
std::pair<SOL, unsigned int> LearningOnlineIndividual<SOL>::run(const SOL& s_t0, const SOL s_t1, unsigned int parameter) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " run Individual ";
	solution_t0 = s_t0;
	solution_t1 = s_t1;

	std::pair<double, unsigned int> rewardOp =
		_rewardComputation->operator()(solution_t0, solution_t1, parameter);

	// update
	_parameterSelection->update(rewardOp);

	unsigned int new_parameter = _parameterSelection->getParameter();

	solution_t0 = solution_t1;

	return std::pair<SOL, unsigned int>(solution_t1, new_parameter);
}

template<class SOL>
void LearningOnlineIndividual<SOL>::operator()() {}