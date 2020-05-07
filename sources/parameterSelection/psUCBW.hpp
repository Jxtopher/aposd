///
/// @file psUCBW.h
/// @author Jxtopher
/// @version 1
/// @date 2019-05
/// @brief Implementation UCB with sliding windows
///		   see : Auer - 2002 - Finite-time Analysis of the Multiarmed Bandit Problem
///        see : DaCosta et al. - 2008 - Adaptive Operator Selection with Dynamic Multi-Armed
///        Bandits
///

#ifndef PSUCBW_H
#define PSUCBW_H

#include <cmath>
#include <memory>
#include <random>

#include <boost/circular_buffer.hpp>

#include "parameterSelection.hpp"

using namespace boost;

class PsUCBW : public ParameterSelection {
  public:
	PsUCBW(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
		   const double C = 0.03, const unsigned int windowSize = 300,
		   const char* aggregation_function = AggregationFunction::MEAN)
		: ParameterSelection(number_of_parameters), _mt_rand(mt_rand), _C(C),
		  _windowSize(windowSize), _aggregation_function(aggregation_function) {
		uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
		sliding_window.set_capacity(windowSize);
		reward_aggregation = std::unique_ptr<double[]>(new double[number_of_parameters]);
		number_select = std::unique_ptr<unsigned int[]>(new unsigned int[number_of_parameters]);
		Q = std::unique_ptr<double[]>(new double[number_of_parameters]);
		reset();
	}

	PsUCBW(const PsUCBW& c)
		: ParameterSelection(c._number_of_parameters), _mt_rand(c._mt_rand), _C(c._C),
		  _windowSize(c._windowSize), _aggregation_function(c._aggregation_function) {
		uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
		sliding_window.set_capacity(_windowSize);
		reward_aggregation = std::unique_ptr<double[]>(new double[_number_of_parameters]);
		number_select = std::unique_ptr<unsigned int[]>(new unsigned int[_number_of_parameters]);
		Q = std::unique_ptr<double[]>(new double[_number_of_parameters]);

		sliding_window = c.sliding_window;
		for(unsigned int i = 0; i < _number_of_parameters; i++) {
			reward_aggregation[i] = c.reward_aggregation[i];
			number_select[i] = c.number_select[i];
			Q[i] = c.Q[i];
		}
	}

	virtual ~PsUCBW() {}

	ParameterSelection* clone() const { return new PsUCBW(*this); }

	void reset() {
		init_each_parameter = 0;
		for(unsigned int i = 0; i < this->_number_of_parameters; i++) {
			reward_aggregation[i] = 0;
			number_select[i] = 0;
			Q[i] = 0;
		}
	}

	void update(std::vector<std::pair<double, unsigned int>>& rewards) {
		for(std::vector<std::pair<double, unsigned int>>::iterator it = rewards.begin();
			it != rewards.end(); ++it)
			update(*it);
	}

	///
	/// @brief update the reward aggregation for a parameter
	///
	/// @param rewards is a pair of raward and parameter
	///
	void update(std::pair<double, unsigned int>& rewards) {
		// Update reward_aggregation
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
		} else
			throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									 " [-] The aggregation function is not defined");

		// Update Q
		for(unsigned int i = 0; i < this->_number_of_parameters; i++) {
			if(number_select[i] != 0)
				Q[i] = reward_aggregation[i] +
					   _C * std::sqrt((2 * std::log(sliding_window.size())) / number_select[i]);
		}
	}

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) {
		std::vector<unsigned int> parameterList;

		unsigned int pick = getParameter();
		for(unsigned int i = 0; i < number_of_nodes; i++) parameterList.push_back(pick);

		return parameterList;
	}

	unsigned int getParameter() {
		for(unsigned int i = 0; i < this->_number_of_parameters; i++) {
			if(Q[i] == 0) return i;
		}

		if(init_each_parameter < this->_number_of_parameters)
			return init_each_parameter++;
		else
			return std::distance(Q.get(),
								 std::max_element(Q.get(), Q.get() + this->_number_of_parameters));
	}

	std::string className() const { return "PsUCBW"; }

  protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const double& _C;
	const unsigned int& _windowSize;
	const char* _aggregation_function;
	circular_buffer<std::pair<double, unsigned int>> sliding_window;

	std::uniform_int_distribution<unsigned int>* uid;
	unsigned int init_each_parameter;
	std::unique_ptr<double[]> reward_aggregation;
	std::unique_ptr<unsigned int[]> number_select;
	std::unique_ptr<double[]> Q;
};

#endif