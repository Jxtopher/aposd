///
/// @file selection_minimization.hpp
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#include "selection/selection_minimization.hpp"

template<class SOL>
Selection_minimization<SOL>::Selection_minimization() {}

template<class SOL>
Selection_minimization<SOL>::~Selection_minimization() {}

template<class SOL>
SOL Selection_minimization<SOL>::operator()(const std::vector<SOL>& solutions) const {
	assert(0 < solutions.size());

	double fitness = solutions[0].getFitness();
	int index = 0;
	for(unsigned int i = 0; i < solutions.size(); i++) {
		if(solutions[i].getFitness() < fitness) {
			fitness = solutions[i].getFitness();
			index = i;
		}
	}
	return solutions[index];
}

template<class SOL>
bool Selection_minimization<SOL>::operator()(const SOL& s1, const SOL& s2) const {
	assert(s1.fitnessIsValid());
	assert(s2.fitnessIsValid());

	return s1.getFitness() < s2.getFitness();
}

template<class SOL>
bool Selection_minimization<SOL>::operator()(const SOL& s1, const SOL& s2, const unsigned int numObjectif) const {
	assert(s1.fitnessIsValid(numObjectif));
	assert(s2.fitnessIsValid(numObjectif));

	return s1.getFitness(numObjectif) < s2.getFitness(numObjectif);
}