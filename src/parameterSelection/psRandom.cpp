
#include "parameterSelection/psRandom.hpp"

PsRandom::PsRandom(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
			const char* aggregation_function,
			const char* heterogeneity_policy)
	: ParameterSelection(number_of_parameters), _mt_rand(mt_rand),
		_heterogeneity_policy(heterogeneity_policy), _law(0) {
	uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
}

PsRandom::PsRandom(const PsRandom& c)
	: ParameterSelection(c._number_of_parameters), _mt_rand(c._mt_rand),
		_heterogeneity_policy(c._heterogeneity_policy), _law(c._law) {
	uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
}

PsRandom::~PsRandom() {}

ParameterSelection* PsRandom::clone() const { return new PsRandom(*this); }

void PsRandom::reset() {}

void PsRandom::update(std::vector<std::pair<double, unsigned int>>& rewards) {}

void PsRandom::update(std::pair<double, unsigned int>& rewards) {}

std::vector<unsigned int> PsRandom::getParameter(const unsigned int number_of_nodes) {
	std::vector<unsigned int> parameterList;

	if(_heterogeneity_policy == HeterogeneityPolicy::HETEROGENOUS) {
		for(unsigned int i = 0; i < number_of_nodes; i++)
			parameterList.push_back(uid->operator()(*(this->_mt_rand)));
	} else if(_heterogeneity_policy == HeterogeneityPolicy::HOMOGENEOUS) {
		unsigned int pick = uid->operator()(*(this->_mt_rand));
		for(unsigned int i = 0; i < number_of_nodes; i++) parameterList.push_back(pick);
	} else
		throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__) +
									" [-] The policy model is not defined");

	return parameterList;
}

unsigned int PsRandom::getParameter() { return uid->operator()(*(this->_mt_rand)); }

std::string PsRandom::className() const { return "PsRandom"; }