#ifndef PSRANDOM_H
#define PSRANDOM_H

#include <random>
#include <memory>

#include "parameterSelection.hpp"

class PsRandom : public ParameterSelection {
  public:
	PsRandom(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
			 const char* aggregation_function = AggregationFunction::MEAN,
			 const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS);

	PsRandom(const PsRandom& c);

	virtual ~PsRandom();

	ParameterSelection* clone() const;

	void reset();

	void update(std::vector<std::pair<double, unsigned int>>& rewards);

	void update(std::pair<double, unsigned int>& rewards);

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	std::string className() const;

  protected:
	std::shared_ptr<std::mt19937> _mt_rand;
	const char* _heterogeneity_policy;
	unsigned int _law;
	std::uniform_int_distribution<unsigned int>* uid;
};

#endif
