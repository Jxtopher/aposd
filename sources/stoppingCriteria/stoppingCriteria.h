///
/// \file stoppingCriteria.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2019-03
/// \brief 
///

#ifndef STOPPINGCRITERIA_H
#define STOPPINGCRITERIA_H

#include <iostream>
#include <list>
#include <memory>

#include "criteria.h"

using namespace std;

template<typename SOL, typename TYPE_FITNESS>
class StoppingCriteria {
    public:
    StoppingCriteria() {

    }

    virtual ~StoppingCriteria() {
        for(unsigned int i = 0 ; i < criteria.size() ; i++)
            delete criteria[i];

        criteria.clear();
    }

    bool operator()(const SOL &s) {
        bool total = 1;

        for(unsigned int i = 0 ; i < criteria.size() ; i++)
            total = total & criteria[i]->operator()(s);
        return total;
    }

    void addCriteria(Criteria<SOL, TYPE_FITNESS> *c) {
        criteria.push_back(c);
    }
    
    
    protected:
    vector<Criteria<SOL, TYPE_FITNESS> *> criteria;
};

#endif