#ifndef PSCONSTANT_H
#define	PSCONSTANT_H

#include "parameterSelection.h"

class PsConstant : public ParameterSelection {
	public:
		PsConstant(unsigned int nbParameter, unsigned int parameterUsed) :
			ParameterSelection(nbParameter),
			_parameterUsed(parameterUsed) {

		}

		virtual ~PsConstant() {

		}

	PsConstant(const PsConstant &c) : 
		ParameterSelection(c._nbParameter),
		_parameterUsed(c._parameterUsed) {
    }

	ParameterSelection* clone() const { return new PsConstant(*this); }

	void reset() {

	}

	void update(std::vector<std::pair<double, unsigned int>> &rewards) {

	}

	void update(std::pair<double, unsigned int> &rewards) {

	}
		
	std::vector<unsigned int> getParameter(const unsigned int nbNodes) {
		std::vector<unsigned int> parameterList;
		
		for (unsigned int i = 0 ; i < nbNodes ; i++)
			parameterList.push_back(_parameterUsed);
		
		return parameterList;
	}

	unsigned int getParameter() {
		return _parameterUsed;
	}

    std::string className() const {
        return "PsConstant";
    }

	protected:
		unsigned int _parameterUsed;

};

#endif
