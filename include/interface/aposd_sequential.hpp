///
/// @file aposd_sequential.h
/// @author Jxtopher
/// @brief
/// @version 0.1
/// @date 2019-08-13
///
///

#ifndef JXTOPHER_APOSD_SEQUENTIAL_H
#define JXTOPHER_APOSD_SEQUENTIAL_H

#include <jsoncpp/json/json.h>
#include <memory>

#include "../calculationModel/sequentialModel/sequentialModel.hpp"
#include "../launcher/launcher.hpp"
#include "../solution/solution.hpp"
#include "classBuilder.hpp"

void Interface_sequential(int argc, char** argv, const Json::Value& configuration);

#endif