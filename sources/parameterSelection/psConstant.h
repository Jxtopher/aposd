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

	void reset() {

	}

	void update(vector<pair<double, unsigned int>> &rewards) {

	}

	void update(pair<double, unsigned int> &rewards) {

	}
	
	vector<unsigned int> getParameter(const unsigned int nbNodes) {
		vector<unsigned int> parameterList;
		
		for (unsigned int i = 0 ; i < nbNodes ; i++)
			parameterList.push_back(_parameterUsed);
		
		return parameterList;
	}

	unsigned int getParameter() {
		return _parameterUsed;
	}

	protected:
		unsigned int _parameterUsed;

};

#endif
