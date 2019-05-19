#ifndef PSRANDOM_H
#define	PSRANDOM_H

#include <random>
#include "parameterSelection.h"

class PsRandom : public ParameterSelection {
	public:
	PsRandom(std::mt19937 &mt_rand,
		unsigned int nbParameter,
		AggregationFunction aggregationFunction = AggregationFunction::MEAN,
		HeterogeneityPolicy heterogeneityPolicy = HeterogeneityPolicy::HETEROGENOUS) :
		ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
		_heterogeneityPolicy(heterogeneityPolicy),
		_law(0) {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
	}

	virtual ~PsRandom() {

	}

	void reset() {

	}

	void update(vector<pair<double, unsigned int>> &rewards) {

	}

	void update(pair<double, unsigned int> &rewards) {

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
	const HeterogeneityPolicy _heterogeneityPolicy;
	unsigned int _law;
	uniform_int_distribution<unsigned int> *uid;
};

#endif
