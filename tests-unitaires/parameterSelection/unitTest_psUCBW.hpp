#ifndef UNITTEST_PSUCBW_H
#define UNITTEST_PSUCBW_H

#include <iostream>
#include <sstream>

#include "parameterSelection/psUCBW.hpp"

using namespace CppUnit;

class UnitTest_psUCBW : public CppUnit::TestFixture {
    CPPUNIT_TEST_SUITE(UnitTest_psUCBW);
    CPPUNIT_TEST(update);
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp(void) {
    }

    void tearDown(void) {
    }

    void update(void) {
        std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();
        unsigned int number_of_parameters = 2;
        const double espilon = 0.01;
        const unsigned int windowSize = 5;

        PsUCBW ucbw(mt_rand, number_of_parameters, 0.03, 300, AggregationFunction::MEAN);
        std::pair<double, unsigned int> r0(5, 0); // Parameter 0, reward = 5
        std::pair<double, unsigned int> r1(1, 1); // Parameter 1, reward = 1

        for(unsigned int i = 0; i++ < 10;) {
            ucbw.update(r0);
            ucbw.update(r1);
        }

        unsigned int p0 = 0, p1 = 0;
        for(unsigned int i = 0; i < 100; i++) {
            unsigned int p = ucbw.getParameter();
            if(p == 0)
                p0++;
            else if(p == 1)
                p1++;
        }

        CPPUNIT_ASSERT(p1 < p0);
        CPPUNIT_ASSERT(25 < (p0 - p1));
    }

  private:
};

#endif