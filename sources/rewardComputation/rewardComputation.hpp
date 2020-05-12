#ifndef REWARDCOMPUTATION_H
#define REWARDCOMPUTATION_H

#include <cassert>
#include <utility>

template<typename SOL>
class RewardComputation {
  public:
	RewardComputation() {
	}

	virtual ~RewardComputation() {
	}

	std::pair<double, unsigned int> operator()(const SOL& previousSolution, const SOL& solution,
											   const unsigned int& parameter) const {
		std::pair<double, unsigned int> rewardOp(
			solution.getFitness() - previousSolution.getFitness(), parameter);
		return rewardOp;
	}

	std::vector<std::pair<double, unsigned int>>
		operator()(const std::vector<SOL>& previousSolution, const std::vector<SOL>& solution,
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

  protected:
};

#endif