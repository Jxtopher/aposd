///
/// @file criteriaBudget.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef CRITERIABUDGET_H
#define CRITERIABUDGET_H

#include "criteria.hpp"

template<typename SOL, typename TYPE_FITNESS>
class CriteriaBudget : public Criteria<SOL, TYPE_FITNESS> {
  public:
	CriteriaBudget(unsigned int _budget);

	virtual ~CriteriaBudget();

	virtual bool operator()(const SOL& s);

	unsigned int getConsumedBudget() const;

	void resetConsumedBudget();

  protected:
	unsigned int consumedBudget;
	unsigned int budget;
};

#endif