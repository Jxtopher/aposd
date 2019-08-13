#ifndef JXTOPHER_CLASSBUILDER_H
#define JXTOPHER_CLASSBUILDER_H

#include <memory>

#include "../parameterSelection/parameterSelection.h"

class ClassBuilder {
public:

    unique_ptr<ParameterSelection> operator()(const Json::Value &configuration) {

        return nullptr;
    }
private:


};

#endif