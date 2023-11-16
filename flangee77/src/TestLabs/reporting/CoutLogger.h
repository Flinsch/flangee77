#pragma once
#ifndef TL7_REPORTING_COUTLOGGER_H
#define TL7_REPORTING_COUTLOGGER_H
#include "./IListener.h"

#include "../Result.h"
#include "../Stats.h"

#include <vector>
#include <map>



namespace tl7 {
namespace reporting {



class CoutLogger
    : public IListener
{

    // #############################################################################
    // Attributes
    // #############################################################################
private:
    bool _defer_result_output = true;

    size_t _total_count = 0;
    size_t _current_index = 0;

    std::map<Meta, std::vector<Result>> _buffered_results;

    const Meta* _current_meta = nullptr;
    const Meta* _logged_meta = nullptr;
    size_t _carriage_position = 0;
    bool _continue = false;



    // #############################################################################
    // IListener Implementations
    // #############################################################################
public:
    /**
     * Handles the start of a new test run, providing the total number of test cases
     * (multiple executions/branches due to subcases etc. are not taken into account
     * here).
     */
    virtual void on_start_run(size_t count) override;

    /**
     * Handles the start of the specified test case.
     */
    virtual void on_start_case(const Meta& meta) override;

    /**
     * Handles the specified result.
     */
    virtual void on_result(const Result& result) override;

    /**
     * Handles the specified (test case) stats at the end of a test case.
     */
    virtual void on_end_case(const Stats& stats) override;

    /**
     * Handles the specified (final) stats at the end of a test run.
     */
    virtual void on_end_run(const Stats& stats) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Buffers the specified result for later output.
     */
    void _buffer_result(const Result& result);

    /**
     * Logs (and clears) the buffered results.
     */
    void _log_results();

    /**
     * Logs the specified results.
     */
    void _log_results(const std::vector<Result>& results);

    /**
     * Logs the specified result.
     */
    void _log_result(const Result& result);

    /**
     * Logs the specified (final) stats (at the end of a test run).
     */
    void _log_final_stats(const Stats& stats);

    /**
     * Logs the meta description of the specified test case (called when the first
     * problem occurs within said test case).
     */
    void _log_test_case(const Meta& meta);

    /**
     * 
     */
    void _log_signature(const Signature& signature);

}; // class CoutLogger



} // namespace reporting
} // namespace tl7

#endif // TL7_REPORTING_COUTLOGGER_H
