#ifndef PSCONSTANT_H
#define PSCONSTANT_H

#include "parameterSelection.hpp"

class PsConstant : public ParameterSelection {
  public:
	PsConstant(unsigned int number_of_parameters, unsigned int parameterUsed)
		: ParameterSelection(number_of_parameters), _parameterUsed(parameterUsed) {}

	virtual ~PsConstant() {}

	PsConstant(const PsConstant& c)
		: ParameterSelection(c._number_of_parameters), _parameterUsed(c._parameterUsed) {}

	ParameterSelection* clone() const { return new PsConstant(*this); }

	void reset() {}

	void update(std::vector<std::pair<double, unsigned int>>& rewards) {}

	void update(std::pair<double, unsigned int>& rewards) {}

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) {
		std::vector<unsigned int> parameterList;

		for(unsigned int i = 0; i < number_of_nodes; i++) parameterList.push_back(_parameterUsed);

		return parameterList;
	}

	unsigned int getParameter() { return _parameterUsed; }

	std::string className() const { return "PsConstant"; }

  protected:
	unsigned int _parameterUsed;
};

#endif
