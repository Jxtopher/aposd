/// 
/// \file aposd_sequential.h
/// \author Jxtopher
/// \brief 
/// \version 0.1
/// \date 2019-08-13
/// 
/// 

#ifndef JXTOPHER_APOSD_SEQUENTIAL_H
#define JXTOPHER_APOSD_SEQUENTIAL_H

#include <memory>
#include <jsoncpp/json/json.h>

#include "../launcher/launcher.h"
#include "../calculationModel/sequentialModel/sequentialModel.h"
#include "classBuilder.h"

void CommunicationModel_sequential(int argc, char** argv, const Json::Value &configuration);

void CommunicationModel_sequential(int argc, char** argv, const Json::Value &configuration) {
    std::mt19937 mt_rand;
    ClassBuilder classBuilder(mt_rand);

    if (!configuration["seed"].empty())
        mt_rand.seed(configuration["seed"].isInt());
    else
        mt_rand.seed(static_cast<mt19937::result_type>(time(0)));
    
    unique_ptr<Launcher> launcher = classBuilder.launcher(configuration["Launcher"]);
    unique_ptr<ParameterSelection> parameterSelection = classBuilder.parameterSelection(configuration["ParameterSelection"]);
    unique_ptr<RewardComputation<Solution<unsigned int>>> rewardComputation = classBuilder.rewardComputation<Solution<unsigned int>>(configuration["RewardComputation"]);

    cout<<configuration<<endl;
    SequentialModel<Solution<unsigned int>> calculationmodel(
    	std::move(launcher), 
    	std::move(parameterSelection), 
    	std::move(rewardComputation));
    
    calculationmodel();
}

#endif