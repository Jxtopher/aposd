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
#include <iostream>
#include <map>
#include <memory>
#include <unistd.h>
#include <utility> // std:pair
#include <vector>

#include "../calculationModel/SaaS/learningOnlineCollective.hpp"
#include "../calculationModel/SaaS/learningOnlineIndividual.hpp"
#include "../parameterSelection/parameterSelection.hpp"
#include "../selection/selection_maximization.hpp"
#include "../selection/solutionSelection.hpp"
#include "../solution/solution.hpp"
#include "classBuilder.hpp"

using APOSD_SOL = Solution<double>;

void Interface_webApps(int argc, char** argv, const Json::Value& configuration);
std::string jsonAsString(const Json::Value& json);
Json::Value stringAsjson(const std::string& strJson);
cppcms::json::value jsonValueASJsonCppcms(const Json::Value& jsonValue);

//-----------------------------------------------------------------------------
template<typename T> std::string convertPointerToStringAddress(const T* obj) {
	long long int address(reinterpret_cast<long long int>(obj));
	std::stringstream ss;
	ss << address;
	return ss.str();
}

template<typename T> T* convertAddressStringToPointer(const std::string& address) {
	std::stringstream ss;
	ss << address;
	long long int tmp(0);
	if(!(ss >> tmp))
		throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
								 " Failed - invalid address!");
	return reinterpret_cast<T*>(tmp);
}

//-----------------------------------------------------------------------------
std::string jsonAsString(const Json::Value& json) {
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";
	return Json::writeString(builder, json);
}

Json::Value stringAsjson(const std::string& strJson) {
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(strJson.c_str(), root);
	if(!parsingSuccessful)
		throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) + " " +
								 reader.getFormattedErrorMessages());
	return root;
}

cppcms::json::value jsonValueASJsonCppcms(const Json::Value& jsonValue) {
	std::stringstream ss;
	ss << jsonAsString(jsonValue);
	cppcms::json::value tmp;
	tmp.load(ss, true);
	return tmp;
}

//-----------------------------------------------------------------------------

///
/// \class WebAposd
/// @brief Définie l'application web de Aposd avec cppcms.
///        Automate : initialization -> learning (many time) ->finish
///
class WebAposd : public cppcms::rpc::json_rpc_server {
  public:
	WebAposd(cppcms::service& srv) : cppcms::rpc::json_rpc_server(srv) {
		bind("echo", cppcms::rpc::json_method(&WebAposd::echo, this));
		// bind("ping", cppcms::rpc::json_method(&WebAposd::ping, this));
		bind("initialization", cppcms::rpc::json_method(&WebAposd::initialization, this));
		bind("learning", cppcms::rpc::json_method(&WebAposd::learning, this));
		bind("finish", cppcms::rpc::json_method(&WebAposd::finish, this));
	}

