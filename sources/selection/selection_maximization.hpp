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

#include "solutionSelection.hpp"

template<class SOL> class Selection_maximization : public SolutionSelection<SOL> {
  public:
	Selection_maximization() {}

	virtual ~Selection_maximization() {}

	SOL operator()(const std::vector<SOL>& solutions) const {
		assert(0 < solutions.size());

		double fitness = solutions[0].getFitness();
		int index = 0;
		for(unsigned int i = 0; i < solutions.size(); i++) {
			if(fitness < solutions[i].getFitness()) {
				fitness = solutions[i].getFitness();
				index = i;
			}
		}
		return solutions[index];
	}

	bool operator()(const SOL& s1, const SOL& s2) const {
		assert(s1.fitnessIsValid());
		assert(s2.fitnessIsValid());

		return s2.getFitness() < s1.getFitness();
	}

	bool operator()(const SOL& s1, const SOL& s2, const unsigned int numObjectif) const {
		assert(s1.fitnessIsValid(numObjectif));
		assert(s2.fitnessIsValid(numObjectif));

		return s2.getFitness(numObjectif) < s1.getFitness(numObjectif);
	}
};

#endif