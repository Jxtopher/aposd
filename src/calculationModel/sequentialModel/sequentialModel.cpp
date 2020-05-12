#include "calculationModel/sequentialModel/sequentialModel.hpp"

template<class SOL>
SequentialModel<SOL>::SequentialModel(std::unique_ptr<Launcher> launcher,
				std::unique_ptr<ParameterSelection> parameterSelection,
				std::unique_ptr<RewardComputation<SOL>> rewardComputation)
	: _launcher(std::move(launcher)), _parameterSelection(std::move(parameterSelection)),
		_rewardComputation(std::move(rewardComputation)) {}

template<class SOL>
SequentialModel<SOL>::~SequentialModel() {}

template<class SOL>
void SequentialModel<SOL>::operator()() {
	// Initialisation de la 1er solution
	Solution<unsigned int> s(_launcher->initSolution());
	std::cout << ">" << s << "<" << std::endl;

	_parameterSelection->reset();

	while(s.getFitness() < 50) {
		// Get list operators to apply
		unsigned int parameter = _parameterSelection->getParameter();

		Solution<unsigned int> s_new(_launcher->solve(s.str(), parameter));

		std::pair<double, unsigned int> rewardOp =
			_rewardComputation->operator()(s, s_new, parameter);

		// update
		_parameterSelection->update(rewardOp);

		s = s_new;

		std::cout << "Out>" << parameter << " | " << s << "<" << std::endl;
	}
}