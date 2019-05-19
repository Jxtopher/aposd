#ifndef PSEPSILONGREEDYW_H
#define	PSEPSILONGREEDYW_H

#include <random>
#include <algorithm>    // std::min_element, std::max_element
#include <iterator>		// distance

#include <boost/circular_buffer.hpp>

#include "parameterSelection.h"

using namespace std;
using namespace boost;


class PsEspsilonGreedy : public ParameterSelection {
	public:
	PsEspsilonGreedy(std::mt19937 &mt_rand,
		unsigned int nbParameter, 
		const double espilon = 0.01,
		const unsigned int windowSize = 150,
		AggregationFunction aggregationFunction = AggregationFunction::MEAN,
		HeterogeneityPolicy heterogeneityPolicy = HeterogeneityPolicy::HETEROGENOUS) :
		ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
		_espilon(espilon),
		_windowSize(windowSize),
		_aggregationFunction(aggregationFunction),
		_heterogeneityPolicy(heterogeneityPolicy) {
			assert(0 <= _espilon && _espilon <= 1);
			
			urd = new uniform_real_distribution<>(0.0, 1.0);
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
			slidingWindow.set_capacity(windowSize);
			rewardAggregation = unique_ptr<double []>(new double[nbParameter]);
			numberSelect = unique_ptr<unsigned int []>(new unsigned int[nbParameter]);
			reset();
	}

	virtual ~PsEspsilonGreedy() {

	}

	void reset() {
		initEachParameter = 0;
		for(unsigned int i = 0 ; i < this->_nbParameter ; i++) {
			rewardAggregation[i] = 0;
			numberSelect[i] = 0;
		}
	}

	void update(vector<pair<double, unsigned int>> &rewards) {
		for(std::vector<pair<double, unsigned int>>::iterator it = rewards.begin(); it != rewards.end(); ++it)
			update(*it);
	}

    ///
    /// \brief update the reward aggregation for a parameter
    ///
    /// \param p : rewards is a pair of raward and parameter
    ///
	void update(pair<double, unsigned int> &rewards) {
		switch (_aggregationFunction)
		{
			case AggregationFunction::MAX:
				assert(false);
				// NEED implementation
				slidingWindow.push_front(rewards);
				{

				}
				break;
			case AggregationFunction::MEAN:
				if (slidingWindow.full()) {
					unsigned int r = slidingWindow.back().first;  // Reward
					unsigned int p = slidingWindow.back().second; // Parameter
					assert(p < this->_nbParameter);
					rewardAggregation[p] = (rewardAggregation[p] * numberSelect[p] - r) / (numberSelect[p] - 1);
					numberSelect[p]--;
				}

				slidingWindow.push_front(rewards);

				{
					unsigned int r = slidingWindow.begin()->first;  // Reward
					unsigned int p = slidingWindow.begin()->second; // Parameter
					assert(p < this->_nbParameter);
					rewardAggregation[p] = (rewardAggregation[p] * numberSelect[p] + r) / (numberSelect[p] + 1);
					numberSelect[p]++;
				}
				break;
			default:
				assert(false);
				break;
		}
	}
	
	vector<unsigned int> getParameter(const unsigned int nbNodes) {
		vector<unsigned int> parameterList;
		
		if (initEachParameter < this->_nbParameter) {
			for (unsigned int i = 0 ; i < nbNodes ; i++)
				parameterList.push_back(initEachParameter);
			initEachParameter++;
		} else {
			unsigned int bestParameter = distance(rewardAggregation.get(), max_element(rewardAggregation.get(), rewardAggregation.get() + this->_nbParameter));
			switch (_heterogeneityPolicy) {
				case HeterogeneityPolicy::HETEROGENOUS:
					for (unsigned int i = 0 ; i < nbNodes ; i++) {
						if (urd->operator()(this->_mt_rand) <= _espilon)
							parameterList.push_back(bestParameter);
						else
							parameterList.push_back(uid->operator()(this->_mt_rand));
					}
					break;
				case HeterogeneityPolicy::HOMOGENEOUS:
					if (urd->operator()(this->_mt_rand) <= _espilon) {
						for (unsigned int i = 0 ; i < nbNodes ; i++)
							parameterList.push_back(bestParameter);
					} else {
						for (unsigned int i = 0 ; i < nbNodes ; i++)
							parameterList.push_back(uid->operator()(this->_mt_rand));
					}
					break;
				default:
					assert(false);
					break;
			}
		}
				
		return parameterList;
	}

	unsigned int getParameter() {
		if (initEachParameter < this->_nbParameter) {
			return initEachParameter++;
		} else if (urd->operator()(this->_mt_rand) <= _espilon) {
			return distance(rewardAggregation.get(), max_element(rewardAggregation.get(), rewardAggregation.get() + this->_nbParameter));
		} else {
			return uid->operator()(this->_mt_rand);
		}
	}


	double getreward(const unsigned int parameter) const {
		assert(parameter < this->_nbParameter);
		return rewardAggregation[parameter];
	}

	protected:
	std::mt19937 &_mt_rand;
	const double &_espilon;
	const unsigned int &_windowSize;
	const AggregationFunction _aggregationFunction;
	const HeterogeneityPolicy _heterogeneityPolicy;
	uniform_real_distribution<> *urd;
	uniform_int_distribution<unsigned int> *uid;
	circular_buffer<pair<double, unsigned int>> slidingWindow;

	unsigned int initEachParameter;
	unique_ptr<double[]> rewardAggregation;
	unique_ptr<unsigned int[]> numberSelect;
};

#endif
