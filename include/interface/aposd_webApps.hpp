///
/// @file aposd_webApps.h
/// @author Jxtopher
/// @date 2019-08
/// @brief Mode de communication SaaS
///        https://en.wikipedia.org/wiki/JSON-RPC
///

#ifndef JXTOPHER_APOSD_WEBAPPS_H
#define JXTOPHER_APOSD_WEBAPPS_H

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/http_response.h>
#include <cppcms/rpc_json.h>
#include <cppcms/service.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <utility>  // std:pair
#include <vector>
#include <memory>

#include "classBuilder.hpp"
#include "../parameterSelection/parameterSelection.hpp"
#include "../selection/solutionSelection.hpp"
#include "../selection/selection_maximization.hpp"
#include "../solution/solution.hpp"
#include "../calculationModel/SaaS/learningOnlineIndividual.hpp"
#include "../calculationModel/SaaS/learningOnlineCollective.hpp"

using APOSD_SOL = Solution<double>;

void Interface_webApps(int argc, char** argv, const Json::Value &configuration);
std::string jsonAsString(const Json::Value &json);
Json::Value stringAsjson(const std::string &strJson);
cppcms::json::value jsonValueASJsonCppcms(const Json::Value &jsonValue);

template <typename T>
std::string convertPointerToStringAddress(const T* obj);

template <typename T>
T* convertAddressStringToPointer(const std::string& address);

///
/// \class WebAposd
/// @brief Définie l'application web de Aposd avec cppcms.
///        Automate : initialization -> learning (many time) ->finish
///
class WebAposd : public cppcms::rpc::json_rpc_server {
   public:
    WebAposd(cppcms::service& srv);

    // curl -i -X POST --header "Content-Type:application/json" -d '{"method":"notify", "params":["msgxxxxxxxxxxxxxxxxxxxxxx"], "id":1}' http://127.0.0.1:8080/
    void echo(const std::string &msg);

    // void ping(std::string msg) { 
    //     DEBUG_TRACE("echo")
    //     cppcms::json::value json;
    //     json["pong"] = msg;  // true | false
    //     json["timer"] = "";
    //     response().out() << json;
    // }


    ///
    /// @brief construction de l'objet
    /// 
    /// @param msg 
    ///
    void initialization(const std::string &msg);
    ///
    /// @brief apprentissage adaptative
    /// 
    /// @param msg 
    ///
    void learning(const std::string &msg);

    ///
    /// @brief destruction de l'objet
    /// 
    /// @param msg
    ///
    void finish(const std::string &msg);

   private:
    std::map<LearningOnline<APOSD_SOL> *, std::pair<unsigned int, unsigned int>> methodList;  //< <@, <count, time>>
    std::map<std::string, LearningOnline<APOSD_SOL> *> group_id;     //< <group name, <@, count>>
};

#endif