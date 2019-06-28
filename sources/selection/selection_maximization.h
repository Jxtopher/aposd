///
/// \file solutionSelection_best.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief
///

#ifndef SELECTION_MAXIMIZATION_H
#define SELECTION_MAXIMIZATION_H

#include <vector>

#include "Selection.h"

using namespace std;

template <class SOL>
class Selection_maximization : public Selection<SOL> {
    public:
    Selection_maximization() : Selection<SOL>() {

    }

    virtual ~Selection_maximization() {

    }
   
    SOL operator()(const vector<SOL> &solutions) const {
        assert(0 < solutions.size());
        
        double fitness = solutions[0].getFitness();
        int index = 0;
        for (unsigned int i = 0 ; i < solutions.size() ; i++) {
            if (fitness < solutions[i].getFitness()) {
                fitness = solutions[i].getFitness();
                index = i;
            }
        }
        return solutions[index];
    }

    protected:

};

#endif
