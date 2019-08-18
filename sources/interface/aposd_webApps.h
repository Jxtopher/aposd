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

#include "classBuilder.h"
#include "../parameterSelection/parameterSelection.h"
#include "../selection/selection.h"
#include "../selection/selection_maximization.h"
#include "../solution/solution.h"
#include "../calculationModel/SaaS/learningOnline.h"




using APOSD_SOL = Solution<unsigned int>;

void Interface_webApps(int argc, char** argv, const Json::Value &configuration);
std::string jsonAsString(const Json::Value &json);
Json::Value stringAsjson(const std::string &strJson);
cppcms::json::value jsonValueASJsonCppcms(const Json::Value &jsonValue);

//-----------------------------------------------------------------------------
template <typename T>
std::string convertPointerToStringAddress(const T* obj) {
    long long int address(reinterpret_cast<long long int>(obj));
    std::stringstream ss;
    ss << address;
    return ss.str();
}

template <typename T>
T* convertAddressStringToPointer(const std::string& address) {
    std::stringstream ss;
    ss << address;
    long long int tmp(0);
    if (!(ss >> tmp)) throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +" Failed - invalid address!");
    return reinterpret_cast<T*>(tmp);
}

//-----------------------------------------------------------------------------
std::string jsonAsString(const Json::Value &json) {
    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "";
    return Json::writeString(builder, json);
}

Json::Value stringAsjson(const std::string &strJson) {
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse(strJson.c_str(), root);
    if (!parsingSuccessful) throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " " + reader.getFormattedErrorMessages());
    return root;
}

cppcms::json::value jsonValueASJsonCppcms(const Json::Value &jsonValue) {
    std::stringstream ss;
    ss << jsonAsString(jsonValue);
    cppcms::json::value tmp;
    tmp.load(ss,true);
    return tmp;
}


//-----------------------------------------------------------------------------

///
/// \class WebAposd
/// @brief Définie l'application web de Aposd avec cppcms.
///        Automate : initialization -> learning (many time) ->finish
///
class WebAposd : public Interface, public cppcms::rpc::json_rpc_server {
   public:
    WebAposd(cppcms::service& srv) : cppcms::rpc::json_rpc_server(srv) {
        bind("echo", cppcms::rpc::json_method(&WebAposd::echo, this));
        // bind("ping", cppcms::rpc::json_method(&WebAposd::ping, this));
        bind("initialization", cppcms::rpc::json_method(&WebAposd::initialization, this));
        bind("learning", cppcms::rpc::json_method(&WebAposd::learning, this));
        bind("finish", cppcms::rpc::json_method(&WebAposd::finish, this));
    }

    // curl -i -X POST --header "Content-Type:application/json" -d '{"method":"notify", "params":["msgxxxxxxxxxxxxxxxxxxxxxx"], "id":1}' http://127.0.0.1:8080/
    void echo(const std::string &msg) { 
        DEBUG_TRACE("echo")
        cppcms::json::value json;
        json["your_msg"] = msg;  // true | false
        json["help"] = "initialization | learning | finish";
        response().out() << json;
    }

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
    void initialization(const std::string &msg) {
        Json::Value configuration = stringAsjson(msg);

        std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();
        if (!configuration["seed"].empty())
            mt_rand->seed(configuration["seed"].isInt());
        else
            mt_rand->seed(static_cast<std::mt19937::result_type>(time(0)));

        std::unique_ptr<ClassBuilder> classBuilder = std::make_unique<ClassBuilder>(mt_rand);

        std::unique_ptr<ParameterSelection> parameterSelection = classBuilder->parameterSelection(configuration["CalculationModel"]["ParameterSelection"]);
        std::unique_ptr<RewardComputation<APOSD_SOL>> rewardComputation = classBuilder->rewardComputation<APOSD_SOL>(configuration["CalculationModel"]["RewardComputation"]);

        LearningOnline<APOSD_SOL> *calculationmodel = new LearningOnline<APOSD_SOL>( 
            std::move(parameterSelection), 
            std::move(rewardComputation));

        methodList.push_back(std::move(calculationmodel));

        // Response
        cppcms::json::value r;
        std::pair<APOSD_SOL, unsigned int> buff = calculationmodel->initialSolution(APOSD_SOL(configuration["initialSolution"]));
        r["objectId"] = convertPointerToStringAddress(calculationmodel);
        r["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
        r["num_paramter"] = buff.second;
        response().out()<<r;

        DEBUG_TRACE("initialization " + convertPointerToStringAddress(calculationmodel))
    }

    ///
    /// @brief apprentissage adaptative
    /// 
    /// @param msg 
    ///
    void learning(const std::string &msg) {
        Json::Value data = stringAsjson(msg);
        LearningOnline<APOSD_SOL>* calculationmodel = convertAddressStringToPointer<LearningOnline<APOSD_SOL>>(data["objectId"].asString());
        DEBUG_TRACE("learning " + convertPointerToStringAddress(calculationmodel))
        
        // Response
        cppcms::json::value r;
        std::pair<APOSD_SOL, unsigned int> buff = calculationmodel->run(APOSD_SOL(data["Solution_t0"]), APOSD_SOL(data["Solution_t1"]),  data["num_paramter"].asUInt());
        r["objectId"] = convertPointerToStringAddress(calculationmodel);
        r["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
        r["num_paramter"] = buff.second;
        response().out()<<r;
    }

    ///
    /// @brief destruction de l'objet
    /// 
    /// @param msg
    ///
    void finish(const std::string &msg) {
        Json::Value data = stringAsjson(msg);
        if (data["objectId"].empty()) {
            cppcms::json::value r;
            r["ack"] = 0;
            r["error_msg"] = "[-] \"objectId\" is empty";
            response().out()<<r;
            return ;
        }

        // cppcms::json::value json;
        LearningOnline<APOSD_SOL>* method = convertAddressStringToPointer<LearningOnline<APOSD_SOL>>(data["objectId"].asString());
        DEBUG_TRACE("finish " + data["objectId"].asString())

        // Vérifie que l'objet existe
        std::vector<LearningOnline<APOSD_SOL>*>::iterator it = std::find(methodList.begin(), methodList.end(), method);

        cppcms::json::value r;
        if (it != methodList.end()) {
            methodList.erase(it);
            r["ack"] = 1;  // true | false
            response().out()<<r;
        } else {
            r["ack"] = 0;
            r["error_msg"] = "[-] The method was not initialize";
            response().out()<<r;
        }
    }

   private:
    std::vector<LearningOnline<APOSD_SOL> *> methodList;
};

void Interface_webApps(int argc, char** argv, const Json::Value &configuration) {
    DEBUG_TRACE("CREATE Interface_webApps")
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<WebAposd>());
        srv.run();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

#endif