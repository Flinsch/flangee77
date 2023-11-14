#include "Context.h"

#include "./exceptions/assertion_exception.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Context::Context(reporting::Reporter* reporter, const Meta* root_meta)
        : reporter( reporter )
        , root_meta( root_meta )
        , subcases()
        , stats()
    {
        reporter->add_listener( 0, this );
    }

    /**
     * Destructor.
     */
    Context::~Context()
    {
        reporter->remove_listener( this );
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Publishes the specified result by forwarding it to the reporter, with a few
     * exceptions: In the case of a "successful presumption", nothing happens. In
     * the case of a "failed assertion", a corresponding exception is thrown. All
     * other cases will be passed unchanged.
     */
    void Context::try_post_result(const Result& result)
    {
        // "Successful presumption"? Do nothing.
        if ( result.origin_type == Result::OriginType::Presumption && result.is_success() )
            return;

        // "Failed assertion"? Throw exception instead.
        if ( result.origin_type == Result::OriginType::Assertion && result.is_failure() )
            throw exceptions::assertion_exception( result.original_expression, result.evaluated_expression, result.meta.stringification, result.meta.file_path, result.meta. line_number );

        // Forward all other cases.
        reporter->post_result( result );
    }



    // #############################################################################
    // IListener Implementations
    // #############################################################################

    /**
     * Handles the start of a new test run, providing the total number of test cases
     * (multiple executions/branches due to subcases etc. are not taken into account
     * here).
     */
    void Context::on_start_run(size_t count)
    {
        // Nothing to do here.
    }

    /**
     * Handles the start of the specified test case.
     */
    void Context::on_start_case(const Meta& meta)
    {
        // Nothing to do here.
    }

    /**
     * Handles the specified result.
     */
    void Context::on_result(const Result& result)
    {
        stats.update( result );
    }

    /**
     * Handles the specified (test case) stats at the end of a test case.
     */
    void Context::on_end_case(const Stats& stats)
    {
        // Nothing to do here.
    }

    /**
     * Handles the specified (final) stats at the end of a test run.
     */
    void Context::on_end_run(const Stats& stats)
    {
        // Nothing to do here.
    }



} // namespace tl7
