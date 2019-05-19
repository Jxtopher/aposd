#ifndef SELECTIONSTRATEGIE_H
#define	SELECTIONSTRATEGIE_H

#include <vector>

using namespace std;


enum class AggregationFunction {
	MAX,
	MEAN
};

enum class HeterogeneityPolicy {
	NONE,
	HOMOGENEOUS,
	HETEROGENOUS
};

class ParameterSelection {
	public:
		ParameterSelection(unsigned int nbParameter) : 
			_nbParameter(nbParameter) {

		}

		virtual ~ParameterSelection() {
			
		}

		virtual void reset() = 0;
		virtual void update(vector<pair<double, unsigned int>> &rewards) = 0;
		virtual void update(pair<double, unsigned int> &rewards) = 0;
		virtual vector<unsigned int> getParameter(const unsigned int nbNodes) = 0;
		virtual unsigned int getParameter() = 0;

	protected:
		const unsigned int &_nbParameter;
};

#endif
