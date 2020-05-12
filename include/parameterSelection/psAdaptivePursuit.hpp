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
					  const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS);

	PsAdaptivePursuit(const PsAdaptivePursuit& c);

	virtual ~PsAdaptivePursuit();

	template<typename T>
	static unsigned int roulette_wheel(const T* array, unsigned int size, std::mt19937& mt_rand);

	ParameterSelection* clone() const;

	void reset();
	///
	/// @brief Collective version
	///
	void update(std::vector<std::pair<double, unsigned int>>& rewards);
	///
	/// @brief Individual version
	///
	/// @param rewards : give a pair<reward, paramter>
	///
	void update(std::pair<double, unsigned int>& rewards);

	void _selection_probability();

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	std::string className() const;

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