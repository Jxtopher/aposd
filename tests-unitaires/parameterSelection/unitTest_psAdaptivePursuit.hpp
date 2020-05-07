#ifndef UNITTEST_PSADAPTIVEPURSUIT_H
#define UNITTEST_PSADAPTIVEPURSUIT_H

#include <iostream>
#include <sstream>

#include "parameterSelection/psAdaptivePursuit.hpp"

using namespace CppUnit;

class UnitTest_psAdaptivePursuit : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UnitTest_psAdaptivePursuit);
	CPPUNIT_TEST(update);
	CPPUNIT_TEST(roulette_wheel);
	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp(void) {}

	void tearDown(void) {}

	void roulette_wheel(void) {
		std::mt19937 mt_rand;
		mt_rand.seed(0);

		{ // -> TEST 1
			double array1[] = {1, 1, 88, 1, 1, 1};
			double Resultat_1[] = {0, 0, 0, 0, 0, 0};
			unsigned int array_size = 6;

			for(unsigned int i = 0; i < 100; i++) {
				unsigned int pick =
					PsAdaptivePursuit::roulette_wheel<double>(array1, array_size, mt_rand);
				Resultat_1[pick]++;
			}

			CPPUNIT_ASSERT(90 < Resultat_1[2]);
		}

		// -> TEST 2
		{
			double array1[] = {1, 1, 88, 1, 88, 1};
			double Resultat_1[] = {0, 0, 0, 0, 0, 0};
			unsigned int array_size = 6;

			for(unsigned int i = 0; i < 100; i++) {
				unsigned int pick =
					PsAdaptivePursuit::roulette_wheel<double>(array1, array_size, mt_rand);
				Resultat_1[pick]++;
			}

			CPPUNIT_ASSERT(40 < Resultat_1[2]);
			CPPUNIT_ASSERT(40 < Resultat_1[4]);
		}
	}

	void update(void) {
		std::shared_ptr<std::mt19937> mt_rand = std::make_shared<std::mt19937>();
		unsigned int number_of_parameters = 2;
		const double espilon = 0.01;
		const unsigned int windowSize = 5;
		PsAdaptivePursuit adaptive_pursuit(mt_rand, number_of_parameters);

		std::pair<double, unsigned int> r0(5, 0); // Parameter 0, reward = 5
		std::pair<double, unsigned int> r1(1, 1); // Parameter 1, reward = 1

		adaptive_pursuit.update(r0);
		adaptive_pursuit.update(r1);

		unsigned int p0 = 0, p1 = 0;
		for(unsigned int i = 0; i < 100; i++) {
			unsigned int p = adaptive_pursuit.getParameter();
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