#ifndef SAAS_LEARNINGONLINEINDIVIDUAL_H
#define SAAS_LEARNINGONLINEINDIVIDUAL_H

#include <memory>
#include <utility>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../../selection/solutionSelection.hpp"
#include "learningOnline.hpp"

template<class SOL> class LearningOnlineIndividual : public LearningOnline<SOL> {
  public:
	LearningOnlineIndividual(std::unique_ptr<ParameterSelection> parameterSelection,
							 std::unique_ptr<RewardComputation<SOL>> rewardComputation,
							 std::unique_ptr<SolutionSelection<SOL>> selection);

	virtual ~LearningOnlineIndividual();

	std::pair<SOL, unsigned int> initial_solution(const SOL& s);

	std::pair<SOL, unsigned int> run(const SOL& s_t0, const SOL s_t1, unsigned int parameter);

	void operator()();

  protected:
	std::unique_ptr<ParameterSelection> _parameterSelection;
	std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
	std::unique_ptr<SolutionSelection<SOL>> _selection;
	SOL solution_t0;
	SOL solution_t1;
};

#endif