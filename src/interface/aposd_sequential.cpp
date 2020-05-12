///
/// @file aposd_sequential.h
/// @author Jxtopher
/// @brief
/// @version 0.1
/// @date 2019-08-13
///
///

#include "interface/aposd_sequential.hpp"

void Interface_sequential(int argc, char** argv, const Json::Value& configuration) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " CREATE Interface_sequential";
	std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();

	if(!configuration["seed"].empty())
		mt_rand->seed(configuration["seed"].asInt());
	else
		mt_rand->seed(static_cast<std::mt19937::result_type>(time(0)));

	ClassBuilder classBuilder(mt_rand);

	std::unique_ptr<Launcher> launcher =
		classBuilder.launcher(configuration["CalculationModel"]["Launcher"]);
	std::unique_ptr<ParameterSelection> parameterSelection =
		classBuilder.parameterSelection(configuration["CalculationModel"]["ParameterSelection"]);
	std::unique_ptr<RewardComputation<Solution<unsigned int>>> rewardComputation =
		classBuilder.rewardComputation<Solution<unsigned int>>(
			configuration["CalculationModel"]["RewardComputation"]);

	std::cout << configuration << std::endl;
	SequentialModel<Solution<unsigned int>> calculationmodel(
		std::move(launcher), std::move(parameterSelection), std::move(rewardComputation));

	calculationmodel();
}