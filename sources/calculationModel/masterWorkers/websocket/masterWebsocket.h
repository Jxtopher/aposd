#ifndef JXTOPHER_MASTERWEBSOCKET_H
#define JXTOPHER_MASTERWEBSOCKET_H

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
#include <vector>

#include "../../../parameterSelection/parameterSelection.h"
#include "../../../rewardComputation/rewardComputation.h"
#include "../../../selection/selection.h"
#include "../../calculationModel.h"

using namespace std;

template <class SOL>
class Learning {
   public:
    Learning(ParameterSelection* parameterSelection, RewardComputation<SOL>* rewardComputation,
             Selection<SOL>* selection)
        : _parameterSelection(parameterSelection), _rewardComputation(rewardComputation), _selection(selection) {}

    Learning(const Learning& l) {
        _parameterSelection = l._parameterSelection->clone();
        _rewardComputation = l._rewardComputation;
        _selection = l._selection;
    }

    Learning& operator=(const Learning& l) {
        _parameterSelection = l._parameterSelection->clone();
        _rewardComputation = l._rewardComputation;
        _selection = l._selection;
    }

    unsigned int getFitness(SOL s_new) {
        unsigned int parameter = 0;
        SOL s;
        pair<double, unsigned int> rewardOp = _rewardComputation->operator()(s, s_new, parameter);
        _parameterSelection->update(rewardOp);
        return _parameterSelection->getParameter();
    }

   private:
    ParameterSelection* _parameterSelection;
    RewardComputation<SOL>* _rewardComputation;
    Selection<SOL>* _selection;
};

template <class SOL>
class hello : public cppcms::application {
   public:
    hello(cppcms::service& srv, Learning<SOL> learning) : cppcms::application(srv), _learning(learning) {
        // dispatcher().assign("aa", &hello::page_home, this);

        dispatcher().assign("/initialization/(\\d+)", &hello::initialization, this, 1);
        mapper().assign("initialization", "/initialization/{1}");

        dispatcher().assign("/learning/(\\d+)/(\\d+)/(\\d+)", &hello::learning, this, 1, 2, 3);
        mapper().assign("learning", "/learning/{1}/{1}/{1}");

        dispatcher().assign("/test", &hello::test, this);
        mapper().assign("test", "/test");

        dispatcher().assign("/finish/(\\d+)", &hello::finish, this, 1);
        mapper().assign("finish", "/finish/{1}");

        // http://127.0.0.1:8080/hello
        mapper().root("/hello");
    }

    void test() {
        cppcms::json::value json;
        json["test"] = "test"; // true | false
        response().out() << json;
    }

    //--------------------------------
    void initialization(std::string nbParameter) {
        cppcms::json::value r;

        if (idmanagement.empty()) {
            idmanagement.push_back(false);
        }
        unsigned int index = 0;
        while (index < idmanagement.size() && idmanagement[index] == true) {
            index++;
        }

        if (index == idmanagement.size()) idmanagement.push_back(false);

        idmanagement[index] = true;
        pSelection[index] = 6;
        r["id"] = index;
        response().out() << r;
    }

    void learning(std::string id, std::string num_parameter, std::string solution) {
        SOL s;
        cppcms::json::value json;
        json["solution"] = "solsol";
        json["num_paramter"] = _learning.getFitness(s);
        response().out() << json;
    }
    void finish(std::string id) {
        cppcms::json::value json;
        json["ack"] = 1; // true | false
        response().out() << json;
    }

   private:
    Learning<SOL> _learning;
    std::map<unsigned int, int> pSelection;
    vector<bool> idmanagement;
};

template <class SOL>
class MasterWebsocket : public CalculationModel {
   public:
    MasterWebsocket(int argc, char** argv, ParameterSelection& parameterSelection,
                    RewardComputation<SOL>& rewardComputation, Selection<SOL>& selection)
        : _argc(argc),
          _argv(argv),
          _parameterSelection(parameterSelection),
          _rewardComputation(rewardComputation),
          _selection(selection) {}
    virtual ~MasterWebsocket() {}

    void operator()() {
        Learning<SOL> l(_parameterSelection.clone(), &_rewardComputation, &_selection);

        try {
            cppcms::service srv(_argc, _argv);
            srv.applications_pool().mount(cppcms::applications_factory<hello<SOL>>(l));
            srv.run();
        } catch (std::exception const& e) {
            std::cerr << e.what() << std::endl;
        }
    }

   private:
    int _argc;
    char** _argv;
    ParameterSelection& _parameterSelection;
    RewardComputation<SOL>& _rewardComputation;
    Selection<SOL>& _selection;
};

#endif