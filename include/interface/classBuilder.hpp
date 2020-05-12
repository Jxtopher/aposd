#ifndef JXTOPHER_CLASSBUILDER_H
#define JXTOPHER_CLASSBUILDER_H

#include <jsoncpp/json/json.h>
#include <memory>

#include "../launcher/launcher.hpp"
#include "../launcher/launcherExec.hpp"

#include "../parameterSelection/parameterSelection.hpp"
#include "../parameterSelection/psAdaptivePursuit.hpp"
#include "../parameterSelection/psConstant.hpp"
#include "../parameterSelection/psEpsilonGreedyW.hpp"
#include "../parameterSelection/psRandom.hpp"
#include "../parameterSelection/psSelectBestMutate.hpp"
#include "../parameterSelection/psUCBW.hpp"

#include "../rewardComputation/rewardComputation.hpp"

#include "../calculationModel/islandModel/topologies/circle.hpp"
#include "../calculationModel/islandModel/topologies/complete.hpp"
#include "../calculationModel/islandModel/topologies/randomEdge.hpp"
#include "../calculationModel/islandModel/topologies/star.hpp"
#include "../calculationModel/islandModel/topologies/topologies.hpp"

#include "../selection/selection_maximization.hpp"
#include "../selection/selection_minimization.hpp"
#include "../selection/solutionSelection.hpp"

class ClassBuilder {
  public:
	ClassBuilder(std::shared_ptr<std::mt19937> mt_rand);

	std::unique_ptr<Topologies> topologies(const Json::Value& configuration);

	std::unique_ptr<ParameterSelection> parameterSelection(const Json::Value& configuration);

	std::unique_ptr<Launcher> launcher(const Json::Value& configuration);
	template<typename SOL>
	std::unique_ptr<RewardComputation<SOL>> rewardComputation(const Json::Value& configuration);
	
	template<typename SOL>
	std::unique_ptr<SolutionSelection<SOL>> solutionSelection(const Json::Value& configuration);
  private:
	std::shared_ptr<std::mt19937> _mt_rand;
};

#endif