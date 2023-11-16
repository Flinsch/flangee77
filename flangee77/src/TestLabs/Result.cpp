#include "Result.h"



namespace tl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    Result::Result()
        : origin_type( OriginType::Unknown )
        , outcome( Outcome::Failure )
        , original_expression()
        , evaluated_expression()
        , data_string()
        , context_meta{ {}, TEXT(""), 0 }
        , result_meta{ {}, TEXT(""), 0 }
    {
    }



} // namespace tl7
