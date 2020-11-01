///
/// @file stoppingCriteria.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-03
/// @brief
///

#ifndef STOPPINGCRITERIA_H
#define STOPPINGCRITERIA_H

#include <iostream>
#include <list>
#include <memory>
#include <vector>

#include "criteria.hpp"

template<typename SOL, typename TYPE_FITNESS>
class StoppingCriteria : private std::list<Criteria<SOL, TYPE_FITNESS>*> {
  public:
    virtual ~StoppingCriteria() {
        for(auto criteria : *this) delete criteria;
        this->clear();
    }

    bool operator()(const SOL& s) {
        bool total = true;
        for(auto criteria : *this) total = total & criteria->operator()(s);
        return total;
    }

    ///
    /// @brief add a criterion to the list
    ///
    void addCriteria(Criteria<SOL, TYPE_FITNESS>* criteria) {
        this->push_back(criteria);
    }

    void reset() {
        for(auto criteria : *this) this->reset();
    }
};

#endif