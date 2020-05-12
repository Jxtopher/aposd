///
/// @file selection_minimization.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#include <vector>

#include "solutionSelection.hpp"

#ifndef SELECTION_MINIMIZATION_H
#define SELECTION_MINIMIZATION_H

template<class SOL> class Selection_minimization : public SolutionSelection<SOL> {
  public:
	Selection_minimization();

	virtual ~Selection_minimization();

	SOL operator()(const std::vector<SOL>& solutions) const;
	bool operator()(const SOL& s1, const SOL& s2) const;

	bool operator()(const SOL& s1, const SOL& s2, const unsigned int numObjectif) const;
};

#endif