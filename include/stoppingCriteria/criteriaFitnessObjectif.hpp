///
/// @file criteriaFitnessObjectif.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef CRITERIAFITNESSOBJECTIF_H
#define CRITERIAFITNESSOBJECTIF_H

#include "criteria.hpp"
#include <cassert>

template<typename SOL, typename TYPE_FITNESS>
class CriteriaFitnessObjectif : public Criteria<SOL, TYPE_FITNESS> {
  public:
	CriteriaFitnessObjectif(TYPE_FITNESS fitnessObjectif);

	CriteriaFitnessObjectif(TYPE_FITNESS* fitnessObjectif, unsigned int numberOfObjective);

	virtual ~CriteriaFitnessObjectif();

	virtual bool operator()(const SOL& s);

  protected:
	TYPE_FITNESS* _fitnessObjectif;
	unsigned int _numberOfObjective;
};

#endif