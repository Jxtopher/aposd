///
/// @file selection_minimization.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief 
///

#include "solutionSelection.hpp"

#ifndef SELECTION_MINIMIZATION_H
#define SELECTION_MINIMIZATION_H

template<class SOL>
class Selection_minimization : public SolutionSelection<SOL> {
    public:
    Selection_minimization()  {

    }

    virtual ~Selection_minimization() {

    }

    SOL operator()(const std::vector<SOL> &solutions) const {
        assert(0 < solutions.size());
        
        double fitness = solutions[0].getFitness();
        int index = 0;
        for (unsigned int i = 0 ; i < solutions.size() ; i++) {
            if (solutions[i].getFitness() < fitness) {
                fitness = solutions[i].getFitness();
                index = i;
            }
        }
        return solutions[index];
    }

    bool operator()(const SOL &s1, const SOL &s2) const {
        assert(s1.fitnessIsValid());
        assert(s2.fitnessIsValid());
        
        return s1.getFitness() < s2.getFitness();
    }

    bool operator()(const SOL &s1, const SOL &s2, const unsigned int numObjectif) const {
        assert(s1.fitnessIsValid(numObjectif));
        assert(s2.fitnessIsValid(numObjectif));
        
        return s1.getFitness(numObjectif) < s2.getFitness(numObjectif);
    }
};

#endif