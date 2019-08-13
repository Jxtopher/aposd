#ifndef REWARDCOMPUTATION_H
#define	REWARDCOMPUTATION_H

#include <utility>
#include <cassert>

using namespace std;

template <typename SOL>
class RewardComputation {
public:
    RewardComputation() {

    }

    virtual ~RewardComputation() {

    }


    pair<double, unsigned int> operator()(const SOL & previousSolution, const SOL & solution, const unsigned int & parameter) const {
        pair<double, unsigned int> rewardOp(solution.getFitness() - previousSolution.getFitness(), parameter);
        return rewardOp;
    }

    vector<pair<double, unsigned int>> operator()(const vector<SOL> &previousSolution, const vector<SOL> & solution, const vector<unsigned int> &parameter) const {
        vector<pair<double, unsigned int>> rewardOp;
        assert(previousSolution.size() == solution.size());
        assert(previousSolution.size() == parameter.size());
        for (unsigned int i = 0 ; i < previousSolution.size() ; i++) {
            rewardOp.push_back(pair<double, unsigned int>(solution[i].getFitness() - previousSolution[i].getFitness(), parameter[i]));
        }
        return rewardOp;
    }
    

protected:

};

#endif