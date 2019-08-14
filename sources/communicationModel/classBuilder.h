#ifndef JXTOPHER_CLASSBUILDER_H
#define JXTOPHER_CLASSBUILDER_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../launcher/launcher.h"
#include "../launcher/launcherExec.h"

#include "../parameterSelection/parameterSelection.h"
#include "../parameterSelection/psConstant.h"
#include "../parameterSelection/psRandom.h"
#include "../parameterSelection/psAdaptivePursuit.h"
#include "../parameterSelection/psUCBW.h"
#include "../parameterSelection/psSelectBestMutate.h"
#include "../parameterSelection/psEpsilonGreedyW.h"

#include "../rewardComputation/rewardComputation.h"

#include "../calculationModel/islandModel/topologies/topologies.h"
#include "../calculationModel/islandModel/topologies/circle.h"
#include "../calculationModel/islandModel/topologies/complete.h"
#include "../calculationModel/islandModel/topologies/randomEdge.h"
#include "../calculationModel/islandModel/topologies/star.h"

using namespace std;

class ClassBuilder {
public:
    ClassBuilder(std::shared_ptr<std::mt19937> mt_rand) : _mt_rand(mt_rand) {

    }

    std::unique_ptr<Topologies> topologies(const Json::Value &configuration) {
        std::unique_ptr<Topologies> _topologies;
        if (configuration["className"].asString() == Topologies::CIRCLE)
            _topologies = make_unique<Circle>(4);
        else if (configuration["className"].asString() == Topologies::COMPLETE)
            _topologies = make_unique<Complete>(4);
        else if (configuration["className"].asString() == Topologies::RANDOMEDGE)
            _topologies = make_unique<RandomEdge>(_mt_rand, 4);
        else if (configuration["className"].asString() == Topologies::STAR)
            _topologies = make_unique<Star>(4);
        else 
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The topologie " + + " is not defined");
        return std::move(_topologies);
    }


    std::unique_ptr<ParameterSelection> parameterSelection(const Json::Value &configuration) {
        std::unique_ptr<ParameterSelection> _parameterSelection;
        if (configuration["className"].asString() == ParameterSelection::STR_ADAPTIVEPURSUIT) {
            _parameterSelection = make_unique<PsAdaptivePursuit>(_mt_rand, configuration["nbParameter"].asUInt());
        } else if (configuration["className"].asString() == ParameterSelection::STR_CONSTANT) {
            _parameterSelection = make_unique<PsConstant>(configuration["nbParameter"].asUInt(), 0);
        } else if (configuration["className"].asString() == ParameterSelection::STR_EPSILONGREEDY) {
            _parameterSelection = make_unique<PsEspsilonGreedy>(_mt_rand, configuration["nbParameter"].asUInt());
        } else if (configuration["className"].asString() == ParameterSelection::STR_RANDOM) {
            _parameterSelection = make_unique<PsRandom>(_mt_rand, configuration["nbParameter"].asUInt());
        } else if (configuration["className"].asString() == ParameterSelection::STR_SELECTBESTMUTATE) {
            _parameterSelection = make_unique<PsRandom>(_mt_rand, configuration["nbParameter"].asUInt());
        } else if (configuration["className"].asString() == ParameterSelection::STR_UCBW) {
            _parameterSelection = make_unique<PsUCBW>(_mt_rand, configuration["nbParameter"].asUInt()); 
        } else {
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The calculation model is not defined");
        }
        return std::move(_parameterSelection);
    }

    std::unique_ptr<Launcher> launcher(const Json::Value &configuration) {
        std::unique_ptr<Launcher> _launcher;
        if (Launcher::LAUNCHEREXEC == configuration["className"].asString())
            _launcher = make_unique<LauncherExec>(configuration["pathExecutable"].asString(), configuration["string_args"].asString());
        else 
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The Launcher is not defined");

        return std::move(_launcher);
    }

    template <typename SOL>
    std::unique_ptr<RewardComputation<SOL>> rewardComputation(const Json::Value &configuration) {
        return make_unique<RewardComputation<SOL>>();
    }

private:
    std::shared_ptr<std::mt19937> _mt_rand;

};

#endif