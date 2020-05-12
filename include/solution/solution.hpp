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

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/program_options.hpp>

#include <cassert>
#include <cstdio>
#include <cstring>
#include <memory>
#include <string>

#include <jsoncpp/json/json.h>

template<typename TYPE_FITNESS> class Solution {
  public:
	Solution(const Solution& s);

	Solution();

	Solution(const unsigned int numberOfObjective);

	Solution(const Json::Value& jsonValue);

	Solution& operator=(Solution const& s) {
		if(_numberOfObjective != s._numberOfObjective) {
			_numberOfObjective = s._numberOfObjective;
			_fitness = std::unique_ptr<TYPE_FITNESS[]>(new TYPE_FITNESS[_numberOfObjective]);
			_fitnessIsValid = std::unique_ptr<bool[]>(new bool[_numberOfObjective]);
			for(unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
		}

		for(unsigned int i = 0; i < _numberOfObjective; i++) {
			_fitness[i] = s._fitness[i];
			_fitnessIsValid[i] = s._fitnessIsValid[i];
		}

		sol = s.sol;
		return *this;
	}

	~Solution();

	bool fitnessIsValid(unsigned int numObjectif) const;

	bool fitnessIsValid() const;

	void setFitness(unsigned int numObjectif, TYPE_FITNESS value);

	void setFitness(TYPE_FITNESS value);

	TYPE_FITNESS getFitness() const;

	TYPE_FITNESS getFitness(unsigned int numObjectif) const;

	unsigned int numberOfObjective() const;

	std::string getSolution() const;

	std::string str();


	friend std::ostream& operator<<(std::ostream& out, const Solution<TYPE_FITNESS>& s) {
		for(unsigned int i = 0; i < s.numberOfObjective(); i++) out << s.getFitness(i) << " ";
		out << ":" << s.getSolution();
		return out;
	}

	void loadJson(const std::string& strJson);

	void loadJson(const Json::Value& jsonValue);

	Json::Value asJson();

  protected:
	unsigned int _numberOfObjective;
	std::unique_ptr<TYPE_FITNESS[]> _fitness;
	std::unique_ptr<bool[]> _fitnessIsValid;
	Json::Value sol;
};

#endif
