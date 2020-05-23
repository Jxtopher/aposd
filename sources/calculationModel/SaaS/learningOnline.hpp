#ifndef SAAS_LEARNINGONL_H
#define SAAS_LEARNINGONL_H

#include <utility> // std::pair

template<class SOL>
class LearningOnline {
  public:
	LearningOnline() {
	}

	virtual ~LearningOnline() {
	}

	virtual std::pair<SOL, unsigned int> initial_solution(const SOL& s) = 0;
	virtual std::pair<SOL, unsigned int> run(const SOL& s_t0, const SOL s_t1,
											 unsigned int parameter) = 0;
};

#endif
