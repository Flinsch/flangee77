#include "TestCase.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    TestCase::TestCase(FuncType func, const cl7::string_view& name, const cl7::char_type* file_path, unsigned line_number)
        : _func( func )
        , meta{ cl7::string(name), file_path, line_number }
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
        _func( ctx );
    }



} // namespace tl7
