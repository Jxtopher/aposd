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
    ClassBuilder(std::shared_ptr<std::mt19937> mt_rand) : _mt_rand(mt_rand) {
    }

    std::unique_ptr<Topologies> topologies(const Json::Value& configuration) {
        std::unique_ptr<Topologies> _topologies;
        if(configuration["className"].asString() == Topologies::CIRCLE)
            _topologies = std::make_unique<Circle>(4);
        else if(configuration["className"].asString() == Topologies::COMPLETE)
            _topologies = std::make_unique<Complete>(4);
        else if(configuration["className"].asString() == Topologies::RANDOMEDGE)
            _topologies = std::make_unique<RandomEdge>(_mt_rand, 4);
        else if(configuration["className"].asString() == Topologies::STAR)
            _topologies = std::make_unique<Star>(4);
        else
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] The topologie " + +" is not defined");
        return std::move(_topologies);
    }

    std::unique_ptr<ParameterSelection> parameterSelection(const Json::Value& configuration) {
        BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " ClassBuilder "
                                 << configuration["className"].asString();

        const char* heterogeneity_policy;
        if(!configuration["heterogeneity_policy"].empty()) {
            if(configuration["heterogeneity_policy"].asString() ==
               HeterogeneityPolicy::HETEROGENOUS) {
                heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS;
            } else if(configuration["heterogeneity_policy"].asString() ==
                      HeterogeneityPolicy::HOMOGENEOUS) {
                heterogeneity_policy = HeterogeneityPolicy::HOMOGENEOUS;
            }
        }

        const char* aggregation_function;
        if(configuration["aggregation_function"].empty()) {
            if(configuration["aggregation_function"].asString() == AggregationFunction::MAX) {
                aggregation_function = AggregationFunction::MAX;
            } else if(configuration["aggregation_function"].asString() ==
                      AggregationFunction::MEAN) {
                aggregation_function = AggregationFunction::MEAN;
            }
        }

        std::unique_ptr<ParameterSelection> _parameterSelection;
        if(configuration["className"].asString() == ParameterSelection::ADAPTIVEPURSUIT) {
            if(!configuration["alpha"].empty() && !configuration["beta"].empty() &&
               !configuration["p_min"].empty() && !configuration["p_max"].empty() &&
               !configuration["aggregation_function"].empty() &&
               !configuration["heterogeneity_policy"].empty()) {
                _parameterSelection = std::make_unique<PsAdaptivePursuit>(
                    _mt_rand, configuration["number_of_parameters"].asUInt(),
                    configuration["alpha"].asDouble(), configuration["beta"].asDouble(),
                    configuration["p_min"].asDouble(), configuration["p_max"].asDouble(),
                    aggregation_function, heterogeneity_policy);
            } else
                _parameterSelection = std::make_unique<PsAdaptivePursuit>(
                    _mt_rand, configuration["number_of_parameters"].asUInt());
        } else if(configuration["className"].asString() == ParameterSelection::CONSTANT) {
            if(!configuration["parameterUsed"].empty())
                _parameterSelection =
                    std::make_unique<PsConstant>(configuration["number_of_parameters"].asUInt(),
                                                 configuration["parameterUsed"].asUInt());
            else
                _parameterSelection =
                    std::make_unique<PsConstant>(configuration["number_of_parameters"].asUInt(), 0);
        } else if(configuration["className"].asString() == ParameterSelection::EPSILONGREEDY) {
            if(!configuration["espilon"].empty() && !configuration["windowSize"].empty() &&
               !configuration["aggregation_function"].empty() &&
               !configuration["heterogeneity_policy"].empty())
                _parameterSelection = std::make_unique<PsEspsilonGreedy>(
                    _mt_rand, configuration["number_of_parameters"].asUInt(),
                    configuration["espilon"].asDouble(), configuration["windowSize"].asUInt(),
                    aggregation_function, heterogeneity_policy);
            else
                _parameterSelection = std::make_unique<PsEspsilonGreedy>(
                    _mt_rand, configuration["number_of_parameters"].asUInt());
        } else if(configuration["className"].asString() == ParameterSelection::RANDOM) {
            if(!configuration["aggregation_function"].empty() &&
               !configuration["heterogeneity_policy"].empty())
                _parameterSelection = std::make_unique<PsRandom>(
                    _mt_rand, configuration["number_of_parameters"].asUInt(), aggregation_function,
                    heterogeneity_policy);
            else
                _parameterSelection = std::make_unique<PsRandom>(
                    _mt_rand, configuration["number_of_parameters"].asUInt());
        } else if(configuration["className"].asString() == ParameterSelection::SELECTBESTMUTATE) {
            if(!configuration["espilon"].empty() && !configuration["windowSize"].empty() &&
               !configuration["aggregation_function"].empty() &&
               !configuration["heterogeneity_policy"].empty())
                _parameterSelection = std::make_unique<PsSelectBestMutate>(
                    _mt_rand, configuration["number_of_parameters"].asUInt(),
                    configuration["espilon"].asDouble(), configuration["windowSize"].asUInt(),
                    aggregation_function, heterogeneity_policy);
            else
                _parameterSelection = std::make_unique<PsSelectBestMutate>(
                    _mt_rand, configuration["number_of_parameters"].asUInt());
        } else if(configuration["className"].asString() == ParameterSelection::UCBW) {
            if(!configuration["C"].empty() && !configuration["windowSize"].empty() &&
               !configuration["aggregation_function"].empty())
                _parameterSelection = std::make_unique<PsUCBW>(
                    _mt_rand, configuration["number_of_parameters"].asUInt(),
                    configuration["C"].asDouble(), configuration["windowSize"].asUInt(),
                    aggregation_function);
            else
                _parameterSelection = std::make_unique<PsUCBW>(
                    _mt_rand, configuration["number_of_parameters"].asUInt());
        } else {
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] The calculation model is not defined");
        }
        return std::move(_parameterSelection);
    }

    std::unique_ptr<Launcher> launcher(const Json::Value& configuration) {
        std::unique_ptr<Launcher> _launcher;
        if(Launcher::LAUNCHEREXEC == configuration["className"].asString())
            _launcher = std::make_unique<LauncherExec>(configuration["pathExecutable"].asString(),
                                                       configuration["string_args"].asString());
        else
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] The Launcher is not defined");

        return std::move(_launcher);
    }

    template<typename SOL>
    std::unique_ptr<RewardComputation<SOL>> rewardComputation(const Json::Value& configuration) {
        return std::make_unique<RewardComputation<SOL>>();
    }

    template<typename SOL>
    std::unique_ptr<SolutionSelection<SOL>> solutionSelection(const Json::Value& configuration) {
        std::unique_ptr<SolutionSelection<SOL>> _solutionSelection;
        if(SolutionSelection<SOL>::MAX == configuration.asString())
            _solutionSelection = std::make_unique<Selection_maximization<SOL>>();
        else if(SolutionSelection<SOL>::MIN == configuration.asString())
            _solutionSelection = std::make_unique<Selection_minimization<SOL>>();
        else
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
                                     " [-] The SolutionSelection is not defined");

        return std::move(_solutionSelection);
    }

  private:
    std::shared_ptr<std::mt19937> _mt_rand;
};

#endif