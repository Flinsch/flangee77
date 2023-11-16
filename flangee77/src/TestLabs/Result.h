#pragma once
#ifndef TL7_RESULT_H
#define TL7_RESULT_H

#include "./Meta.h"



namespace tl7 {



struct Result
{

public:
    enum class OriginType
    {
        Unknown,
        TestCase,
        Check,
        Presumption,
        Assertion,
        Exception,
    }; // enum class OriginType

    enum class Outcome
    {
        Failure = 0,
        Success = -1,
    }; // enum class Outcome

    static Outcome make_outcome(bool success) { return success ? Outcome::Success : Outcome::Failure; }



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    Result();



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /**
     * The test result's origin type.
     */
    OriginType origin_type;

    /**
     * The outcome of the test.
     */
    Outcome outcome;

    /**
     * The original expression (or empty if not applicable).
     */
    cl7::string original_expression;

    /**
     * The evaluated expression (or empty if not applicable).
     */
    cl7::string evaluated_expression;

    /**
     * The "stringification" of the underlying input data, of a subcase iteration
     * for example (or NULL if not applicable).
     */
    std::shared_ptr<cl7::string> data_string;

    /**
     * The meta description of the respective test case/subcase branch.
     */
    Meta context_meta;

    /**
     * The meta description of what the result is about and where it has been
     * generated.
     */
    Meta result_meta;



    // #############################################################################
    // Derived Properties
    // #############################################################################
public:
    bool is_success() const { return outcome == Outcome::Success; }
    bool is_failure() const { return outcome == Outcome::Failure; }

}; // struct Result



} // namespace tl7

#endif // TL7_RESULT_H
