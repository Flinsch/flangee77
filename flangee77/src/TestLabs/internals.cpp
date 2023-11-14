#include "internals.h"



namespace tl7 {
namespace internals {



    cl7::string to_string(const std::string& val)
    {
        return to_string( std::string_view( val ) );
    }

    cl7::string to_string(const std::string_view& val)
    {
        cl7::string s( val.size() + 2, '"' );
        for ( size_t i = 0; i < val.size(); ++i )
            s[ i + 1 ] = val[ i ];
        return s;
    }

#ifdef UNICODE
    cl7::string to_string(const std::wstring& val)
    {
        return to_string( std::wstring_view( val ) );
    }

    cl7::string to_string(const std::wstring_view& val)
    {
        cl7::string s( val.size() + 2, L'"' );
        for ( size_t i = 0; i < val.size(); ++i )
            s[ i + 1 ] = val[ i ];
        return s;
    }
#endif



} // namespace internals
} // namespace tl7