	// curl -i -X POST --header "Content-Type:application/json" -d '{"method":"notify",
	// "params":["msgxxxxxxxxxxxxxxxxxxxxxx"], "id":1}' http://127.0.0.1:8080/
	void echo(const std::string& msg) {
		BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " echo";
		cppcms::json::value json;
		json["your_msg"] = msg; // true | false
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
	void initialization(const std::string& msg) {
		Json::Value configuration = stringAsjson(msg);
		cppcms::json::value answer;
		try {
			if(configuration["group_id"].empty()) {
				std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();
				if(!configuration["seed"].empty())
					mt_rand->seed(configuration["seed"].asInt());
				else
					mt_rand->seed(static_cast<std::mt19937::result_type>(time(0)));

				std::unique_ptr<ClassBuilder> classBuilder =
					std::make_unique<ClassBuilder>(mt_rand);

				std::unique_ptr<ParameterSelection> parameterSelection =
					classBuilder->parameterSelection(
						configuration["CalculationModel"]["ParameterSelection"]);
				std::unique_ptr<RewardComputation<APOSD_SOL>> rewardComputation =
					classBuilder->rewardComputation<APOSD_SOL>(
						configuration["CalculationModel"]["RewardComputation"]);
				std::unique_ptr<SolutionSelection<APOSD_SOL>> solutionSelection =
					classBuilder->solutionSelection<APOSD_SOL>(
						configuration["CalculationModel"]["SolutionSelection"]);

				LearningOnline<APOSD_SOL>* calculationmodel =
					new LearningOnlineIndividual<APOSD_SOL>(std::move(parameterSelection),
															std::move(rewardComputation),
															std::move(solutionSelection));

				methodList[calculationmodel] = std::pair<unsigned int, unsigned int>(0, 0);

				// Build answer
				std::pair<APOSD_SOL, unsigned int> buff = calculationmodel->initial_solution(
					APOSD_SOL(configuration["initial_solution"]));
				answer["object_id"] = convertPointerToStringAddress(calculationmodel);
				answer["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
				answer["num_paramter"] = buff.second;

				BOOST_LOG_TRIVIAL(debug)
					<< __FILE__ << ":" << __LINE__ << " initialization Individual "
					<< convertPointerToStringAddress(calculationmodel);
			} else {
				if(group_id.count(configuration["group_id"].asString()) > 0) { // Found
					// Build answer
					cppcms::json::value answer;
					std::pair<APOSD_SOL, unsigned int> buff =
						group_id[configuration["group_id"].asString()]->initial_solution(
							APOSD_SOL(configuration["initial_solution"]));
					answer["object_id"] = convertPointerToStringAddress(
						group_id[configuration["group_id"].asString()]);
					answer["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
					answer["num_paramter"] = buff.second;

					BOOST_LOG_TRIVIAL(debug)
						<< __FILE__ << ":" << __LINE__ << " initialization Collective";
				} else { // Not Found
					std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();
					if(!configuration["seed"].empty())
						mt_rand->seed(configuration["seed"].asInt());
					else
						mt_rand->seed(static_cast<std::mt19937::result_type>(time(0)));

					std::unique_ptr<ClassBuilder> classBuilder =
						std::make_unique<ClassBuilder>(mt_rand);

					std::unique_ptr<ParameterSelection> parameterSelection =
						classBuilder->parameterSelection(
							configuration["CalculationModel"]["ParameterSelection"]);
					std::unique_ptr<RewardComputation<APOSD_SOL>> rewardComputation =
						classBuilder->rewardComputation<APOSD_SOL>(
							configuration["CalculationModel"]["RewardComputation"]);
					std::unique_ptr<SolutionSelection<APOSD_SOL>> solutionSelection =
						classBuilder->solutionSelection<APOSD_SOL>(
							configuration["CalculationModel"]["SolutionSelection"]);

					LearningOnline<APOSD_SOL>* calculationmodel =
						new LearningOnlineCollective<APOSD_SOL>(std::move(parameterSelection),
																std::move(rewardComputation),
																std::move(solutionSelection));

					// methodList.push_back(calculationmodel);
					methodList[calculationmodel] = std::pair<unsigned int, unsigned int>(0, 0);
					group_id[configuration["group_id"].asString()] = calculationmodel;

					// Build answer
					std::pair<APOSD_SOL, unsigned int> buff = calculationmodel->initial_solution(
						APOSD_SOL(configuration["initial_solution"]));
					answer["object_id"] = convertPointerToStringAddress(calculationmodel);
					answer["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
					answer["num_paramter"] = buff.second;

					BOOST_LOG_TRIVIAL(debug)
						<< __FILE__ << ":" << __LINE__ << " initialization Collective"
						<< convertPointerToStringAddress(calculationmodel);
				}
			}
			response().out() << answer;
		} catch(...) {
			answer["error"] = -1;
			answer["error_msg"] = "[-] can't build class";
			response().out() << answer;
		}
	}

	///
	/// @brief apprentissage adaptative
	///
	/// @param msg
	///
	void learning(const std::string& msg) {
		Json::Value data = stringAsjson(msg);
		cppcms::json::value answer;

		LearningOnline<APOSD_SOL>* calculationmodel =
			convertAddressStringToPointer<LearningOnline<APOSD_SOL>>(data["object_id"].asString());
		BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__
								 << "learning " + convertPointerToStringAddress(calculationmodel);

		std::map<LearningOnline<APOSD_SOL>*, std::pair<unsigned int, unsigned int>>::iterator it;
		if((it = methodList.find(calculationmodel)) == methodList.end()) { // not found
			answer["error"] = -1;
			answer["error_msg"] = "[-] The method was not initialize";
			response().out() << answer;
		} else { // found
			// Response
			std::pair<APOSD_SOL, unsigned int> buff = calculationmodel->run(
				APOSD_SOL(data["Solution_t0"]), APOSD_SOL(data["Solution_t1"]),
				data["num_paramter"].asUInt());
			answer["object_id"] = convertPointerToStringAddress(calculationmodel);
			answer["Solution"] = jsonValueASJsonCppcms(buff.first.asJson());
			answer["num_paramter"] = buff.second;
			response().out() << answer;
		}
	}

	///
	/// @brief destruction de l'objet
	///
	/// @param msg
	///
	void finish(const std::string& msg) {
		Json::Value data = stringAsjson(msg);
		cppcms::json::value answer;

		if(data["object_id"].empty()) {
			answer["ack"] = 0;
			answer["error_msg"] = "[-] \"object_id\" is empty";
			response().out() << answer;
			return;
		}

		// cppcms::json::value json;
		LearningOnline<APOSD_SOL>* calculationmodel =
			convertAddressStringToPointer<LearningOnlineIndividual<APOSD_SOL>>(
				data["object_id"].asString());
		BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__
								 << " finish " + data["object_id"].asString();

		// Vérifie que l'objet existe
		std::map<LearningOnline<APOSD_SOL>*, std::pair<unsigned int, unsigned int>>::iterator it;
		if((it = methodList.find(calculationmodel)) == methodList.end()) { // not found
			answer["ack"] = 0;
			answer["error"] = -1;
			answer["error_msg"] = "[-] The method was not initialize";
			response().out() << answer;
		} else { // found
			methodList.erase(it);

			for(auto it2 = group_id.begin(); it2 != group_id.end(); ++it2) {
				if(it2->second == calculationmodel) {
					group_id.erase(it2);
					break;
				}
			}
			answer["ack"] = 1; // true | false
			response().out() << answer;
		}
	}

  private:
	std::map<LearningOnline<APOSD_SOL>*, std::pair<unsigned int, unsigned int>>
		methodList;												//< <@, <count, time>>
	std::map<std::string, LearningOnline<APOSD_SOL>*> group_id; //< <group name, <@, count>>
};

void Interface_webApps(int argc, char** argv, const Json::Value& configuration) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " CREATE Interface_webApps";
	try {
		cppcms::service srv(argc, argv);
		srv.applications_pool().mount(cppcms::applications_factory<WebAposd>());
		srv.run();
	} catch(std::exception const& e) { std::cerr << e.what() << std::endl; }
}

#endif