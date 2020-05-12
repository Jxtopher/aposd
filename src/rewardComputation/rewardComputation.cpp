
#include "rewardComputation/rewardComputation.hpp"

template<typename SOL>
RewardComputation<SOL>::RewardComputation() {}

template<typename SOL>
RewardComputation<SOL>::~RewardComputation() {}

template<typename SOL>
std::pair<double, unsigned int> RewardComputation<SOL>::operator()(const SOL& previousSolution, const SOL& solution,
											const unsigned int& parameter) const {
	std::pair<double, unsigned int> rewardOp(
		solution.getFitness() - previousSolution.getFitness(), parameter);
	return rewardOp;
}

template<typename SOL>
std::vector<std::pair<double, unsigned int>>
	RewardComputation<SOL>::operator()(const std::vector<SOL>& previousSolution, const std::vector<SOL>& solution,
				const std::vector<unsigned int>& parameter) const {
	std::vector<std::pair<double, unsigned int>> rewardOp;
	assert(previousSolution.size() == solution.size());
	assert(previousSolution.size() == parameter.size());
	for(unsigned int i = 0; i < previousSolution.size(); i++) {
		rewardOp.push_back(std::pair<double, unsigned int>(
			solution[i].getFitness() - previousSolution[i].getFitness(), parameter[i]));
	}
	return rewardOp;
}