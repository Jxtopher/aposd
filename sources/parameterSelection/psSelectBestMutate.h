///
/// \file psSelectBestMutate.h
/// \author Jxtopher
/// \version 1
/// \date 2019-05
/// \brief Implementation Select best mutate
///		   see : Derbel et Verel - 2011 - DAMS distributed adaptive metaheuristic selection
///

#ifndef PSSELECTBESTMUTATE_H
#define	PSSELECTBESTMUTATE_H

#include <random>
#include "parameterSelection.h"

class PsSelectBestMutate : public ParameterSelection {
	public:
	PsSelectBestMutate(std::mt19937 &mt_rand,
		unsigned int nbParameter,
		const double espilon = 0.15,
		AggregationFunction aggregationFunction = AggregationFunction::MEAN,
		HeterogeneityPolicy heterogeneityPolicy = HeterogeneityPolicy::HETEROGENOUS) :
		ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
		_espilon(espilon),
		_windowSize(0),
		_aggregationFunction(aggregationFunction),
		_heterogeneityPolicy(heterogeneityPolicy) {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
	}

	PsSelectBestMutate(std::mt19937 &mt_rand,
		unsigned int nbParameter,
		const double espilon = 0.15,
		const unsigned int windowSize = 150,
		AggregationFunction aggregationFunction = AggregationFunction::MEAN,
		HeterogeneityPolicy heterogeneityPolicy = HeterogeneityPolicy::HETEROGENOUS) :
		ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
		_espilon(espilon),
		_windowSize(windowSize),
		_aggregationFunction(aggregationFunction),
		_heterogeneityPolicy(heterogeneityPolicy) {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
	}

	virtual ~PsSelectBestMutate() {

	}

	void reset() {

	}

	void update(vector<pair<double, unsigned int>> &rewards) {
		assert(_windowSize == 0);
		switch (_aggregationFunction)
		{
			case AggregationFunction::MAX:
/*
 const auto p = std::minmax_element(pairs.begin(), pairs.end());
 auto min = p.first->first;
 auto max = p.second->first;
 */
				break;
			case AggregationFunction::MEAN:

				break;
			default:
				assert(false);
				break;
		}
	}

	void update(pair<double, unsigned int> &rewards) {
		assert(_windowSize != 0);
		// Need implementation
		
		switch (_aggregationFunction)
		{
			case AggregationFunction::MAX:
/*
 const auto p = std::minmax_element(pairs.begin(), pairs.end());
 auto min = p.first->first;
 auto max = p.second->first;
 */
				break;
			case AggregationFunction::MEAN:

				break;
			default:
				assert(false);
				break;
		}
	}

	vector<unsigned int> getParameter(const unsigned int nbNodes) {
		vector<unsigned int> parameterList;
		
		switch (_heterogeneityPolicy) {
			case HeterogeneityPolicy::HETEROGENOUS:
				for (unsigned int i = 0 ; i < nbNodes ; i++)
					parameterList.push_back(uid->operator()(this->_mt_rand));
				break;
			case HeterogeneityPolicy::HOMOGENEOUS:
				{
				unsigned int pick = uid->operator()(this->_mt_rand);
				for (unsigned int i = 0 ; i < nbNodes ; i++)
					parameterList.push_back(pick);
				}
				break;
			default:
				assert(false);
				break;
		}

		return parameterList;
	}

	unsigned int getParameter() {
		return uid->operator()(this->_mt_rand);
	}

	protected:
	std::mt19937 &_mt_rand;
	const double &_espilon;
	const unsigned int _windowSize;
	const AggregationFunction _aggregationFunction;
	const HeterogeneityPolicy _heterogeneityPolicy;
	uniform_int_distribution<unsigned int> *uid;
};

#endif
