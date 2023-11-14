#pragma once
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

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /**
     * Default constructor.
     */
    TestSuite();

    /**
     * Destructor.
     */
    ~TestSuite();

private:
    /** Copy constructor. */
    TestSuite(const TestSuite&) = delete;
    /** Copy assignment operator. */
    TestSuite& operator = (const TestSuite&) = delete;



    // #############################################################################
    // Singleton Object/Access
    // #############################################################################
public:
    static TestSuite& instance()
    {
        static TestSuite _instance;
        return _instance;
    }



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The registered test cases.
     */
    std::unique_ptr<std::vector<TestCasePtr>> _registered_test_cases;

    /**
     * The central listener object for logging test results etc. to the standard
     * output stream.
     */
    std::unique_ptr<reporting::CoutLogger> _cout_logger;

public:
    /**
     * The central reporter object for communicating test results etc.
     */
    reporting::Reporter reporter;

    /**
     * The aggregated statistics about tests carried out.
     */
    Stats stats;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Registers the specified test case.
     */
    int register_test_case(TestCase::FuncType func, const cl7::string_view& name, const cl7::char_type* file_path, unsigned line_number);

    /**
     * Runs the registered test cases.
     */
    void run_tests();



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Runs the specified test case (and all of its branches).
     */
    void _run_test_case(TestCase& test_case);

    /**
     * Runs a specific test (sub)case branch. Cancels test execution in case of any
     * exception and returns false then (true otherwise).
     */
    bool _run_test_case_branch(TestCase& test_case, Context& ctx);

}; // class TestSuite



} // namespace tl7

#endif // TL7_TESTSUITE_H
