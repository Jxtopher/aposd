///
/// @file selection.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef SOLUTIONSELECTION_H
#define SOLUTIONSELECTION_H


template <class SOL>
class SolutionSelection {
    public:
    static constexpr const char* MAX = "max";
    static constexpr const char* MIN = "min";

    SolutionSelection() {

    }

    virtual ~SolutionSelection() {

    }

    virtual SOL operator()(const std::vector<SOL> &solutions) const = 0;

    protected:

};

#endif
