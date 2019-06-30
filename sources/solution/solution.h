///
/// \file solution.h
/// \author Jxtopher
/// \version 1
/// \copyright CC-BY-NC-SA
/// \date 2018-10
/// \brief defined the solution
///

#ifndef SOLUTION_H
#define SOLUTION_H

#include <cstdio>
#include <cstring>
#include <string>
#include <memory>
#include <cassert>

#include "../macro.h"

using namespace std;

template <typename TYPE_FITNESS>
class Solution {
   public:
    Solution(const Solution &s) : _numberOfObjective(s._numberOfObjective) {
        DEBUG_TRACE("Constructeur de copie Solution(const Solution &s)");
        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            _fitness[i] = s._fitness[i];
            _fitnessIsValid[i] = s._fitnessIsValid[i];
        }

        sol = s.sol;
    }

    Solution() : _numberOfObjective(1) {
        DEBUG_TRACE("Creation Solution()");
        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
    }

    Solution(const unsigned int numberOfObjective) : _numberOfObjective(numberOfObjective) {
        DEBUG_TRACE("Creation Solution(const unsigned int numberOfObjective)");
        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
    }

    // Deserialization
    /*Solution(unsigned char const *deserialization) {
        DEBUG_TRACE("Constructeur de deserialization");
        unsigned long long size = 0;
        unsigned char const *cursorData = deserialization;

        memcpy(static_cast<void *>(&size), static_cast<const void *>(cursorData), sizeof(unsigned long long));
        cursorData += sizeof(unsigned long long);

        memcpy(static_cast<void *>(&_numberOfObjective), static_cast<const void *>(cursorData), sizeof(unsigned int));
        cursorData += sizeof(unsigned int);

        // std::cout<<size<<std::endl;
        // std::cout<<_numberOfObjective<<std::endl;

        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            memcpy(static_cast<void *>(&_fitness[i]), static_cast<const void *>(cursorData), sizeof(TYPE_FITNESS));
            cursorData += sizeof(TYPE_FITNESS);
        }

        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            memcpy(static_cast<void *>(&_fitnessIsValid[i]), static_cast<const void *>(cursorData), sizeof(bool));
            cursorData += sizeof(bool);
        }

        // for (unsigned int i = 0 ; i < 62 ; i++) {
        //	printf("%X ", deserialization[i]);
        //}
    }*/


    Solution(const string &solution) {
        DEBUG_TRACE("Creation Solution(const string &solution)");
        std::string token;
        std::istringstream ss(solution);
        vector<string> x;

        while (std::getline(ss, token, ':')) {
            x.push_back(token);
        }

        _fitness = nullptr;
        _fitnessIsValid = nullptr;

        // fitness
        unsigned int i = 0;
        _numberOfObjective = 0;
        std::istringstream ssFitness(x[0]);
        while (std::getline(ssFitness, token, ' ')) {
            _numberOfObjective++;
            if (_fitness == nullptr) {
                _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
                _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
            } else {
                cout << "->"<<__LINE__ << ":" << _numberOfObjective << endl;
                _fitness.reset(static_cast<TYPE_FITNESS *>(realloc(static_cast<void*>(_fitness.release()), _numberOfObjective * sizeof(TYPE_FITNESS))));
				_fitnessIsValid.reset(static_cast<bool *>(realloc(static_cast<void*>(_fitnessIsValid.release()), _numberOfObjective * sizeof(bool))));            
            }
            std::stringstream convert(token);
            TYPE_FITNESS value;
            convert >> value;
            this->setFitness(i++, value);
        }

        assert(i == this->numberOfObjective());
        
        // solution
        sol = x[1];
    }

    Solution &operator=(Solution const &s) {
        if (_numberOfObjective != s._numberOfObjective) {
            //this->~Solution();
            _numberOfObjective = s._numberOfObjective;
            //_fitness = new TYPE_FITNESS[_numberOfObjective];
            //_fitnessIsValid = new bool[_numberOfObjective];
            _fitness = unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
            _fitnessIsValid = unique_ptr<bool []>(new bool[_numberOfObjective]);
            for (unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
        }

        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            _fitness[i] = s._fitness[i];
            _fitnessIsValid[i] = s._fitnessIsValid[i];
        }

        sol = s.sol;
        return *this;
    }

    ~Solution() {
        DEBUG_TRACE("Delete Solution");
    }

    // Serialization
    // Return : unsigned char
    // Total size - numberOfObjective - Fitness * numberOfObjective -
    // FitnessIsValid * numberOfObjective
    /*unsigned char *serialization() {
        unsigned long long size = sizeof(unsigned long long) + sizeof(unsigned int) +
                                  (_numberOfObjective * sizeof(TYPE_FITNESS)) + (_numberOfObjective * sizeof(bool));
        unsigned char *data = new unsigned char[size];
        unsigned char *cursorData = data;

        // memset(data, 'A',  size);

        memcpy(static_cast<void *>(cursorData), static_cast<void *>(&size), sizeof(unsigned long long));
        cursorData += sizeof(unsigned long long);

        memcpy(static_cast<void *>(cursorData), static_cast<void *>(&_numberOfObjective), sizeof(unsigned int));
        cursorData += sizeof(unsigned int);

        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            memcpy(static_cast<void *>(cursorData), static_cast<void *>(&_fitness[i]), sizeof(TYPE_FITNESS));
            cursorData += sizeof(TYPE_FITNESS);
        }

        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            memcpy(static_cast<void *>(cursorData), static_cast<void *>(&_fitnessIsValid[i]), sizeof(bool));
            cursorData += sizeof(bool);
        }

        // std::cout<<size<<std::endl;

        // for (unsigned int i = 0 ; i < size ; i++) {
        //	printf("%X ", data[i]);
        //}
        // std::cout<<std::endl;
        return data;
    }*/

    bool fitnessIsValid(unsigned int numObjectif) const {
        assert(numObjectif < _numberOfObjective);
        return _fitnessIsValid[numObjectif];
    }

    bool fitnessIsValid() const {
        unsigned int numObjectif = 0;
        assert(numObjectif < _numberOfObjective);
        return _fitnessIsValid[numObjectif];
    }

    void setFitness(unsigned int numObjectif, TYPE_FITNESS value) {
        assert(numObjectif < _numberOfObjective);
        if (_fitness[numObjectif] != value) {
            _fitness[numObjectif] = value;
            _fitnessIsValid[numObjectif] = true;
        }
    }

    void setFitness(TYPE_FITNESS value) {
        unsigned int numObjectif = 0;
        assert(numObjectif < _numberOfObjective);
        if (_fitness[numObjectif] != value) {
            _fitness[numObjectif] = value;
            _fitnessIsValid[numObjectif] = true;
        }
    }

    TYPE_FITNESS getFitness() const {
        unsigned int numObjectif = 0;
        assert(numObjectif < _numberOfObjective);
        return _fitness[numObjectif];
    }

    TYPE_FITNESS getFitness(unsigned int numObjectif) const {
        assert(numObjectif < _numberOfObjective);
        return _fitness[numObjectif];
    }

    unsigned int numberOfObjective() const { return _numberOfObjective; }

    string getSolution() const { return sol; }

    /*void toSolution(string const &solution) {
            std::string token;
            std::istringstream ss(solution);
            vector<string> x;

            while(std::getline(ss, token, ':')) {
                    x.push_back(token);
            }

            // fitness
            unsigned int i = 0;
            std::istringstream ssFitness(x[0]);
            while(std::getline(ssFitness, token, ' ')) {
                    std::stringstream convert(token);
                    TYPE_FITNESS value;
                    convert >> value;
                    this->setFitness(i++, value);
            }
            assert(i  == this->numberOfObjective());

            // solution
            sol = x[1];

    }*/

    string str() {
        std::stringstream ss;
        for (unsigned int i = 0; i < numberOfObjective(); i++) ss << getFitness(i) << " ";
        ss << ":" << getSolution();
        return ss.str();
    }

    friend std::ostream &operator<<(std::ostream &out, const Solution<TYPE_FITNESS> &s) {
        for (unsigned int i = 0; i < s.numberOfObjective(); i++) out << s.getFitness(i) << " ";
        out << ":" << s.getSolution();
        return out;
    }

   protected:
    unsigned int _numberOfObjective;
    //TYPE_FITNESS *_fitness;
    //bool *_fitnessIsValid;
	unique_ptr<TYPE_FITNESS []> _fitness;
	unique_ptr<bool []>_fitnessIsValid;
    string sol;
};

#endif
