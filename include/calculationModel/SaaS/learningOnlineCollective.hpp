#ifndef SAAS_LEARNINGONLINECOLLECTIVE_H
#define SAAS_LEARNINGONLINECOLLECTIVE_H

#include <memory>
#include <numeric>
#include <utility>
#include <iostream>

#include <boost/circular_buffer.hpp>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../../selection/solutionSelection.hpp"
#include "../calculationModel.hpp"
#include "learningOnline.hpp"

template<class SOL> class LearningOnlineCollective : public LearningOnline<SOL> {
  public:
	LearningOnlineCollective(std::unique_ptr<ParameterSelection> parameterSelection,
							 std::unique_ptr<RewardComputation<SOL>> rewardComputation,
							 std::unique_ptr<SolutionSelection<SOL>> selection,
							 unsigned int number_of_nodes = 5);

	virtual ~LearningOnlineCollective();

	std::pair<SOL, unsigned int> initial_solution(const SOL& s);

	std::pair<SOL, unsigned int> run(const SOL& s_t0, const SOL s_t1, unsigned int parameter);

  protected:
	boost::circular_buffer<double> history_fitness;
	std::unique_ptr<ParameterSelection> _parameterSelection;
	std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
	std::unique_ptr<SolutionSelection<SOL>> _selection;
	SOL solution_star;
	bool solution_star_init;
	unsigned int _number_of_nodes;
	std::vector<unsigned int> parameters;
};

#endif