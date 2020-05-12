///
/// @file psEpsilonGreedyW.h
/// @author Jxtopher
/// @brief
/// @version 0.1
/// @date 2019-08-19
///
/// @copyright Copyright (c) 2019
///
///

#include "parameterSelection/psEpsilonGreedyW.hpp"

PsEspsilonGreedy::PsEspsilonGreedy(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
					const double espilon, const unsigned int windowSize,
					const char* aggregation_function,
					const char* heterogeneity_policy)
	: ParameterSelection(number_of_parameters), _mt_rand(mt_rand), _espilon(espilon),
		_windowSize(windowSize), _aggregation_function(aggregation_function),
		_heterogeneity_policy(heterogeneity_policy) {
	assert(0 <= _espilon && _espilon <= 1);

	init_each_parameter = 0;
	urd = std::make_unique<std::uniform_real_distribution<>>(0.0, 1.0);
	uid = std::make_unique<std::uniform_int_distribution<unsigned int>>(
		0, this->_number_of_parameters - 1);
	sliding_window.set_capacity(windowSize);
	reward_aggregation = std::unique_ptr<double[]>(new double[number_of_parameters]);
	number_select = std::unique_ptr<unsigned int[]>(new unsigned int[number_of_parameters]);
	reset();
}

PsEspsilonGreedy::PsEspsilonGreedy(const PsEspsilonGreedy& c)
	: ParameterSelection(c._number_of_parameters), _mt_rand(c._mt_rand), _espilon(c._espilon),
		_windowSize(c._windowSize), _aggregation_function(c._aggregation_function),
		_heterogeneity_policy(c._heterogeneity_policy),
		init_each_parameter(c.init_each_parameter) {
	urd = std::make_unique<std::uniform_real_distribution<>>(0.0, 1.0);
	uid = std::make_unique<std::uniform_int_distribution<unsigned int>>(
		0, this->_number_of_parameters - 1);
	sliding_window.set_capacity(_windowSize);
	reward_aggregation = std::unique_ptr<double[]>(new double[_number_of_parameters]);
	number_select = std::unique_ptr<unsigned int[]>(new unsigned int[_number_of_parameters]);

	sliding_window = c.sliding_window;
	for(unsigned int i = 0; i < _number_of_parameters; i++) {
		reward_aggregation[i] = c.reward_aggregation[i];
		number_select[i] = c.number_select[i];
	}
}

PsEspsilonGreedy::~PsEspsilonGreedy() {}

ParameterSelection* PsEspsilonGreedy::clone() const { return new PsEspsilonGreedy(*this); }

void PsEspsilonGreedy::reset() {
	init_each_parameter = 0;
	for(unsigned int i = 0; i < this->_number_of_parameters; i++) {
		reward_aggregation[i] = 0;
		number_select[i] = 0;
	}
}

void PsEspsilonGreedy::update(std::vector<std::pair<double, unsigned int>>& rewards) {
	for(std::vector<std::pair<double, unsigned int>>::iterator it = rewards.begin();
		it != rewards.end(); ++it)
		update(*it);
}

///
/// @brief update the reward aggregation for a parameter
///
/// @param rewards : rewards is a pair of raward and parameter
///
void PsEspsilonGreedy::update(std::pair<double, unsigned int>& rewards) {
	if(_aggregation_function == AggregationFunction::MAX) {
		sliding_window.push_front(rewards);

		const auto p = std::minmax_element(sliding_window.begin(), sliding_window.end());
		auto min = p.first->second;
		auto max = p.second->second;
		for(unsigned int i = 0; i < _number_of_parameters; i++) {
			if(p.second->second == p.second->second)
				reward_aggregation[i] = p.second->first;
			else
				reward_aggregation[i] = p.first->first;
		}
	} else if(_aggregation_function == AggregationFunction::MEAN) {
		if(sliding_window.full()) {
			unsigned int r = sliding_window.back().first;  // Reward
			unsigned int p = sliding_window.back().second; // Parameter
			assert(p < this->_number_of_parameters);
			reward_aggregation[p] =
				(reward_aggregation[p] * number_select[p] - r) / (number_select[p] - 1);
			number_select[p]--;
		}

		sliding_window.push_front(rewards);

		{
			unsigned int r = sliding_window.begin()->first;  // Reward
			unsigned int p = sliding_window.begin()->second; // Parameter
			assert(p < this->_number_of_parameters);
			reward_aggregation[p] =
				(reward_aggregation[p] * number_select[p] + r) / (number_select[p] + 1);
			number_select[p]++;
		}
	} else {
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									" [-] The aggregation function is not defined");
	}
}

std::vector<unsigned int> PsEspsilonGreedy::getParameter(const unsigned int number_of_nodes) {
	std::vector<unsigned int> parameterList;

	unsigned int bestParameter =
		std::distance(reward_aggregation.get(),
						std::max_element(reward_aggregation.get(),
										reward_aggregation.get() + this->_number_of_parameters));
	if(_heterogeneity_policy == HeterogeneityPolicy::HETEROGENOUS) {
		if(init_each_parameter < this->_number_of_parameters) {
			init_each_parameter = number_of_nodes < _number_of_parameters
										? number_of_nodes
										: _number_of_parameters;
			for(unsigned int i = 0; i < number_of_nodes; i++) {
				parameterList.push_back(i % init_each_parameter);
			}
		} else {
			for(unsigned int i = 0; i < number_of_nodes; i++) {
				if(urd->operator()(*(this->_mt_rand)) <= _espilon)
					parameterList.push_back(uid->operator()(*(this->_mt_rand)));
				else
					parameterList.push_back(bestParameter);
			}
		}
	} else if(_heterogeneity_policy == HeterogeneityPolicy::HOMOGENEOUS) {
		if(init_each_parameter < this->_number_of_parameters) {
			for(unsigned int i = 0; i < number_of_nodes; i++)
				parameterList.push_back(init_each_parameter);
			init_each_parameter++;
		} else {
			if(urd->operator()(*(this->_mt_rand)) <= _espilon) {
				for(unsigned int i = 0; i < number_of_nodes; i++)
					parameterList.push_back(uid->operator()(*(this->_mt_rand)));
			} else {
				for(unsigned int i = 0; i < number_of_nodes; i++)
					parameterList.push_back(bestParameter);
			}
		}
	} else
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									" [-] The policy model is not defined");

	return parameterList;
}

unsigned int PsEspsilonGreedy::getParameter() {
	if(init_each_parameter < this->_number_of_parameters) {
		return init_each_parameter++;
	} else if(urd->operator()(*(this->_mt_rand)) <= _espilon) {
		return uid->operator()(*(this->_mt_rand));
	} else {
		return std::distance(
			reward_aggregation.get(),
			std::max_element(reward_aggregation.get(),
								reward_aggregation.get() + this->_number_of_parameters));
	}
}

double PsEspsilonGreedy::getreward(const unsigned int parameter) const {
	assert(parameter < this->_number_of_parameters);
	return reward_aggregation[parameter];
}

std::string PsEspsilonGreedy::className() const { return "PsEspsilonGreedy"; }