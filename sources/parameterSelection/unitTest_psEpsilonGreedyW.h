#ifndef UNITTEST_PSESPSILONGREEDYW_H
#define UNITTEST_PSESPSILONGREEDYW_H

#include <iostream>
#include <sstream>

#include "psEpsilonGreedyW.h"

using namespace CppUnit;
using namespace std;

class UnitTest_PsEspsilonGreedyW : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_PsEspsilonGreedyW);
    CPPUNIT_TEST(test_Constructor);
    CPPUNIT_TEST(test_update);
    CPPUNIT_TEST_SUITE_END();

    public:

    void setUp(void) {
    }
	
    void tearDown(void) {
    }

    void test_Constructor(void) {

    }

    void test_update(void) {
        std::mt19937 mt_rand;
		unsigned int nbParameter = 4;
		const double espilon = 1;
		const unsigned int windowSize = 5;
        PsEspsilonGreedy egreedy(mt_rand, nbParameter, espilon, windowSize);

        pair<double, unsigned int> r1(55, 2);
        pair<double, unsigned int> r2(60, 2);
        pair<double, unsigned int> r3(14, 2);
        pair<double, unsigned int> r4(22, 2);
        pair<double, unsigned int> r5(14, 2);
        pair<double, unsigned int> r6(42, 2);
        egreedy.update(r1);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 55);
        egreedy.update(r2);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 57.5);
        egreedy.update(r3);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 43);
        egreedy.update(r4);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 37.75);
        egreedy.update(r5);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 33);
        egreedy.update(r6);
        CPPUNIT_ASSERT(egreedy.getreward(2) == 30.4);

        CPPUNIT_ASSERT(egreedy.getParameter() == 0);
        CPPUNIT_ASSERT(egreedy.getParameter() == 1);
        CPPUNIT_ASSERT(egreedy.getParameter() == 2);
        CPPUNIT_ASSERT(egreedy.getParameter() == 3);
    }

    private:
};

#endif