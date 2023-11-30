#include "errors.h"



namespace xl7 {
namespace errors {



    cl7::string directx_result(long error_code)
    {
        // TODO: DXERR or something?
        return cl7::errors::system_result( static_cast<unsigned long>( error_code ) );
    }

    cl7::string directx_result(long error_code, const cl7::string_view& context)
    {
        return cl7::errors::with_context( directx_result( error_code ), context );
    }



} // namespace errors
} // namespace xl7
