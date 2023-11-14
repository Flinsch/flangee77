#include "Stats.h"



namespace tl7 {



    Stats::Group Stats::Group::operator + (const Group& rhs) const
    {
        Group that = *this;
        that += rhs;
        return that;
    }

    Stats::Group& Stats::Group::operator += (const Group& rhs)
    {
        total_count += rhs.total_count;
        pass_count += rhs.pass_count;
        fail_count += rhs.fail_count;

        return *this;
    }

    void Stats::Group::reset()
    {
        total_count = 0;
        pass_count = 0;
        fail_count = 0;
    }

    void Stats::Group::update(bool success)
    {
        ++total_count;
        success ? ++pass_count : ++fail_count;
    }



    Stats Stats::operator + (const Stats& rhs) const
    {
        Stats that = *this;
        that += rhs;
        return that;
    }

    Stats& Stats::operator += (const Stats& rhs)
    {
        tests += rhs.tests;
        checks += rhs.checks;
        assertions += rhs.assertions;

        exception_count += rhs.exception_count;
        warning_count += rhs.warning_count;
        execution_time_msecs += rhs.execution_time_msecs;

        return *this;
    }

    void Stats::reset()
    {
        ::memset( this, 0, sizeof(*this) );
    }

    void Stats::update(const Result& result)
    {
        switch ( result.origin_type )
        {
        case Result::OriginType::TestCase:
            tests.update( result.is_success() );
            break;
        case Result::OriginType::Check:
            checks.update( result.is_success() );
            break;
        case Result::OriginType::Assertion:
            assertions.update( result.is_success() );
            break;
        case Result::OriginType::Exception:
            assert( result.is_failure() );
            ++exception_count;
            break;
        case Result::OriginType::Presumption:
            if ( result.is_failure() ) ++warning_count;
            break;
        }
    }



} // namespace tl7
