#ifndef UNITTEST_LAUNCHEREXEC_H
#define UNITTEST_LAUNCHEREXEC_H

#include "launcher/launcherExec.hpp"
#include "solution/solution.hpp"
#include <iostream>
#include <sstream>

using namespace CppUnit;

class UnitTest_LauncherExec : public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(UnitTest_LauncherExec);
	CPPUNIT_TEST(test_launchExecutable);
	CPPUNIT_TEST_SUITE_END();

  public:
	void setUp(void) {}

	void tearDown(void) {}

	void test_launchExecutable(void) {
		std::string x = "echo";
		// LauncherExec lExec(x);
		// string r = lExec.launchExecutable("\"xxx xxxx -1 !\"");
		// CPPUNIT_ASSERT(r == "xxx xxxx -1 !\n");
	}

  private:
};

#endif