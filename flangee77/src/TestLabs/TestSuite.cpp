#include "TestSuite.h"

#include "./ResultBuilder.h"

#include "./exceptions/assertion_exception.h"

#include <CoreLabs/system/datetime.h>
#include <CoreLabs/strings.h>



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    TestSuite::TestSuite()
        : settings()
        , _registered_test_cases()
        , _cout_logger()
        , reporter()
        , stats()
    {
        if ( true ) // Make it optional/configurable or something?
        {
            _cout_logger = std::make_unique<reporting::CoutLogger>();
            reporter.add_listener( 0, _cout_logger.get() );
        }
    }

    /**
     * Destructor.
     */
    TestSuite::~TestSuite()
    {
        if ( _cout_logger )
            reporter.remove_listener( _cout_logger.get() );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Registers the specified test case.
     */
    int TestSuite::register_test_case(TestCase::FuncType func, cl7::string_view name, const cl7::char_type* file_path, unsigned line_number)
    {
        if ( !_registered_test_cases )
            _registered_test_cases = std::make_unique<std::vector<TestCasePtr>>();
        _registered_test_cases->emplace_back( std::make_unique<TestCase>( func, name, file_path, line_number ) );

        return 0;
    }

    /**
     * Runs the registered test cases.
     */
    void TestSuite::run_tests()
    {
        if ( !_registered_test_cases )
            return;

        auto test_cases = std::move(_registered_test_cases);
        assert( !_registered_test_cases );

        reporter.post_start_run( test_cases->size() );

        const auto msecs0 = cl7::system::datetime::current_msecs_since_epoch();

        assert( test_cases );
        for ( const TestCasePtr& p : *test_cases )
        {
            _run_test_case( *p );

            if ( settings.max_check_fail_count > 0 && stats.checks.fail_count > settings.max_check_fail_count )
            {
                // Report something?
                break;
            }
        }

        const auto msecs1 = cl7::system::datetime::current_msecs_since_epoch();
        stats.execution_time_msecs += static_cast<unsigned>( msecs1 - msecs0 );

        reporter.post_end_run( stats );
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Runs the specified test case (and all of its branches).
     */
    void TestSuite::_run_test_case(TestCase& test_case)
    {
        Meta root_meta = test_case.meta;
        Context ctx( &reporter, &root_meta );

        reporter.post_start_case( root_meta );

        do
        {
            ctx.subcases.start_run();

            const unsigned fail_count = ctx.stats.interim_fail_count();
            const bool b = _run_test_case_branch( test_case, ctx );
            reporter.post_result( ResultBuilder().make_test_case_result( ctx, Result::make_outcome( fail_count == ctx.stats.interim_fail_count() ) ) );
            if ( !b )
                break;

            const unsigned total_check_fail_count = stats.checks.fail_count + ctx.stats.checks.fail_count;
            if ( settings.max_check_fail_count > 0 && total_check_fail_count > settings.max_check_fail_count )
                break;

            assert( ctx.subcases.get_current_depth() == 0 );
        }
        while ( ctx.subcases.has_next() );

        ctx.stats.cases.update( ctx.stats.subcases.fail_count == 0 );
        stats += ctx.stats;

        reporter.post_end_case( ctx.stats );
    }

    /**
     * Runs a specific test (sub)case branch. Cancels test execution in case of any
     * exception and returns false then (true otherwise).
     */
    bool TestSuite::_run_test_case_branch(TestCase& test_case, Context& ctx)
    {
        try
        {
            test_case.run( ctx );
            return true;
        }
        catch (const exceptions::assertion_exception& e)
        {
            reporter.post_result( ResultBuilder().make_assertion_result( ctx, e.original_expression, e.evaluated_expression, e.meta.file_path, e.meta.line_number, Result::Outcome::Failure ) );
        }
        catch (const std::exception& e)
        {
            reporter.post_result( ResultBuilder().make_exception_result( ctx, cl7::strings::from_latin1( e.what() ) ) );
        }
        catch (const std::string& message)
        {
            reporter.post_result( ResultBuilder().make_exception_result( ctx, cl7::strings::from_latin1( message ) ) );
        }
        catch (const char* message)
        {
            reporter.post_result( ResultBuilder().make_exception_result( ctx, cl7::strings::from_latin1( message ) ) );
        }
        catch (...)
        {
            reporter.post_result( ResultBuilder().make_exception_result( ctx, TEXT("Unknown exception") ) );
        }

        return false;
    }



} // namespace tl7
