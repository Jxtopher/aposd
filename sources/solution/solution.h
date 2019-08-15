///
/// @file solution.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief defined the solution
///

#ifndef SOLUTION_H
#define SOLUTION_H

#include <cstdio>
#include <cstring>
#include <string>
#include <memory>
#include <cassert>

#include <jsoncpp/json/json.h>

#include "../macro.h"

using namespace std;

template <typename TYPE_FITNESS>
class Solution {
   public:
    Solution(const Solution &s) : _numberOfObjective(s._numberOfObjective) {
        DEBUG_TRACE("Constructeur de copie Solution(const Solution &s)");
        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitness = std::unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = std::unique_ptr<bool []>(new bool[_numberOfObjective]);
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
        _fitness = std::unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = std::unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
    }

    Solution(const unsigned int numberOfObjective) : _numberOfObjective(numberOfObjective) {
        DEBUG_TRACE("Creation Solution(const unsigned int numberOfObjective)");
        //_fitness = new TYPE_FITNESS[_numberOfObjective];
        //_fitnessIsValid = new bool[_numberOfObjective];
        _fitness = std::unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
        _fitnessIsValid = std::unique_ptr<bool []>(new bool[_numberOfObjective]);
        for (unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
    }

	Solution(const Json::Value &jsonValue) :
		_numberOfObjective(0),
		_fitness(nullptr),
		_fitnessIsValid(nullptr)
		 {
		DEBUG_TRACE("Creation Solution");
		loadJson(jsonValue);
	}

    Solution &operator=(Solution const &s) {
        if (_numberOfObjective != s._numberOfObjective) {
            //this->~Solution();
            _numberOfObjective = s._numberOfObjective;
            //_fitness = new TYPE_FITNESS[_numberOfObjective];
            //_fitnessIsValid = new bool[_numberOfObjective];
            _fitness = std::unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[_numberOfObjective]);
            _fitnessIsValid = std::unique_ptr<bool []>(new bool[_numberOfObjective]);
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

    string getSolution() const { 
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "";
        return Json::writeString(builder, sol);
     }

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

    void loadJson(const std::string &strJson) {
        Json::Value root;
        Json::Reader reader;
        bool parsingSuccessful = reader.parse(strJson.c_str(), root);  // parse process
        if (!parsingSuccessful) throw runtime_error(reader.getFormattedErrorMessages());
		loadJson(root);
    }

    void loadJson(const Json::Value &jsonValue) {
        _numberOfObjective = jsonValue["fitness"].size();
		if (this->_fitness == nullptr) {
			this->_fitness = std::unique_ptr<TYPE_FITNESS []>(new TYPE_FITNESS[this->_numberOfObjective]);
			this->_fitnessIsValid = std::unique_ptr<bool []>(new bool[this->_numberOfObjective]);
		} else {
			this->_fitness.reset(static_cast<TYPE_FITNESS *>(realloc(static_cast<void*>(this->_fitness.release()), this->_numberOfObjective * sizeof(TYPE_FITNESS))));
			this->_fitnessIsValid.reset(static_cast<bool *>(realloc(static_cast<void*>(this->_fitnessIsValid.release()), this->_numberOfObjective * sizeof(bool))));
		}
        for (unsigned int i = 0; i < jsonValue["fitness"].size() ; i++) {
            _fitness[i] = static_cast<TYPE_FITNESS>(jsonValue["fitness"][i].asDouble());
            _fitnessIsValid[i] = jsonValue["fitnessIsValid"][i].asBool();
        }

        sol = jsonValue["solution"];
    }

    Json::Value asJson() {
        Json::Value jsonValue;
        for (unsigned int i = 0; i < _numberOfObjective; i++) {
            jsonValue["fitness"].append(_fitness[i]);
            jsonValue["fitnessIsValid"].append(_fitnessIsValid[i]);
        }
        jsonValue["solution"] = sol;
        return jsonValue;
    }

   protected:
    unsigned int _numberOfObjective;
	std::unique_ptr<TYPE_FITNESS []> _fitness;
	std::unique_ptr<bool []>_fitnessIsValid;
    Json::Value sol;
};

#endif
