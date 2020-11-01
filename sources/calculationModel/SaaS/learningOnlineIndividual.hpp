#ifndef SAAS_LEARNINGONLINEINDIVIDUAL_H
#define SAAS_LEARNINGONLINEINDIVIDUAL_H

#include <memory>
#include <utility>

#include "../../launcher/launcher.hpp"
#include "../../parameterSelection/parameterSelection.hpp"
#include "../../rewardComputation/rewardComputation.hpp"
#include "../../selection/solutionSelection.hpp"
#include "learningOnline.hpp"

template<class SOL>
class LearningOnlineIndividual : public LearningOnline<SOL> {
  public:
    LearningOnlineIndividual(std::unique_ptr<ParameterSelection> parameterSelection,
                             std::unique_ptr<RewardComputation<SOL>> rewardComputation,
                             std::unique_ptr<SolutionSelection<SOL>> selection)
        : _parameterSelection(std::move(parameterSelection)),
          _rewardComputation(std::move(rewardComputation)), _selection(std::move(selection)) {
    }

    virtual ~LearningOnlineIndividual() {
    }

    //
    std::pair<SOL, unsigned int> initial_solution(const SOL& s) {
        solution_t0 = s;
        return std::pair<SOL, unsigned int>(solution_t0, 0);
    }

    std::pair<SOL, unsigned int> run(const SOL& s_t0, const SOL s_t1, unsigned int parameter) {
        BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " run Individual ";
        solution_t0 = s_t0;
        solution_t1 = s_t1;

        std::pair<double, unsigned int> rewardOp =
            _rewardComputation->operator()(solution_t0, solution_t1, parameter);

        // update
        _parameterSelection->update(rewardOp);

        unsigned int new_parameter = _parameterSelection->getParameter();

        solution_t0 = solution_t1;

        return std::pair<SOL, unsigned int>(solution_t1, new_parameter);
    }

    void operator()() {
    }

  protected:
    std::unique_ptr<ParameterSelection> _parameterSelection;
    std::unique_ptr<RewardComputation<SOL>> _rewardComputation;
    std::unique_ptr<SolutionSelection<SOL>> _selection;
    SOL solution_t0;
    SOL solution_t1;
};

#endif