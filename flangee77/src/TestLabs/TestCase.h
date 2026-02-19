#ifndef TL7_TESTCASE_H
#define TL7_TESTCASE_H

#include "./Meta.h"
#include "./Context.h"

#include <CoreLabs/string.h>



namespace tl7 {



class TestCase;
using TestCasePtr = std::unique_ptr<TestCase>;



class TestCase
{

public:
    using FuncType = std::add_pointer_t<void (Context &)>;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    TestCase() = delete;

    TestCase(FuncType func, cl7::u8string_view name, const char* file_path, unsigned line_number, const char* function_name);

    TestCase(const TestCase&) = delete;
    TestCase& operator=(const TestCase&) = delete;
    TestCase(TestCase&&) = delete;
    TestCase& operator=(TestCase&&) = delete;

    ~TestCase() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * The meta description of where the test case has been defined and registered.
     */
    const Meta& meta() const { return _meta; }

    /**
     * Runs the test case.
     */
    void run(Context& ctx);



private:

    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The meta description of where the test case has been defined and registered.
     */
    const Meta _meta;

    /**
     * The function to be called running the test case.
     */
    const FuncType _func;

}; // class TestCase



} // namespace tl7

#endif // TL7_TESTCASE_H
