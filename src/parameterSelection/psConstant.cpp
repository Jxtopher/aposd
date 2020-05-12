#include "parameterSelection/psConstant.hpp"
PsConstant::PsConstant(unsigned int number_of_parameters, unsigned int parameterUsed)
	: ParameterSelection(number_of_parameters), _parameterUsed(parameterUsed) {}

PsConstant::~PsConstant() {}

PsConstant::PsConstant(const PsConstant& c)
	: ParameterSelection(c._number_of_parameters), _parameterUsed(c._parameterUsed) {}

ParameterSelection* PsConstant::clone() const { return new PsConstant(*this); }

void PsConstant::reset() {}

void PsConstant::update(std::vector<std::pair<double, unsigned int>>& rewards) {}

void PsConstant::update(std::pair<double, unsigned int>& rewards) {}

std::vector<unsigned int> PsConstant::getParameter(const unsigned int number_of_nodes) {
	std::vector<unsigned int> parameterList;

	for(unsigned int i = 0; i < number_of_nodes; i++) parameterList.push_back(_parameterUsed);

	return parameterList;
}

unsigned int PsConstant::getParameter() { return _parameterUsed; }

std::string PsConstant::className() const { return "PsConstant"; }