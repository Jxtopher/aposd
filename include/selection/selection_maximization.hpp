///
/// @file selection_maximization.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef SELECTION_MAXIMIZATION_H
#define SELECTION_MAXIMIZATION_H

#include <vector>

#include "solutionSelection.hpp"

template<class SOL> class Selection_maximization : public SolutionSelection<SOL> {
  public:
	Selection_maximization();

	virtual ~Selection_maximization();

	SOL operator()(const std::vector<SOL>& solutions) const;

	bool operator()(const SOL& s1, const SOL& s2) const;

	bool operator()(const SOL& s1, const SOL& s2, const unsigned int numObjectif) const;
};

#endif