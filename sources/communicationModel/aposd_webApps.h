///
/// \file aposd_webApps.h
/// \author Jxtopher
/// \date 2019-08
/// \brief Mode de communication SaaS
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

#include "../parameterSelection/parameterSelection.h"
#include "../parameterSelection/psAdaptivePursuit.h"
#include "../parameterSelection/psConstant.h"
#include "../parameterSelection/psEpsilonGreedyW.h"
#include "../parameterSelection/psRandom.h"
#include "../parameterSelection/psSelectBestMutate.h"
#include "../parameterSelection/psUCBW.h"
#include "../selection/selection.h"
#include "../selection/selection_maximization.h"
#include "../solution/solution.h"

using namespace std;

void CommunicationModel_webApps(int argc, char** argv, const Json::Value &configuration);

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
    if (!(ss >> tmp)) throw std::runtime_error("Failed - invalid address!");
    return reinterpret_cast<T*>(tmp);
}

class MethodBuilder {
   public:
    MethodBuilder(unsigned int nbParameter) {
        //
        unsigned long int seed = static_cast<unsigned long int>(time(0));

        mt_rand.seed(seed);

        switch (ParameterSelection::RANDOM) {
            case ParameterSelection::ADAPTIVEPURSUIT:
                parameterSelection = new PsAdaptivePursuit(mt_rand, nbParameter);
                break;
            case ParameterSelection::CONSTANT:
                parameterSelection = new PsConstant(nbParameter, 0);
                break;
            case ParameterSelection::EPSILONGREEDY:
                parameterSelection = new PsEspsilonGreedy(mt_rand, nbParameter);
                break;
            case ParameterSelection::RANDOM:
                parameterSelection = new PsRandom(mt_rand, nbParameter);
                break;
            case ParameterSelection::SELECTBESTMUTATE:
                // parameterSelection = new PsSelectBestMutate(mt_rand, nbParameter);
                break;
            case ParameterSelection::UCBW:
                parameterSelection = new PsUCBW(mt_rand, nbParameter);
                break;
            default:
                assert("The calculation model is not defined" && false);
                break;
        }

        // Calcul de la récompense
        rewardComputation = new RewardComputation<Solution<unsigned int>>;
        // switch() {

        // Selection de la meilleurs solution
        selection = new Selection_maximization<Solution<unsigned int>>;
    }

    ~MethodBuilder() {}

    unsigned int init() { return parameterSelection->getParameter(); }

    std::pair<unsigned int, Solution<unsigned int>> learning(unsigned int parameter, Solution<unsigned int> s_new) {
        std::pair<double, unsigned int> rewardOp = rewardComputation->operator()(s, s_new, parameter);

        parameterSelection->update(rewardOp);

        s = s_new;

        unsigned int new_parameter = parameterSelection->getParameter();

        return std::pair<unsigned int, Solution<unsigned int>>(new_parameter, s);
    }

   private:
    std::mt19937 mt_rand;
    Solution<unsigned int> s;
    ParameterSelection* parameterSelection;
    RewardComputation<Solution<unsigned int>>* rewardComputation;
    Selection<Solution<unsigned int>>* selection;
};

///
/// \class WebAposd
/// \brief Définie l'application web de Aposd avec cppcms.
///        Automate : initialization -> learning (many time) ->finish
///
class WebAposd : public CommunicationModel, public cppcms::rpc::json_rpc_server {
   public:
    WebAposd(cppcms::service& srv) : cppcms::rpc::json_rpc_server(srv) {
        bind("notify", cppcms::rpc::json_method(&WebAposd::notify, this));
        bind("test", cppcms::rpc::json_method(&WebAposd::test, this));
        bind("initialization", cppcms::rpc::json_method(&WebAposd::initialization, this));
        bind("learning", cppcms::rpc::json_method(&WebAposd::learning, this));
        bind("finish", cppcms::rpc::json_method(&WebAposd::finish, this));
    }

    // curl -i -X POST --header "Content-Type:application/json" -d '{"method":"notify", "params":["msgxxxxxxxxxxxxxxxxxxxxxx"], "id":1}' http://127.0.0.1:8080/rpc
    void notify(std::string msg) { 
        std::cout << "We got notification " << msg << std::endl;
        cppcms::json::value json;
        json["test"] = "test";  // true | false
        response().out() << json;
    }

    void test() {
        cppcms::json::value json;
        json["test"] = "test";  // true | false
        response().out() << json;
    }

    ///
    /// \brief construction de l'objet
    ///
    /// \param nbParameter : le nombre de paramètre à considérer
    ///
    //void initialization(std::string nbParameter) {
    void initialization(unsigned int nbParameter) {
        cppcms::json::value json;

        MethodBuilder* method = new MethodBuilder(nbParameter);
        methodList.push_back(method);

        json["id"] = convertPointerToStringAddress(method);
        json["num_paramter"] = method->init();

        response().out() << json;
    }

    ///
    /// \brief apprentissage adaptative
    ///
    /// \param id : numero de l'objet
    /// \param num_parameter : numero du parametre utiliser
    /// \param solution : nouvelle solution avec la fitness
    ///
    void learning(std::string id, unsigned int num_parameter, std::string solution) {
        MethodBuilder* method = convertAddressStringToPointer<MethodBuilder>(id);
        cppcms::json::value json;
        
        // Vérifie que l'objet existe
        std::vector<MethodBuilder*>::iterator it = std::find(methodList.begin(), methodList.end(), method);
        if (it != methodList.end()) {
            Solution<unsigned int> s(solution);

            std::pair<unsigned int, Solution<unsigned int>> apply = method->learning(num_parameter, s);

            json["solution"] = apply.second.getSolution();
            json["num_paramter"] = apply.first;
            response().out() << json;
        } else {
            json["error"] = -1;
            json["error_msg"] = "[-] The method was not initialize";
            response().out() << json;
        }
    }

    ///
    /// \brief destruction de l'objet
    ///
    /// \param id : numero de l'objet
    ///
    void finish(std::string id) {
        cppcms::json::value json;
        MethodBuilder* method = convertAddressStringToPointer<MethodBuilder>(id);

        // Vérifie que l'objet existe
        std::vector<MethodBuilder*>::iterator it = std::find(methodList.begin(), methodList.end(), method);
        if (it != methodList.end()) {
            methodList.erase(it);

            json["ack"] = 1;  // true | false
            response().out() << json;
        } else {
            json["error"] = -1;
            json["error_msg"] = "[-] The method was not initialize";
            response().out() << json;
        }
    }

   private:
    vector<MethodBuilder*> methodList;
};

void CommunicationModel_webApps(int argc, char** argv, const Json::Value &configuration) {
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<WebAposd>());
        srv.run();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

#endif