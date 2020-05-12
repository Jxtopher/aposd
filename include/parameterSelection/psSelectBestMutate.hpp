///
/// @file psSelectBestMutate.h
/// @author Jxtopher
/// @version 1
/// @date 2019-05
/// @brief Implementation Select best mutate
///		   see : Derbel et Verel - 2011 - DAMS distributed adaptive metaheuristic selection
///

#ifndef PSSELECTBESTMUTATE_H
#define PSSELECTBESTMUTATE_H


#include <cassert>
#include <memory>
#include <random>
#include <algorithm>

#include <boost/circular_buffer.hpp>

#include "parameterSelection.hpp"

class PsSelectBestMutate : public ParameterSelection {
  public:
	PsSelectBestMutate(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
					   const double espilon = 0.15, const unsigned int windowSize = 150,
					   const char* aggregation_function = AggregationFunction::MEAN,
					   const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS);

	PsSelectBestMutate(const PsSelectBestMutate& c);

	virtual ~PsSelectBestMutate();

	ParameterSelection* clone() const;

	void reset();

	void update(std::vector<std::pair<double, unsigned int>>& rewards);

	void update(std::pair<double, unsigned int>& rewards);

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	std::string className() const;

  protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const double _espilon;
	const unsigned int _windowSize;
	boost::circular_buffer<std::pair<double, unsigned int>> sliding_window;
	const char* _aggregation_function;
	const char* _heterogeneity_policy;
	std::unique_ptr<std::uniform_real_distribution<>> urd;
	std::uniform_int_distribution<unsigned int>* uid;

	unsigned int init_each_parameter;
	std::unique_ptr<double[]> reward_aggregation;
	std::unique_ptr<unsigned int[]> number_select;
};

#endif
