///
/// @file unitTest.cpp
/// @author *
/// @version *
/// @date *
/// @brief http://www.yolinux.com/TUTORIALS/CppUnit.html
///

#define BOOST_LOG_DYN_LINK 0

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <cppunit/BriefTestProgressListener.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/TestRunner.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TextTestRunner.h>
#include <netinet/in.h>

#include "launcher/unitTest_launcherExec.hpp"
#include "parameterSelection/unitTest_psAdaptivePursuit.hpp"
#include "parameterSelection/unitTest_psConstant.hpp"
#include "parameterSelection/unitTest_psEpsilonGreedyW.hpp"
#include "parameterSelection/unitTest_psRandom.hpp"
#include "parameterSelection/unitTest_psSelectBestMutate.hpp"
#include "parameterSelection/unitTest_psUCBW.hpp"
#include "solution/unitTest_solution.hpp"

CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_LauncherExec);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_Solution);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_PsEspsilonGreedyW);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_psAdaptivePursuit);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_psSelectBestMutate);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_psUCBW);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_psRandom);
CPPUNIT_TEST_SUITE_REGISTRATION(UnitTest_psConstant);

int main(int argc, char* argv[]) {
	boost::log::core::get()->set_logging_enabled(false);

	// informs test-listener about testresults
	CPPUNIT_NS::TestResult testresult;

	// register listener for collecting the test-results
	CPPUNIT_NS::TestResultCollector collectedresults;
	testresult.addListener(&collectedresults);

	// register listener for per-test progress output
	CPPUNIT_NS::BriefTestProgressListener progress;
	testresult.addListener(&progress);

	// insert test-suite at test-runner by registry
	CPPUNIT_NS::TestRunner testrunner;
	testrunner.addTest(CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest());
	testrunner.run(testresult);

	// output results in compiler-format
	CPPUNIT_NS::CompilerOutputter compileroutputter(&collectedresults, std::cerr);
	compileroutputter.write();

	// Output XML for Jenkins CPPunit plugin
	// ofstream xmlFileOut("cppTestResults.xml");
	// XmlOutputter xmlOut(&collectedresults, xmlFileOut);
	// xmlOut.write();

	// return 0 if tests were successful
	return collectedresults.wasSuccessful() ? 0 : 1;
}