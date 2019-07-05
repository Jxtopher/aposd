///
/// \file criteria.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief 
///

#ifndef CRITERIA_H
#define CRITERIA_H

template<typename SOL, typename TYPE_FITNESS>
class Criteria {
    public:
    Criteria() {

    }

    virtual ~Criteria(){

    }

    virtual bool operator()(const SOL &s) = 0;

    protected:
};

#endif