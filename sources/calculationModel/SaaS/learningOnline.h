#ifndef SAAS_LEARNING_H
#define	SAAS_LEARNING_H

#include <memory>
#include <utility>

#include "../calculationModel.h"
#include "../../rewardComputation/rewardComputation.h"
#include "../../parameterSelection/parameterSelection.h"
#include "../../launcher/launcher.h"

template<class SOL>
class LearningOnline : public CalculationModel {
public:
    LearningOnline(std::unique_ptr<ParameterSelection> parameterSelection,
                    std::unique_ptr<RewardComputation<SOL>> rewardComputation) :
        _parameterSelection(std::move(parameterSelection)),
        _rewardComputation(std::move(rewardComputation)) {

    }
    
    virtual ~LearningOnline() {

    }

    //
    pair<SOL, unsigned int> initialSolution(const SOL &s) {
        solution_t0 = s;
        return pair<SOL, unsigned int>(solution_t0, 0);
    }

    pair<SOL, unsigned int> run(const SOL &s, unsigned int parameter) {
        solution_t1 = s;
        
        pair<double, unsigned int> rewardOp = _rewardComputation->operator()(solution_t0, solution_t1, parameter);

        // update
        _parameterSelection->update(rewardOp);

        unsigned int new_parameter = _parameterSelection->getParameter();

        solution_t0 = solution_t1;

        return pair<SOL, unsigned int>(solution_t1, new_parameter);
    }

    void operator()() {

    }

protected:
    std::unique_ptr<ParameterSelection> _parameterSelection;
    std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
    SOL solution_t0;
    SOL solution_t1;

};

#endif