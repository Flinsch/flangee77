#pragma once
#ifndef TL7_TESTCASE_H
#define TL7_TESTCASE_H

#include "./Meta.h"
#include "./Context.h"
#include "./Stats.h"

#include <CoreLabs/string.h>



namespace tl7 {



class TestCase;
typedef std::unique_ptr<TestCase> TestCasePtr;



class TestCase
{

public:
    typedef void(*FuncType)(Context& ctx);



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    TestCase(FuncType func, const cl7::string_view& name, const cl7::char_type* file_path, unsigned line_number);

    /**
     * Destructor.
     */
    ~TestCase() = default;

private:
    /** Default constructor. */
    TestCase() = delete;
    /** Copy constructor. */
    TestCase(const TestCase&) = delete;
    /** Copy assignment operator. */
    TestCase& operator = (const TestCase&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The function to be called running the test case.
     */
    const FuncType _func;

public:
    /**
     * The meta description of where the test case has been defined and registered.
     */
    const Meta meta;



    // #############################################################################
    // Properties
    // #############################################################################
public:



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Runs the test case.
     */
    void run(Context& ctx);

}; // class TestCase



} // namespace tl7

#endif // TL7_TESTCASE_H
