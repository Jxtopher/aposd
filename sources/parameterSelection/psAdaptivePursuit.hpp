///
/// @file psAdaptivePursuit.h
/// @author Jxtopher
/// @version 1
/// @date 2019-05
/// @brief Implementation adaptive pursuit strategy
///		   see : Thierens - 2005 - An adaptive pursuit strategy for allocating operator
/// Probabilities
///

#ifndef PSADAPTIVEPURSUIT_H
#define PSADAPTIVEPURSUIT_H

#include "parameterSelection.hpp"
#include <memory>
#include <random>

class PsAdaptivePursuit : public ParameterSelection {
  public:
	PsAdaptivePursuit(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
					  const double alpha = 0.2, const double beta = 0.2, const double p_min = 0.1,
					  const double p_max = 0.9,
					  const char* aggregation_function = AggregationFunction::MEAN,
					  const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS)
		: ParameterSelection(number_of_parameters), _mt_rand(mt_rand), _alpha(alpha), _beta(beta),
		  _p_min(p_min), _p_max(p_max), _aggregation_function(aggregation_function),
		  _heterogeneity_policy(heterogeneity_policy) {
		uid = std::make_unique<std::uniform_int_distribution<unsigned int>>(
			0, this->_number_of_parameters - 1);
		reward_estimate = std::make_unique<double[]>(number_of_parameters);
		selection_probability = std::make_unique<double[]>(number_of_parameters);
		reset();
	}

	PsAdaptivePursuit(const PsAdaptivePursuit& c)
		: ParameterSelection(c._number_of_parameters), _mt_rand(c._mt_rand), _alpha(c._alpha),
		  _beta(c._beta), _p_min(c._p_min), _p_max(c._p_max),
		  _aggregation_function(c._aggregation_function),
		  _heterogeneity_policy(c._heterogeneity_policy) {
		uid = std::make_unique<std::uniform_int_distribution<unsigned int>>(
			0, this->_number_of_parameters - 1);
		reward_estimate = std::make_unique<double[]>(_number_of_parameters);
		selection_probability = std::make_unique<double[]>(_number_of_parameters);

		for(unsigned int i = 0; i < _number_of_parameters; i++) {
			reward_estimate[i] = c.reward_estimate[i];
			selection_probability[i] = c.selection_probability[i];
		}
	}

	virtual ~PsAdaptivePursuit() {}

	template<typename T>
	static unsigned int roulette_wheel(const T* array, unsigned int size, std::mt19937& mt_rand) {
		T sum = std::accumulate(array, array + size, 0.0);

		std::uniform_real_distribution<> urd(0, sum);
		T pick = urd(mt_rand);

		sum = 0;
		unsigned int index = 0;
		while(index < size && sum < pick) { sum += array[index++]; }
		return index - 1;
	}

	ParameterSelection* clone() const { return new PsAdaptivePursuit(*this); }

	void reset() {
		init_each_parameter = 0;
		for(unsigned int i = 0; i < _number_of_parameters; i++) {
			reward_estimate[i] = 0;
			selection_probability[i] = 0;
		}
	}

	///
	/// @brief Collective version
	///
	void update(std::vector<std::pair<double, unsigned int>>& rewards) {
		for(std::vector<std::pair<double, unsigned int>>::iterator it = rewards.begin();
			it != rewards.end(); ++it)
			update(*it);
	}

	///
	/// @brief Individual version
	///
	/// @param rewards : give a pair<reward, paramter>
	///
	void update(std::pair<double, unsigned int>& rewards) {
		// rewards.first =  reward (double)
		// rewards.second =  parameter (unsigned int)
		reward_estimate[rewards.second] =
			(1 - _alpha) * reward_estimate[rewards.second] + _alpha * rewards.first;
		_selection_probability();
	}

	void _selection_probability() {
		double reward_estimate_max = reward_estimate[0];

		for(unsigned int i = 1; i < this->_number_of_parameters; i++) {
			if(reward_estimate_max < reward_estimate[i]) reward_estimate_max = reward_estimate[i];
		}

		for(unsigned int i = 0; i < this->_number_of_parameters; i++) {
			if(reward_estimate_max == reward_estimate[i]) {
				selection_probability[i] =
					selection_probability[i] + _beta * (_p_max - selection_probability[i]);
			} else {
				selection_probability[i] =
					selection_probability[i] + _beta * (_p_min - selection_probability[i]);
			}
		}
	}

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) {
		std::vector<unsigned int> parameterList;

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
					parameterList.push_back(roulette_wheel<double>(
						selection_probability.get(), _number_of_parameters, *_mt_rand));
				}
			}

		} else if(_heterogeneity_policy == HeterogeneityPolicy::HOMOGENEOUS) {
			if(init_each_parameter < this->_number_of_parameters) {
				for(unsigned int i = 0; i < number_of_nodes; i++)
					parameterList.push_back(init_each_parameter);
				init_each_parameter++;
			} else {
				unsigned int pick = roulette_wheel<double>(selection_probability.get(),
														   _number_of_parameters, *_mt_rand);
				for(unsigned int i = 0; i < number_of_nodes; i++) parameterList.push_back(pick);
			}
		} else {
			throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									 " [-] The policy model is not defined");
		}

		return parameterList;
	}

	unsigned int getParameter() {
		if(init_each_parameter < this->_number_of_parameters) {
			return init_each_parameter++;
		} else {
			return roulette_wheel(selection_probability.get(), _number_of_parameters, *_mt_rand);
		}
	}

	std::string className() const { return "PsAdaptivePursuit"; }

  protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const double _alpha; // adaptation rate
	const double _beta;  // learning rate
	const double _p_min;
	const double _p_max;

	const char* _aggregation_function;
	const char* _heterogeneity_policy;

	unsigned int init_each_parameter;
	std::unique_ptr<std::uniform_int_distribution<unsigned int>> uid;
	std::unique_ptr<double[]> reward_estimate;
	std::unique_ptr<double[]> selection_probability;
};

#endif