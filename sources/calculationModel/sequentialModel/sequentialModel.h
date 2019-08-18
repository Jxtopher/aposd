#ifndef SEQUENTIALMODEL_H
#define	SEQUENTIALMODEL_H

#include <memory>
#include <utility>

#include "../calculationModel.h"
#include "../../rewardComputation/rewardComputation.h"
#include "../../parameterSelection/parameterSelection.h"
#include "../../launcher/launcher.h"

template<class SOL>
class SequentialModel : public CalculationModel {
public:
    SequentialModel(std::unique_ptr<Launcher> launcher,
                    std::unique_ptr<ParameterSelection> parameterSelection,
                    std::unique_ptr<RewardComputation<SOL>> rewardComputation) : 
        _launcher(std::move(launcher)),
        _parameterSelection(std::move(parameterSelection)),
        _rewardComputation(std::move(rewardComputation)) {

    }
    
    virtual ~SequentialModel() {

    }

    void operator()() {
        // Initialisation de la 1er solution
        Solution<unsigned int> s(_launcher->initSolution());
        std::cout<<">"<<s<<"<"<<std::endl;

        _parameterSelection->reset();


        while(s.getFitness() < 50) {
            // Get list operators to apply
            unsigned int parameter = _parameterSelection->getParameter();

            Solution<unsigned int> s_new(_launcher->solve(s.str(), parameter));

            std::pair<double, unsigned int> rewardOp = _rewardComputation->operator()(s, s_new, parameter);

            // update
            _parameterSelection->update(rewardOp);

            s = s_new;

            std::cout<<"Out>"<<parameter<<" | "<<s<<"<"<<std::endl;
        }
    }

protected:
    std::unique_ptr<Launcher> _launcher;
    std::unique_ptr<ParameterSelection> _parameterSelection;
    std::unique_ptr<RewardComputation<SOL>> _rewardComputation;

};

#endif