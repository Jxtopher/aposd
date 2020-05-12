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
#ifndef PSEPSILONGREEDYW_H
#define PSEPSILONGREEDYW_H

#include <algorithm> // std::min_element, std::max_element
#include <iterator>  // distance
#include <memory>
#include <random>

#include <boost/circular_buffer.hpp>

#include "parameterSelection.hpp"

class PsEspsilonGreedy : public ParameterSelection {
  public:
	PsEspsilonGreedy(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
					 const double espilon = 0.01, const unsigned int windowSize = 150,
					 const char* aggregation_function = AggregationFunction::MEAN,
					 const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS);

	PsEspsilonGreedy(const PsEspsilonGreedy& c);

	virtual ~PsEspsilonGreedy();

	ParameterSelection* clone() const;

	void reset();

	void update(std::vector<std::pair<double, unsigned int>>& rewards);
	///
	/// @brief update the reward aggregation for a parameter
	///
	/// @param rewards : rewards is a pair of raward and parameter
	///
	void update(std::pair<double, unsigned int>& rewards);
	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	double getreward(const unsigned int parameter) const;

	std::string className() const;

  protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const double _espilon;
	const unsigned int _windowSize;
	const char* _aggregation_function;
	const char* _heterogeneity_policy;
	std::unique_ptr<std::uniform_real_distribution<>> urd;
	std::unique_ptr<std::uniform_int_distribution<unsigned int>> uid;
	boost::circular_buffer<std::pair<double, unsigned int>> sliding_window;

	unsigned int init_each_parameter;
	std::unique_ptr<double[]> reward_aggregation;
	std::unique_ptr<unsigned int[]> number_select;
};

#endif
