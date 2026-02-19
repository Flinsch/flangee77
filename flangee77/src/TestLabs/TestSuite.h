#ifndef TL7_TESTSUITE_H
#define TL7_TESTSUITE_H

#include "./TestCase.h"
#include "./SubcaseBranchSwitch.h"
#include "./Context.h"
#include "./ResultBuilder.h"
#include "./Result.h"
#include "./Stats.h"

#include "./reporting/Reporter.h"
#include "./reporting/CoutLogger.h"

#include "./macros.h"
#include "./internals.h"

#include <vector>



/**
 * This entire testing module is heavily inspired by the C++ implementation of the
 * "doctest" testing framework:
 * https://github.com/doctest/doctest
 */



namespace tl7 {



class TestSuite
{

public:
    struct Settings
    {
        /** The maximum tolerated number of failed checks (0: no limit). If it is exceeded between two (sub)cases, test execution will not continue. */
        unsigned max_check_fail_count = 0;
    };



    TestSuite(const TestSuite&) = delete;
    TestSuite& operator=(const TestSuite&) = delete;
    TestSuite(TestSuite&&) = delete;
    TestSuite& operator=(TestSuite&&) = delete;




    // #############################################################################
    // Singleton Object/Access
    // #############################################################################

    static TestSuite& instance()
    {
        static TestSuite _instance;
        return _instance;
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Registers the specified test case.
     */
    int register_test_case(TestCase::FuncType func, cl7::u8string_view name, const char* file_path, unsigned line_number, const char* function_name);

    /**
     * Runs the registered test cases.
     */
    void run_tests();



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the settings.
     */
    const Settings& get_settings() const { return _settings; }

    /**
     * Sets the settings.
     */
    void set_settings(const Settings& settings) { _settings = settings; }

    /**
     * Returns the central reporter object for communicating test results etc.
     */
    const reporting::Reporter& get_reporter() const { return _reporter; }

    /**
     * Returns the aggregated statistics about tests carried out.
     */
    const Stats& get_stats() const { return _stats; }

    /**
     * Resets the aggregated statistics about tests carried out.
     */
    void reset_stats() { _stats = {}; }



private:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    TestSuite();

    ~TestSuite();



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Runs the specified test case (and all of its branches).
     */
    void _run_test_case(TestCase& test_case);

    /**
     * Runs a specific test (sub)case branch. Cancels test execution in case of any
     * exception and returns false then (true otherwise).
     */
    bool _run_test_case_branch(TestCase& test_case, Context& ctx);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The settings.
     */
    Settings _settings;

    /**
     * The registered test cases.
     */
    std::unique_ptr<std::vector<TestCasePtr>> _registered_test_cases;

    /**
     * The central listener object for logging test results etc. to the standard
     * output stream.
     */
    std::unique_ptr<reporting::CoutLogger> _cout_logger;

    /**
     * The central reporter object for communicating test results etc.
     */
    reporting::Reporter _reporter;

    /**
     * The aggregated statistics about tests carried out.
     */
    Stats _stats;

}; // class TestSuite



} // namespace tl7

#endif // TL7_TESTSUITE_H
