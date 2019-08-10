#ifndef JXTOPHER_APOSD_WEBAPPS_H
#define JXTOPHER_APOSD_WEBAPPS_H

#include <cppcms/application.h>
#include <cppcms/applications_pool.h>
#include <cppcms/http_response.h>
#include <cppcms/service.h>
#include <cppcms/url_dispatcher.h>
#include <cppcms/url_mapper.h>
#include <unistd.h>
#include <iostream>
#include <map>
#include <thread>
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

void CommunicationModel_webApps(int argc, char** argv);

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
        std::mt19937 mt_rand;
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

    std::pair<unsigned int, Solution<unsigned int>> learning(unsigned int parameter, Solution<unsigned int> s_new) {
        std::pair<double, unsigned int> rewardOp = rewardComputation->operator()(s, s_new, parameter);

        parameterSelection->update(rewardOp);

        s = s_new;

        unsigned int new_parameter = parameterSelection->getParameter();

        return std::pair<unsigned int, Solution<unsigned int>>(new_parameter, s);
    }

   private:
    Solution<unsigned int> s;
    ParameterSelection* parameterSelection;
    RewardComputation<Solution<unsigned int>>* rewardComputation;
    Selection<Solution<unsigned int>>* selection;
};

class hello1 : public cppcms::application {
   public:
    hello1(cppcms::service& srv) : cppcms::application(srv) {
        // dispatcher().assign("aa", &hello::page_home, this);

        dispatcher().assign("/initialization/(\\d+)", &hello1::initialization, this, 1);
        mapper().assign("initialization", "/initialization/{1}");

        dispatcher().assign("/learning/(\\d+)/(\\d+)/((.*))", &hello1::learning, this, 1, 2, 3);
        mapper().assign("learning", "/learning/{1}/{1}/{1}");

        dispatcher().assign("/test", &hello1::test, this);
        mapper().assign("test", "/test");

        dispatcher().assign("/finish/(\\d+)", &hello1::finish, this, 1);
        mapper().assign("finish", "/finish/{1}");

        // http://127.0.0.1:8080/hello
        mapper().root("/hello");
    }

    void test() {
        cppcms::json::value json;
        json["test"] = "test";  // true | false
        response().out() << json;
    }

    void initialization(std::string nbParameter) {
        cppcms::json::value r;

        stringstream convert(nbParameter);
        int _nbParameter = 0;
        convert >> _nbParameter;

        MethodBuilder* x = new MethodBuilder(_nbParameter);
        methodList.push_back(x);

        r["id"] = convertPointerToStringAddress(x);

        response().out() << r;
    }

    void learning(std::string id, std::string num_parameter, std::string solution) {
        MethodBuilder* method = convertAddressStringToPointer<MethodBuilder>(id);
        cppcms::json::value json;

        stringstream convert(num_parameter);
        int _num_parameter = 0;
        convert >> _num_parameter;

        // Vérifie que l'objet existe
        std::vector<MethodBuilder*>::iterator it = std::find(methodList.begin(), methodList.end(), method);
        if (it != methodList.end()) {
            Solution<unsigned int> s(solution);

            std::pair<unsigned int, Solution<unsigned int>> apply = method->learning(_num_parameter, s);

            json["solution"] = apply.second.getSolution();
            json["num_paramter"] = apply.first;
            response().out() << json;
        } else {
            json["error"] = -1;
            json["error_msg"] = "[-] The method was not initialize";
            response().out() << json;
        }
    }

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
    std::map<unsigned int, int> pSelection;
    vector<MethodBuilder*> methodList;
};

void CommunicationModel_webApps(int argc, char** argv) {
    try {
        cppcms::service srv(argc, argv);
        srv.applications_pool().mount(cppcms::applications_factory<hello1>());
        srv.run();
    } catch (std::exception const& e) {
        std::cerr << e.what() << std::endl;
    }
}

#endif