///
/// @file stoppingCriteria.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2019-03
/// @brief
///

#include "stoppingCriteria/stoppingCriteria.hpp"

template<typename SOL, typename TYPE_FITNESS>
StoppingCriteria<SOL, TYPE_FITNESS>::StoppingCriteria() {}

template<typename SOL, typename TYPE_FITNESS>
StoppingCriteria<SOL, TYPE_FITNESS>::~StoppingCriteria() {
	for(unsigned int i = 0; i < criteria.size(); i++) delete criteria[i];

	criteria.clear();
}

template<typename SOL, typename TYPE_FITNESS>
bool StoppingCriteria<SOL, TYPE_FITNESS>::operator()(const SOL& s) {
	bool total = 1;

	for(unsigned int i = 0; i < criteria.size(); i++)
		total = total & criteria[i]->operator()(s);
	return total;
}

template<typename SOL, typename TYPE_FITNESS>
void StoppingCriteria<SOL, TYPE_FITNESS>::addCriteria(Criteria<SOL, TYPE_FITNESS>* c) { criteria.push_back(c); }