///
/// \file psUCBW.h
/// \author Jxtopher
/// \version 1
/// \date 2019-05
/// \brief Implementation UCB with sliding windows
///		   see : Auer - 2002 - Finite-time Analysis of the Multiarmed Bandit Problem
///        see : DaCosta et al. - 2008 - Adaptive Operator Selection with Dynamic Multi-Armed Bandits
///

#ifndef PSUCBW_H
#define	PSUCBW_H

#include <random>

#include <boost/circular_buffer.hpp>

#include "parameterSelection.h"

using namespace std;
using namespace boost;

class PsUCBW : public ParameterSelection {
	public:
	PsUCBW(std::mt19937 &mt_rand,
		unsigned int nbParameter,
        const double C = 0.03,
        const unsigned int windowSize = 300,
		AggregationFunction aggregationFunction = AggregationFunction::MEAN) :
		ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
        _C(C),
        _windowSize(windowSize),
		_aggregationFunction(aggregationFunction) {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
			slidingWindow.set_capacity(windowSize);
			rewardAggregation = unique_ptr<double []>(new double[nbParameter]);
			numberSelect = unique_ptr<unsigned int []>(new unsigned int[nbParameter]);
            Q = unique_ptr<double []>(new double[nbParameter]);
	}

	PsUCBW(const PsUCBW &c) : 
		ParameterSelection(c._nbParameter),
		_mt_rand(c._mt_rand),
		_C(c._C),
		_windowSize(c._windowSize),
		_aggregationFunction(c._aggregationFunction)  {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
			slidingWindow.set_capacity(_windowSize);
			rewardAggregation = unique_ptr<double []>(new double[_nbParameter]);
			numberSelect = unique_ptr<unsigned int []>(new unsigned int[_nbParameter]);
            Q = unique_ptr<double []>(new double[_nbParameter]);
			
			slidingWindow = c.slidingWindow;
			for (unsigned int i = 0 ; i < _nbParameter ; i++) {
				rewardAggregation[i] = c.rewardAggregation[i];
				numberSelect[i] = c.numberSelect[i];
				Q[i] = c.Q[i];
			}
    }

	virtual ~PsUCBW() {

	}

	ParameterSelection* clone() const { return new PsUCBW(*this); }

	void reset() {
		initEachParameter = 0;
		for(unsigned int i = 0 ; i < this->_nbParameter ; i++) {
			rewardAggregation[i] = 0;
			numberSelect[i] = 0;
            Q[i] = 0;
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
        // Update rewardAggregation
		switch (_aggregationFunction) {
			case AggregationFunction::MAX:
				throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] function is not defined");
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
				throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__)  + " [-] The aggregation function is not defined");
				break;
		}

        // Update Q
        for (unsigned int i = 0 ; i < this->_nbParameter ; i++) {
            if (numberSelect[i] != 0) {
                Q[i] = rewardAggregation[i] + _C * sqrt((2 * log(slidingWindow.size())) / numberSelect[i]);
            }
        }
	}

	vector<unsigned int> getParameter(const unsigned int nbNodes) {
		vector<unsigned int> parameterList;

        unsigned int pick = getParameter();
        for (unsigned int i = 0 ; i < nbNodes ; i++) {
            parameterList.push_back(pick);
        }

		return parameterList;
	}

	unsigned int getParameter() {
        for (unsigned int i = 0 ; i < this->_nbParameter ; i++) {
            if(Q[i] == 0)
                return i;
        }
        if (initEachParameter < this->_nbParameter) {
			return initEachParameter++;
		} else {
			return distance(Q.get(), max_element(Q.get(), Q.get() + this->_nbParameter));
		}
	}

	protected:
	std::mt19937 &_mt_rand;
    const double &_C;
    const unsigned int &_windowSize;
	const AggregationFunction _aggregationFunction;
    circular_buffer<pair<double, unsigned int>> slidingWindow;

	uniform_int_distribution<unsigned int> *uid;
    unsigned int initEachParameter;
    unique_ptr<double[]> rewardAggregation;
	unique_ptr<unsigned int[]> numberSelect;
    unique_ptr<double[]> Q;
};

#endif