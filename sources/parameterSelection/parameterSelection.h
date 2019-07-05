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
        static const int ADAPTIVEPURSUIT = 0;
        static const int CONSTANT = 1;
        static const int EPSILONGREEDY = 2;
        static const int RANDOM = 3;
        static const int SELECTBESTMUTATE = 4;
        static const int UCBW = 5;

		ParameterSelection(unsigned int nbParameter) : 
			_nbParameter(nbParameter) {

		}

		virtual ~ParameterSelection() {
			
		}

		virtual void reset() = 0;
		virtual void update(vector<pair<double, unsigned int>> &rewards) = 0;
		virtual void update(pair<double, unsigned int> &rewards) = 0;
		void update(const double &reward, const unsigned int &parameter) {
			pair<double, unsigned int> p(reward, parameter);
			update(p);
		}
		virtual vector<unsigned int> getParameter(const unsigned int nbNodes) = 0;
		virtual unsigned int getParameter() = 0;

	protected:
		const unsigned int _nbParameter;
};

#endif
