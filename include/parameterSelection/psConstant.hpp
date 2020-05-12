#ifndef PSCONSTANT_H
#define PSCONSTANT_H

#include "parameterSelection.hpp"

class PsConstant : public ParameterSelection {
  public:
	PsConstant(unsigned int number_of_parameters, unsigned int parameterUsed);

	virtual ~PsConstant();

	PsConstant(const PsConstant& c);

	ParameterSelection* clone() const;

	void reset();

	void update(std::vector<std::pair<double, unsigned int>>& rewards);

	void update(std::pair<double, unsigned int>& rewards);

	std::vector<unsigned int> getParameter(const unsigned int number_of_nodes);

	unsigned int getParameter();

	std::string className() const;

  protected:
	unsigned int _parameterUsed;
};

#endif
