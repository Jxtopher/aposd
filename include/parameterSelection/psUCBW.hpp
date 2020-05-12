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
		   const char* aggregation_function = AggregationFunction::MEAN);

	PsUCBW(const PsUCBW& c);

	virtual ~PsUCBW();

	ParameterSelection* clone() const;

	void reset();

	void update(std::vector<std::pair<double, unsigned int>>& rewards);

	///
	/// @brief update the reward aggregation for a parameter
	///
	/// @param rewards is a pair of raward and parameter
	///
	void update(std::pair<double, unsigned int>& rewards);

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	std::string className() const;

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