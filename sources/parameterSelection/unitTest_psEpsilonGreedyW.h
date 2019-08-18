#ifndef UNITTEST_PSESPSILONGREEDYW_H
#define UNITTEST_PSESPSILONGREEDYW_H

#include <iostream>
#include <sstream>

#include "psEpsilonGreedyW.h"

using namespace CppUnit;


class UnitTest_PsEspsilonGreedyW : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_PsEspsilonGreedyW);
    CPPUNIT_TEST(update);
    CPPUNIT_TEST(getParameter);
    CPPUNIT_TEST_SUITE_END();

    public:

    void setUp(void) {
    }
	
    void tearDown(void) {
    }

    void update(void) {
        std::shared_ptr<std::mt19937> mt_rand = make_shared<std::mt19937>();
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

    void getParameter(void) {
        std::shared_ptr<std::mt19937> mt_rand = make_shared<std::mt19937>();
		unsigned int nbParameter = 2;
		const double espilon = 0.01;
		const unsigned int windowSize = 5;
        PsEspsilonGreedy egreedy(mt_rand, nbParameter, espilon, windowSize);

        pair<double, unsigned int> r0(5, 0);    // Parameter 0, reward = 1
        pair<double, unsigned int> r1(1, 1);    // Parameter 1, reward = 5

        egreedy.update(r0);
        egreedy.update(r1);

        unsigned int p0 = 0, p1 = 0;
        for (unsigned int i = 0 ; i < 50 ; i++) {
            unsigned int p = egreedy.getParameter();
            if (p == 0) {
                p0++;
            } else if (p == 1) {
                p1++;
            }
        }
        CPPUNIT_ASSERT(p1 < p0);
        CPPUNIT_ASSERT(25 < (p0 - p1));
    }

    private:
};

#endif