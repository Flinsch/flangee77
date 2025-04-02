#ifndef TL7_RESULT_H
#define TL7_RESULT_H

#include "./Meta.h"



namespace tl7 {



struct Result
{

    enum struct OriginType
    {
        Unknown,
        TestCase,
        Check,
        Presumption,
        Assertion,
        Exception,
    };

    enum struct Outcome
    {
        Failure = 0,
        Success = -1,
    };



    /**
     * The test result's origin type.
     */
    OriginType origin_type = OriginType::Unknown;

    /**
     * The outcome of the test.
     */
    Outcome outcome = Outcome::Failure;

    /**
     * The original expression (or empty if not applicable).
     */
    cl7::u8string original_expression;

    /**
     * The evaluated expression (or empty if not applicable).
     */
    cl7::u8string evaluated_expression;

    /**
     * The "stringification" of the underlying input data, of a subcase iteration
     * for example (or NULL if not applicable).
     */
    std::shared_ptr<cl7::u8string> data_string;

    /**
     * The meta description of the respective test case/subcase branch.
     */
    Meta context_meta{{}, "", 0};

    /**
     * The meta description of what the result is about and where it has been
     * generated.
     */
    Meta result_meta{{}, "", 0};



    Result() = default;



    bool is_success() const { return outcome == Outcome::Success; }
    bool is_failure() const { return outcome == Outcome::Failure; }

    static Outcome make_outcome(bool success) { return success ? Outcome::Success : Outcome::Failure; }

}; // struct Result



} // namespace tl7

#endif // TL7_RESULT_H
