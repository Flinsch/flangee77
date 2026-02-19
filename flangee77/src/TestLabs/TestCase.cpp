#include "TestCase.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    TestCase::TestCase(FuncType func, cl7::u8string_view name, const char* file_path, unsigned line_number, const char* function_name)
        : _meta{cl7::u8string(name), file_path, line_number, function_name}
        , _func(func)
    {
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Runs the test case.
     */
    void TestCase::run(Context& ctx)
    {
        _func(ctx);
    }



} // namespace tl7
