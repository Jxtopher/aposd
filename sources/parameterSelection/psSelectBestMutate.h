///
/// @file psSelectBestMutate.h
/// @author Jxtopher
/// @version 1
/// @date 2019-05
/// @brief Implementation Select best mutate
///		   see : Derbel et Verel - 2011 - DAMS distributed adaptive metaheuristic selection
///

#ifndef PSSELECTBESTMUTATE_H
#define	PSSELECTBESTMUTATE_H

#include <memory>
#include <random>
#include "parameterSelection.h"

class PsSelectBestMutate : public ParameterSelection {
	public:
	PsSelectBestMutate(std::shared_ptr<std::mt19937> mt_rand,
		unsigned int number_of_parameters,
		const double espilon = 0.15,
		const unsigned int windowSize = 150,
		const char* aggregation_function = AggregationFunction::MEAN,
		const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS) :
		ParameterSelection(number_of_parameters),
		_mt_rand(mt_rand),
		_espilon(espilon),
		_windowSize(windowSize),
		_aggregation_function(aggregation_function),
		_heterogeneity_policy(heterogeneity_policy) {
			urd = std::make_unique<std::uniform_real_distribution<>>(0.0, 1.0);
			uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters -1);
			reward_aggregation = std::unique_ptr<double []>(new double[number_of_parameters]);
			number_select = std::unique_ptr<unsigned int []>(new unsigned int[number_of_parameters]);
			
			sliding_window.set_capacity(windowSize);
			reset();
	}

	PsSelectBestMutate(const PsSelectBestMutate &c) : 
		ParameterSelection(c._number_of_parameters),
		_mt_rand(c._mt_rand),
		_espilon(c._espilon),
		_windowSize(c._windowSize),
		_aggregation_function(c._aggregation_function),
		_heterogeneity_policy(c._heterogeneity_policy)  {
			uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters -1);
    }

	virtual ~PsSelectBestMutate() {

	}

	ParameterSelection* clone() const { return new PsSelectBestMutate(*this); }

	void reset() {
		
		init_each_parameter = 0;
		for(unsigned int i = 0 ; i < this->_number_of_parameters ; i++) {
			reward_aggregation[i] = 0;
			number_select[i] = 0;
		}
	}

	void update(std::vector<std::pair<double, unsigned int>> &rewards) {
		if (_aggregation_function == AggregationFunction::MAX) {
			unsigned int max_index = 0;
			unsigned int min_index = 0;
			for (unsigned i = 1 ; i < rewards.size() ; i++) {
				if (rewards[max_index].first < rewards[i].first)
					max_index = i;
				if (rewards[i].first < rewards[min_index].first)
					min_index = i;
			}

			unsigned int max_parameter = rewards[max_index].second;
			unsigned int min_parameter = rewards[min_index].second;

			for (unsigned i = 0 ; i < _number_of_parameters ; i++) {
				if (max_parameter == i) 
					reward_aggregation[i] = rewards[max_index].first;
				else 
					reward_aggregation[i] = rewards[min_index].first;
			}

		} else if (_aggregation_function == AggregationFunction::MEAN) {
			for (unsigned i = 1 ; i < rewards.size() ; i++) {
				reward_aggregation[rewards[i].second] = (reward_aggregation[rewards[i].second] * number_select[rewards[i].second] + rewards[i].first) / (number_select[rewards[i].second] + 1);
				number_select[rewards[i].second]++;
			}
		} else 
			throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The aggregation function is not defined");
	}

	void update(std::pair<double, unsigned int> &rewards) {
		assert(_windowSize != 0);
		
		if (_aggregation_function == AggregationFunction::MAX) {
			sliding_window.push_front(rewards);

			const auto p = std::minmax_element(sliding_window.begin(), sliding_window.end());
			auto min = p.first->second;
			auto max = p.second->second;
			for (unsigned int i = 0 ; i < _number_of_parameters ; i++) {
				if(p.second->second == p.second->second)
					reward_aggregation[i] = p.second->first;
				else 
					reward_aggregation[i] = p.first->first;
			}
		} else if (_aggregation_function == AggregationFunction::MEAN) {
			if (sliding_window.full()) {
				unsigned int r = sliding_window.back().first;  // Reward
				unsigned int p = sliding_window.back().second; // Parameter
				assert(p < this->_number_of_parameters);
				reward_aggregation[p] = (reward_aggregation[p] * number_select[p] - r) / (number_select[p] - 1);
				number_select[p]--;
			}
			
			sliding_window.push_front(rewards);
			
			{
				unsigned int r = sliding_window.begin()->first;  // Reward
				unsigned int p = sliding_window.begin()->second; // Parameter
				assert(p < this->_number_of_parameters);
				reward_aggregation[p] = (reward_aggregation[p] * number_select[p] + r) / (number_select[p] + 1);
				number_select[p]++;
			}
		} else 
			throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The aggregation function is not defined");
		
	}

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) {
		
		std::vector<unsigned int> parameterList;

		if (_heterogeneity_policy == HeterogeneityPolicy::HETEROGENOUS) {
			for (unsigned int i = 0 ; i < number_of_nodes ; i++)
				parameterList.push_back(uid->operator()(*(this->_mt_rand)));
		} else if (_heterogeneity_policy == HeterogeneityPolicy::HOMOGENEOUS) {
			unsigned int pick = uid->operator()(*(this->_mt_rand));
			for (unsigned int i = 0 ; i < number_of_nodes ; i++)
				parameterList.push_back(pick);
		} else 
			throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The policy model is not defined");

		
		return parameterList;
	}

	unsigned int getParameter() {
		if (init_each_parameter < this->_number_of_parameters)
			return init_each_parameter++;
		else if (urd->operator()(*(this->_mt_rand)) <= _espilon)
			return uid->operator()(*(this->_mt_rand));
		else
			return std::distance(reward_aggregation.get(), 
								 std::max_element(reward_aggregation.get(), 
								 reward_aggregation.get() + this->_number_of_parameters));
		
	}

    std::string className() const {
        return "PsSelectBestMutate";
    }

	protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const double _espilon;
	const unsigned int _windowSize;
	circular_buffer<std::pair<double, unsigned int>> sliding_window;
	const char* _aggregation_function;
	const char* _heterogeneity_policy;
	std::unique_ptr<std::uniform_real_distribution<>> urd;
	std::uniform_int_distribution<unsigned int> *uid;

	unsigned int init_each_parameter;
	std::unique_ptr<double[]> reward_aggregation;
	std::unique_ptr<unsigned int[]> number_select;
};

#endif
