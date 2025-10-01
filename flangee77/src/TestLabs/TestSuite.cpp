#include "TestSuite.h"

#include "./ResultBuilder.h"

#include "./exceptions/assertion_exception.h"

#include <CoreLabs/system/datetime.h>
#include <CoreLabs/text/codec.h>



namespace tl7 {



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Registers the specified test case.
     */
    int TestSuite::register_test_case(TestCase::FuncType func, cl7::u8string_view name, const char* file_path, unsigned line_number)
    {
        if (!_registered_test_cases)
            _registered_test_cases = std::make_unique<std::vector<TestCasePtr>>();
        _registered_test_cases->emplace_back(std::make_unique<TestCase>(func, name, file_path, line_number));

        return 0;
    }

    /**
     * Runs the registered test cases.
     */
    void TestSuite::run_tests()
    {
        if (!_registered_test_cases)
            return;

        auto test_cases = std::move(_registered_test_cases);
        assert(!_registered_test_cases);

        _reporter.post_start_run(test_cases->size());

        const auto msecs0 = cl7::system::datetime::current_msecs_since_epoch();

        assert(test_cases);
        for (const TestCasePtr& p : *test_cases)
        {
            _run_test_case(*p);

            if (_settings.max_check_fail_count > 0 && _stats.checks.fail_count > _settings.max_check_fail_count)
            {
                // Report something?
                break;
            }
        }

        const auto msecs1 = cl7::system::datetime::current_msecs_since_epoch();
        _stats.execution_time_msecs += static_cast<unsigned>(msecs1 - msecs0);

        _reporter.post_end_run(_stats);
    }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    TestSuite::TestSuite()
    {
        //if (true) // Make it optional/configurable or something?
        {
            _cout_logger = std::make_unique<reporting::CoutLogger>();
            _reporter.add_listener(0, _cout_logger.get());
        }
    }

    /**
     * Destructor.
     */
    TestSuite::~TestSuite()
    {
        if (_cout_logger)
            _reporter.remove_listener(_cout_logger.get());
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Runs the specified test case (and all of its branches).
     */
    void TestSuite::_run_test_case(TestCase& test_case)
    {
        Meta root_meta = test_case.meta();
        Context ctx(&_reporter, &root_meta);

        _reporter.post_start_case(root_meta);

        do
        {
            ctx.subcases.start_run();

            const unsigned fail_count = ctx.stats.interim_fail_count();
            const bool b = _run_test_case_branch(test_case, ctx);
            _reporter.post_result(ResultBuilder::make_test_case_result(ctx, Result::make_outcome(fail_count == ctx.stats.interim_fail_count())));
            if (!b)
                break;

            const unsigned total_check_fail_count = _stats.checks.fail_count + ctx.stats.checks.fail_count;
            if (_settings.max_check_fail_count > 0 && total_check_fail_count > _settings.max_check_fail_count)
                break;

            assert(ctx.subcases.get_current_depth() == 0);
        }
        while (ctx.subcases.has_next());

        ctx.stats.cases.update(ctx.stats.subcases.fail_count == 0);
        _stats += ctx.stats;

        _reporter.post_end_case(ctx.stats);
    }

    /**
     * Runs a specific test (sub)case branch. Cancels test execution in case of any
     * exception and returns false then (true otherwise).
     */
    bool TestSuite::_run_test_case_branch(TestCase& test_case, Context& ctx)
    {
        try
        {
            test_case.run(ctx);
            return true;
        }
        catch (const exceptions::assertion_exception& e)
        {
            _reporter.post_result(ResultBuilder::make_assertion_result(ctx, e.original_expression, e.evaluated_expression, e.meta.file_path, e.meta.line_number, Result::Outcome::Failure));
        }
        catch (const std::exception& e)
        {
            _reporter.post_result(ResultBuilder::make_exception_result(ctx, cl7::text::codec::reinterpret_utf8(e.what())));
        }
        catch (const std::string& message)
        {
            _reporter.post_result(ResultBuilder::make_exception_result(ctx, cl7::text::codec::reinterpret_utf8(message)));
        }
        catch (const char* message)
        {
            _reporter.post_result(ResultBuilder::make_exception_result(ctx, cl7::text::codec::reinterpret_utf8(message)));
        }
        catch (...)
        {
            _reporter.post_result(ResultBuilder::make_exception_result(ctx, u8"Unknown exception"));
        }

        return false;
    }



} // namespace tl7
