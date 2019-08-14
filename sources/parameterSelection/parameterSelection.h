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
        static constexpr const char* ADAPTIVEPURSUIT = "PsAdaptivePursuit";
        static constexpr const char* CONSTANT = "PsConstant";
        static constexpr const char* EPSILONGREEDY = "PsEspsilonGreedy";
        static constexpr const char* RANDOM = "PsRandom";
        static constexpr const char* SELECTBESTMUTATE = "PsSelectBestMutate";
        static constexpr const char* UCBW = "PsUCBW";

		ParameterSelection(unsigned int nbParameter) : 
			_nbParameter(nbParameter) {
		}

		virtual ~ParameterSelection() {
			
		}

		virtual ParameterSelection* clone() const = 0;
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
