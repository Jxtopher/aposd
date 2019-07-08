#ifndef SEQUENTIALMODEL_H
#define	SEQUENTIALMODEL_H

#include <utility>

#include "../calculationModel.h"
#include "../../rewardComputation/rewardComputation.h"
#include "../../parameterSelection/parameterSelection.h"
#include "../../launcher/launcher.h"

template<class SOL>
class SequentialModel : public CalculationModel {
public:
    SequentialModel(Launcher &launcher,
                    ParameterSelection &parameterSelection,
                    RewardComputation<SOL> &rewardComputation) : 
        _launcher(launcher),
        _parameterSelection(parameterSelection),
        _rewardComputation(rewardComputation) {

    }
    
    virtual ~SequentialModel() {

    }

    void operator()() {
        // Initialisation de la 1er solution
        Solution<unsigned int> s(_launcher.initSolution());
        cout<<">"<<s<<"<"<<endl;

        _parameterSelection.reset();


        while(s.getFitness() < 50) {
            // Get list operators to apply
            unsigned int parameter = _parameterSelection.getParameter();

            Solution<unsigned int> s_new(_launcher.solve(s.str(), parameter));

            pair<double, unsigned int> rewardOp = _rewardComputation(s, s_new, parameter);

            // update
            _parameterSelection.update(rewardOp);

            s = s_new;

            cout<<"Out>"<<parameter<<" | "<<s<<"<"<<endl;
        }
    }

protected:
    Launcher &_launcher;
    ParameterSelection &_parameterSelection;
    RewardComputation<SOL> &_rewardComputation;

};

#endif