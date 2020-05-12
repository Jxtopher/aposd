#ifndef REWARDCOMPUTATION_H
#define REWARDCOMPUTATION_H

#include <utility>	// std::pair
#include <vector>	// std::vector

template<typename SOL> 
class RewardComputation {
  public:
	RewardComputation();

	virtual ~RewardComputation();

	std::pair<double, unsigned int> operator()(const SOL& previousSolution, const SOL& solution,
											   const unsigned int& parameter) const;

	std::vector<std::pair<double, unsigned int>>
		operator()(const std::vector<SOL>& previousSolution, const std::vector<SOL>& solution,
				   const std::vector<unsigned int>& parameter) const;

};

#endif