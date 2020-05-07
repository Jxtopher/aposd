#ifndef SELECTIONSTRATEGIE_H
#define SELECTIONSTRATEGIE_H

#include <string>
#include <vector>

class AggregationFunction {
  public:
	static constexpr const char* MAX = "max";
	static constexpr const char* MEAN = "mean";
};

class HeterogeneityPolicy {
  public:
	static constexpr const char* NONE = "none";
	static constexpr const char* HOMOGENEOUS = "homogeneous";
	static constexpr const char* HETEROGENOUS = "heterogenous";
};

class ParameterSelection {
  public:
	static constexpr const char* ADAPTIVEPURSUIT = "PsAdaptivePursuit";
	static constexpr const char* CONSTANT = "PsConstant";
	static constexpr const char* EPSILONGREEDY = "PsEspsilonGreedy";
	static constexpr const char* RANDOM = "PsRandom";
	static constexpr const char* SELECTBESTMUTATE = "PsSelectBestMutate";
	static constexpr const char* UCBW = "PsUCBW";

	ParameterSelection(unsigned int number_of_parameters)
		: _number_of_parameters(number_of_parameters) {}

	virtual ~ParameterSelection() {}

	virtual ParameterSelection* clone() const = 0;
	virtual void reset() = 0;
	virtual void update(std::vector<std::pair<double, unsigned int>>& rewards) = 0;
	virtual void update(std::pair<double, unsigned int>& rewards) = 0;
	void update(const double& reward, const unsigned int& parameter) {
		std::pair<double, unsigned int> p(reward, parameter);
		update(p);
	}
	virtual std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) = 0;
	virtual unsigned int getParameter() = 0;
	virtual std::string className() const = 0;

  protected:
	const unsigned int _number_of_parameters;
};

#endif
