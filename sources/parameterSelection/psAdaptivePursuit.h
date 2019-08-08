///
/// \file psAdaptivePursuit.h
/// \author Jxtopher
/// \version 1
/// \date 2019-05
/// \brief Implementation adaptive pursuit strategy
///		   see : Thierens - 2005 - An adaptive pursuit strategy for allocating operator Probabilities
///


#ifndef PSADAPTIVEPURSUIT_H
#define	PSADAPTIVEPURSUIT_H

#include <random>
#include <memory>
#include "parameterSelection.h"

class PsAdaptivePursuit : public ParameterSelection {
	public:
	PsAdaptivePursuit(std::mt19937 &mt_rand,
		unsigned int nbParameter,
        const double alpha = 0.2,
        const double beta = 0.2,
        const double p_min = 0.1,
        const double p_max = 0.9,
        AggregationFunction aggregationFunction = AggregationFunction::MEAN,
		HeterogeneityPolicy heterogeneityPolicy = HeterogeneityPolicy::HETEROGENOUS) :
        ParameterSelection(nbParameter),
		_mt_rand(mt_rand),
        _alpha(alpha),
        _beta(beta),
        _p_min(p_min),
        _p_max(p_max),
        _aggregationFunction(aggregationFunction),
		_heterogeneityPolicy(heterogeneityPolicy) {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
            rewardEstimate = unique_ptr<double []>(new double[nbParameter]);
           selectionProbability = unique_ptr<double []>(new double[nbParameter]);
    }
	
	PsAdaptivePursuit(const PsAdaptivePursuit &c) : 
		ParameterSelection(c._nbParameter),
		_mt_rand(c._mt_rand),
		_alpha(c._alpha),
		_beta(c._beta),
		_p_min(c._p_min),
		_p_max(c._p_max),
        _aggregationFunction(c._aggregationFunction),
		_heterogeneityPolicy(c._heterogeneityPolicy)  {
			uid = new uniform_int_distribution<unsigned int>(0, this->_nbParameter -1);
            rewardEstimate = unique_ptr<double []>(new double[_nbParameter]);
           	selectionProbability = unique_ptr<double []>(new double[_nbParameter]);
			
			for (unsigned int i = 0 ; i < _nbParameter ; i++) {
				rewardEstimate[i] = c.rewardEstimate[i]; 
				selectionProbability[i] = c.selectionProbability[i];
			}
    }

	virtual ~PsAdaptivePursuit() {

	}

	ParameterSelection* clone() const { return new PsAdaptivePursuit(*this); }

	void reset() {

	}

    ///
    /// \brief Collective version
    ///
	void update(vector<pair<double, unsigned int>> &rewards) {

	}

    ///
    /// \brief Individual version
    ///
	void update(pair<double, unsigned int> &rewards) {
        // rewards.first =  reward (double)
        // rewards.second =  parameter (unsigned int)
        rewardEstimate[rewards.second] = (1 - _alpha) * rewardEstimate[rewards.second] + _alpha * rewards.first;   
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
	const double _alpha;   // adaptation rate
	const double _beta;    // learning rate
    const double _p_min;
	const double _p_max;
	
    const AggregationFunction _aggregationFunction;
	const HeterogeneityPolicy _heterogeneityPolicy;

    unsigned int initEachParameter;
	uniform_int_distribution<unsigned int> *uid;
    unique_ptr<double []> rewardEstimate;
    unique_ptr<double []> selectionProbability;
};

#endif



/*
	void updateParam(unsigned int op, double rewards) {
		rewardEstimate[op] = (1 - adaptationRate) * rewardEstimate[op] + adaptationRate * rewards;
	}

	void _selectionProbability() {
		double rewardEstimate_max = rewardEstimate[0];

		for (unsigned int i = 1; i < nbOp; i++) {
			if (rewardEstimate_max < rewardEstimate[i])
				rewardEstimate_max = rewardEstimate[i];
		}

		for (unsigned int i = 0; i < nbOp; i++) {
			if (rewardEstimate_max == rewardEstimate[i]) {
				selectionProbability[i] = selectionProbability[i] + learningRate * (p_max - selectionProbability[i]);
			} else {
				selectionProbability[i] = selectionProbability[i] + learningRate * (p_min - selectionProbability[i]);
			}
		}
	}

	void init() {
		if (initOperator < nbOp) {
			for (unsigned int i = 0; i < this->nodeArm.size(); i++) {
				this->nodeArm[i] = initOperator;
			}
			initOperator++;
		} else {
			// Mise à jour du reward
			for (unsigned int i = 0; i < this->rewards.size(); i++) {
				updateParam(this->rewards[i].op(), this->rewards[i].reward());
			}

			// Calcule de la probabilité de selection
			_selectionProbability();

			unsigned int tirage = rng.roulette_wheel<double>(selectionProbability);
			for (unsigned int i = 0; i < this->nodeArm.size(); i++) {
				this->nodeArm[i] = tirage;
			}
		}

	}

	unsigned int run(unsigned int num_node) {
		// Choix du l'operateur en fonction de la probabilité de selection et de la roulette
		return this->nodeArm[num_node];
	}
*/