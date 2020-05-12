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

template<typename SOL, typename TYPE_FITNESS> class StoppingCriteria {
  public:
	StoppingCriteria();

	virtual ~StoppingCriteria();
	bool operator()(const SOL& s);
	void addCriteria(Criteria<SOL, TYPE_FITNESS>* c);

  protected:
	std::vector<Criteria<SOL, TYPE_FITNESS>*> criteria;
};

#endif