#include "Result.h"



namespace tl7 {



    Result::Result()
        : origin_type(OriginType::Unknown)
        , outcome(Outcome::Failure)
        , context_meta{{}, TEXT(""), 0}
        , result_meta{{}, TEXT(""), 0}
    {
    }



} // namespace tl7
