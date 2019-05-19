#ifndef UNITTEST_SOLUTION_H
#define UNITTEST_SOLUTION_H

#include <iostream>
#include <sstream>

#include "solution.h"

using namespace CppUnit;
using namespace std;

class UnitTest_Solution : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_Solution);
    CPPUNIT_TEST(test_toSolution);
    CPPUNIT_TEST_SUITE_END();

    public:

    void setUp(void) {
    }
	
    void tearDown(void) {
    }

    void test_toSolution(void) {
        Solution<double> s(string("12 4.2 : 0 0 3 1 4"));

        CPPUNIT_ASSERT(s.getFitness(0) == 12);
        CPPUNIT_ASSERT(s.getFitness(1) == 4.2);
        CPPUNIT_ASSERT(s.getSolution() == " 0 0 3 1 4");
        stringstream solution;
        solution <<s;
        CPPUNIT_ASSERT(solution.str() == "12 4.2 : 0 0 3 1 4");
    }

    private:
};

#endif