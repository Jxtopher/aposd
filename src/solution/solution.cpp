///
/// @file solution.h
/// @author Jxtopher
/// @version 1
/// @copyright CC-BY-NC-SA
/// @date 2018-10
/// @brief defined the solution
///


#include "solution/solution.hpp"

template<typename TYPE_FITNESS>
Solution<TYPE_FITNESS>::Solution(const Solution& s) : _numberOfObjective(s._numberOfObjective) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__
								<< " Constructeur de copie Solution(const Solution &s)";
	_fitness = std::unique_ptr<TYPE_FITNESS[]>(new TYPE_FITNESS[_numberOfObjective]);
	_fitnessIsValid = std::unique_ptr<bool[]>(new bool[_numberOfObjective]);
	for(unsigned int i = 0; i < _numberOfObjective; i++) {
		_fitness[i] = s._fitness[i];
		_fitnessIsValid[i] = s._fitnessIsValid[i];
	}

	sol = s.sol;
}

template<typename TYPE_FITNESS>
Solution<TYPE_FITNESS>::Solution() : _numberOfObjective(1) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " Creation Solution()";
	_fitness = std::unique_ptr<TYPE_FITNESS[]>(new TYPE_FITNESS[_numberOfObjective]);
	_fitnessIsValid = std::unique_ptr<bool[]>(new bool[_numberOfObjective]);
	for(unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
}

template<typename TYPE_FITNESS>
Solution<TYPE_FITNESS>::Solution(const unsigned int numberOfObjective) : _numberOfObjective(numberOfObjective) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__
								<< " Creation Solution(const unsigned int numberOfObjective)";
	_fitness = std::unique_ptr<TYPE_FITNESS[]>(new TYPE_FITNESS[_numberOfObjective]);
	_fitnessIsValid = std::unique_ptr<bool[]>(new bool[_numberOfObjective]);
	for(unsigned int i = 0; i < _numberOfObjective; i++) _fitnessIsValid[i] = false;
}

template<typename TYPE_FITNESS>
Solution<TYPE_FITNESS>::Solution(const Json::Value& jsonValue)
	: _numberOfObjective(0), _fitness(nullptr), _fitnessIsValid(nullptr) {
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " Creation Solution";
	loadJson(jsonValue);
}

template<typename TYPE_FITNESS>
Solution<TYPE_FITNESS>::~Solution() { 
	BOOST_LOG_TRIVIAL(debug) << __FILE__ << ":" << __LINE__ << " Delete Solution"; 
}

template<typename TYPE_FITNESS>
bool Solution<TYPE_FITNESS>::fitnessIsValid(unsigned int numObjectif) const {
	assert(numObjectif < _numberOfObjective);
	return _fitnessIsValid[numObjectif];
}

template<typename TYPE_FITNESS>
bool Solution<TYPE_FITNESS>::fitnessIsValid() const {
	unsigned int numObjectif = 0;
	assert(numObjectif < _numberOfObjective);
	return _fitnessIsValid[numObjectif];
}

template<typename TYPE_FITNESS>
void Solution<TYPE_FITNESS>::setFitness(unsigned int numObjectif, TYPE_FITNESS value) {
	assert(numObjectif < _numberOfObjective);
	if(_fitness[numObjectif] != value) {
		_fitness[numObjectif] = value;
		_fitnessIsValid[numObjectif] = true;
	}
}

template<typename TYPE_FITNESS>
void Solution<TYPE_FITNESS>::setFitness(TYPE_FITNESS value) {
	unsigned int numObjectif = 0;
	assert(numObjectif < _numberOfObjective);
	if(_fitness[numObjectif] != value) {
		_fitness[numObjectif] = value;
		_fitnessIsValid[numObjectif] = true;
	}
}

template<typename TYPE_FITNESS>
TYPE_FITNESS Solution<TYPE_FITNESS>::getFitness() const {
	unsigned int numObjectif = 0;
	assert(numObjectif < _numberOfObjective);
	return _fitness[numObjectif];
}

template<typename TYPE_FITNESS>
TYPE_FITNESS Solution<TYPE_FITNESS>::getFitness(unsigned int numObjectif) const {
	assert(numObjectif < _numberOfObjective);
	return _fitness[numObjectif];
}

template<typename TYPE_FITNESS>
unsigned int Solution<TYPE_FITNESS>::numberOfObjective() const { return _numberOfObjective; }

template<typename TYPE_FITNESS>
std::string Solution<TYPE_FITNESS>::getSolution() const {
	Json::StreamWriterBuilder builder;
	builder["commentStyle"] = "None";
	builder["indentation"] = "";
	return Json::writeString(builder, sol);
}

template<typename TYPE_FITNESS>
std::string Solution<TYPE_FITNESS>::str() {
	std::stringstream ss;
	for(unsigned int i = 0; i < numberOfObjective(); i++) ss << getFitness(i) << " ";
	ss << ":" << getSolution();
	return ss.str();
}

template<typename TYPE_FITNESS>
void Solution<TYPE_FITNESS>::loadJson(const std::string& strJson) {
	Json::Value root;
	Json::Reader reader;
	bool parsingSuccessful = reader.parse(strJson.c_str(), root); // parse process
	if(!parsingSuccessful)
		throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
									" " + reader.getFormattedErrorMessages());
	loadJson(root);
}

template<typename TYPE_FITNESS>
void Solution<TYPE_FITNESS>::loadJson(const Json::Value& jsonValue) {
	_numberOfObjective = jsonValue["fitness"].size();
	if(this->_fitness == nullptr) {
		this->_fitness =
			std::unique_ptr<TYPE_FITNESS[]>(new TYPE_FITNESS[this->_numberOfObjective]);
		this->_fitnessIsValid = std::unique_ptr<bool[]>(new bool[this->_numberOfObjective]);
	} else {
		this->_fitness.reset(static_cast<TYPE_FITNESS*>(
			realloc(static_cast<void*>(this->_fitness.release()),
					this->_numberOfObjective * sizeof(TYPE_FITNESS))));
		this->_fitnessIsValid.reset(
			static_cast<bool*>(realloc(static_cast<void*>(this->_fitnessIsValid.release()),
										this->_numberOfObjective * sizeof(bool))));
	}
	for(unsigned int i = 0; i < jsonValue["fitness"].size(); i++) {
		_fitness[i] = static_cast<TYPE_FITNESS>(jsonValue["fitness"][i].asDouble());
		_fitnessIsValid[i] = jsonValue["fitnessIsValid"][i].asBool();
	}

	sol = jsonValue["solution"].empty() ? Json::Value() : jsonValue["solution"];
}

template<typename TYPE_FITNESS>
Json::Value Solution<TYPE_FITNESS>::asJson() {
	Json::Value jsonValue;
	for(unsigned int i = 0; i < _numberOfObjective; i++) {
		jsonValue["fitness"].append(_fitness[i]);
		jsonValue["fitnessIsValid"].append(_fitnessIsValid[i]);
	}

	if(!sol.empty()) jsonValue["solution"] = sol;
	return jsonValue;
}
