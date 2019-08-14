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
    LearningOnline(std::unique_ptr<Launcher> launcher,
                    std::unique_ptr<ParameterSelection> parameterSelection,
                    std::unique_ptr<RewardComputation<SOL>> rewardComputation) : 
        _launcher(std::move(launcher)),
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


// // Initialisation de la 1er solution
// Solution<unsigned int> s(_launcher->initSolution());
// cout<<">"<<s<<"<"<<endl;

// _parameterSelection->reset();


// while(s.getFitness() < 50) {
//     // Get list operators to apply
//     unsigned int parameter = _parameterSelection->getParameter();

//     Solution<unsigned int> s_new(_launcher->solve(s.str(), parameter));

//     pair<double, unsigned int> rewardOp = _rewardComputation->operator()(s, s_new, parameter);

//     // update
//     _parameterSelection->update(rewardOp);

//     s = s_new;

//     cout<<"Out>"<<parameter<<" | "<<s<<"<"<<endl;
// }

protected:
    std::unique_ptr<Launcher> _launcher;
    std::unique_ptr<ParameterSelection> _parameterSelection;
    std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
    SOL solution_t0;
    SOL solution_t1;

};

#endif