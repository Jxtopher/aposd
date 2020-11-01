#ifndef PSRANDOM_H
#define PSRANDOM_H

#include "parameterSelection.hpp"
#include <random>

class PsRandom : public ParameterSelection {
  public:
    PsRandom(std::shared_ptr<std::mt19937> mt_rand, unsigned int number_of_parameters,
             const char* aggregation_function = AggregationFunction::MEAN,
             const char* heterogeneity_policy = HeterogeneityPolicy::HETEROGENOUS)
        : ParameterSelection(number_of_parameters), _mt_rand(mt_rand),
          _heterogeneity_policy(heterogeneity_policy), _law(0) {
        uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
    }

    PsRandom(const PsRandom& c)
        : ParameterSelection(c._number_of_parameters), _mt_rand(c._mt_rand),
          _heterogeneity_policy(c._heterogeneity_policy), _law(c._law) {
        uid = new std::uniform_int_distribution<unsigned int>(0, this->_number_of_parameters - 1);
    }

    virtual ~PsRandom() {
    }

    ParameterSelection* clone() const {
        return new PsRandom(*this);
    }

    void reset() {
    }

    void update(std::vector<std::pair<double, unsigned int>>& rewards) {
    }

    void update(std::pair<double, unsigned int>& rewards) {
    }

    std::vector<unsigned int> getParameter(const unsigned int number_of_nodes) {
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

    unsigned int getParameter() {
        return uid->operator()(*(this->_mt_rand));
    }

    std::string className() const {
        return "PsRandom";
    }

  protected:
    std::shared_ptr<std::mt19937> _mt_rand;
    const char* _heterogeneity_policy;
    unsigned int _law;
    std::uniform_int_distribution<unsigned int>* uid;
};

#endif
