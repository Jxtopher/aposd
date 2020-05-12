///
/// @file criteriaFitnessObjectif.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#include "stoppingCriteria/criteriaFitnessObjectif.hpp"

template<typename SOL, typename TYPE_FITNESS>
CriteriaFitnessObjectif<SOL, TYPE_FITNESS>::CriteriaFitnessObjectif(TYPE_FITNESS fitnessObjectif) : Criteria<SOL, TYPE_FITNESS>() {
	_numberOfObjective = 1;
	_fitnessObjectif = new TYPE_FITNESS[_numberOfObjective];
	_fitnessObjectif[0] = fitnessObjectif;
}

template<typename SOL, typename TYPE_FITNESS>
CriteriaFitnessObjectif<SOL, TYPE_FITNESS>::CriteriaFitnessObjectif(TYPE_FITNESS* fitnessObjectif, unsigned int numberOfObjective)
	: Criteria<SOL, TYPE_FITNESS>() {
	_numberOfObjective = numberOfObjective;
	_fitnessObjectif = new TYPE_FITNESS[_numberOfObjective];
	for(unsigned int i = 0; i < numberOfObjective; i++) {
		_fitnessObjectif[0] = fitnessObjectif;
	}
}

template<typename SOL, typename TYPE_FITNESS>
CriteriaFitnessObjectif<SOL, TYPE_FITNESS>::~CriteriaFitnessObjectif() { delete[] _fitnessObjectif; }

template<typename SOL, typename TYPE_FITNESS>
bool CriteriaFitnessObjectif<SOL, TYPE_FITNESS>::operator()(const SOL& s) {
	assert(s.numberOfObjective() == _numberOfObjective);
	// Besion de la définir pour le multi-objectifs
	if(s.fitnessIsValid()) {
		if((_fitnessObjectif[0] - s.getFitness()) == 0) {
			return false;
		} else {
			return true;
		}
	} else {
		return true;
	}
}