///
/// \file criteriaFitnessObjectif.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef CRITERIAFITNESSOBJECTIF_H
#define CRITERIAFITNESSOBJECTIF_H

#include <cassert>
#include "criteria.h"


template<typename SOL, typename TYPE_FITNESS>
class CriteriaFitnessObjectif : public Criteria<SOL, TYPE_FITNESS> {
    public:
    CriteriaFitnessObjectif(TYPE_FITNESS fitnessObjectif) : 
        Criteria<SOL, TYPE_FITNESS>() {
            _numberOfObjective = 1;
            _fitnessObjectif = new TYPE_FITNESS[_numberOfObjective];
            _fitnessObjectif[0] = fitnessObjectif;
    }

    CriteriaFitnessObjectif(TYPE_FITNESS *fitnessObjectif, unsigned int numberOfObjective) : 
        Criteria<SOL, TYPE_FITNESS>() {
            _numberOfObjective = numberOfObjective;
            _fitnessObjectif = new TYPE_FITNESS[_numberOfObjective];
            for (unsigned int i = 0 ; i < numberOfObjective ; i++) {
                _fitnessObjectif[0] = fitnessObjectif;
            }
    }

    virtual ~CriteriaFitnessObjectif(){
        delete[] _fitnessObjectif;
    }

    virtual bool operator()(const SOL &s) {
        assert(s.numberOfObjective() == _numberOfObjective);
        // Besion de la définir pour le multi-objectifs
        if (s.fitnessIsValid()) {
            if ((_fitnessObjectif[0] - s.getFitness()) == 0) {
                return false;
            } else {
                return true;
            }
        } else {
            return true;
        }
    }

    protected:
    TYPE_FITNESS *_fitnessObjectif;
    unsigned int _numberOfObjective;
};

#endif