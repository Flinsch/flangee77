#pragma once
#ifndef TL7_CONTEXT_H
#define TL7_CONTEXT_H
#include "./reporting/IListener.h"

#include "./reporting/Reporter.h"

#include "./SubcaseContext.h"
#include "./Meta.h"
#include "./Result.h"
#include "./Stats.h"



namespace tl7 {



struct Context
    : public reporting::IListener
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Context() = delete;

    Context(reporting::Reporter* reporter, const Meta* root_meta);

    Context(const Context&) = delete;
    Context& operator=(const Context&) = delete;
    Context(Context&&) = delete;
    Context& operator=(Context&&) = delete;

    ~Context() override;



    // #############################################################################
    // IListener Implementations
    // #############################################################################

    /**
     * Handles the start of a new test run, providing the total number of test cases
     * (multiple executions/branches due to subcases etc. are not taken into account
     * here).
     */
    void on_start_run(size_t count) override;

    /**
     * Handles the start of the specified test case.
     */
    void on_start_case(const Meta& meta) override;

    /**
     * Handles the specified result.
     */
    void on_result(const Result& result) override;

    /**
     * Handles the specified (test case) stats at the end of a test case.
     */
    void on_end_case(const Stats& stats) override;

    /**
     * Handles the specified (final) stats at the end of a test run.
     */
    void on_end_run(const Stats& stats) override;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Publishes the specified result by forwarding it to the reporter, with a few
     * exceptions: In the case of a "successful presumption", nothing happens. In
     * the case of a "failed assertion", a corresponding exception is thrown. All
     * other cases will be passed unchanged.
     */
    void try_post_result(const Result& result) const;



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The reporter with which the context will be registered as a listener during
     * its lifetime.
     */
    reporting::Reporter* const reporter;

    /**
     * The meta description of the current test case as root.
     */
    const Meta* const root_meta;

    /**
     * The (state of) subcases that are traversed sequentially (if any).
     */
    SubcaseContext subcases;

    /**
     * The (aggregated) statistics about tests carried out within this context.
     */
    Stats stats;

}; // struct Context



} // namespace tl7

#endif // TL7_CONTEXT_H
