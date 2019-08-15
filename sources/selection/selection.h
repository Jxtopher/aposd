///
/// @file selection.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief
///

#ifndef SELECTION_H
#define SELECTION_H


template <class SOL>
class Selection {
    public:
    Selection() {

    }

    virtual ~Selection() {

    }

    virtual SOL operator()(const vector<SOL> &solutions) const = 0;

    protected:

};

#endif
