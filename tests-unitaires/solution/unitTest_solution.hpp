#ifndef UNITTEST_SOLUTION_H
#define UNITTEST_SOLUTION_H

#include <iostream>
#include <sstream>

#include "solution/solution.hpp"

using namespace CppUnit;

class UnitTest_Solution : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UnitTest_Solution);
	CPPUNIT_TEST(test_toSolution);
	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp(void) {}

	void tearDown(void) {}

	Json::Value stringAsjson(const std::string& strJson) {
		Json::Value root;
		Json::Reader reader;
		bool parsingSuccessful = reader.parse(strJson.c_str(), root);
		if(!parsingSuccessful)
			throw std::runtime_error(std::string{} + __FILE__ + ":" + std::to_string(__LINE__) +
									 " " + reader.getFormattedErrorMessages());
		return root;
	}

	void test_toSolution(void) {
		std::string str_solution = std::string("{\"fitness\" : [12, 4.2], \"fitnessIsValid\" : "
											   "[true, true], \"solution\" : [0, 0, 3, 1, 4]}");
		Solution<double> s(stringAsjson(str_solution));

		CPPUNIT_ASSERT(s.getFitness(0) == 12);
		CPPUNIT_ASSERT(s.getFitness(1) == 4.2);
		CPPUNIT_ASSERT(s.getSolution() == "[0, 0, 3, 1, 4]");
		std::stringstream solution;
		solution << s;
		// CPPUNIT_ASSERT(solution.str() == "12 4.2 : 0 0 3 1 4");
	}

  private:
};

#endif