#pragma once
#ifndef CL7_STRING_H
#define CL7_STRING_H

#include <CoreLabs/root.h>

#include <string>



#if defined(UNICODE) ^ defined(_UNICODE)
#error Inconsistent UNICODE and _UNICODE definition.
#endif



namespace cl7 {



#ifdef UNICODE
    typedef wchar_t char_type;
    typedef std::wstring string;
    typedef std::wstring_view string_view;
#else // => !UNICODE
    typedef char char_type;
    typedef std::string string;
    typedef std::string_view string_view;
#endif // #else => !UNICODE

#ifdef UNICODE
    template <typename T>
    string to_string(const T& val) { return std::to_wstring( val ); }
#else // => !UNICODE
    string to_string(const T& val) { return std::to_string( val ); }
#endif // #else => !UNICODE

#ifdef  UNICODE                     
#define __TEXT(quote) L##quote      
#define __RTEXT(quote) LR##quote    
#else   /* UNICODE */               
#define __TEXT(quote) quote         
#define __RTEXT(quote) R##quote     
#endif /* UNICODE */                
#define TEXT(quote) __TEXT(quote)   
#define RTEXT(quote) __RTEXT(quote) 



    typedef char cchar_type;
    typedef wchar_t wchar_type;

    typedef std::string cstring;
    typedef std::wstring wstring;

    typedef std::string_view cstring_view;
    typedef std::wstring_view wstring_view;

    template <typename T>
    cstring to_cstring(const T& val) { return std::to_string( val ); }
    template <typename T>
    wstring to_wstring(const T& val) { return std::to_wstring( val ); }



} // namespace cl7

#endif // CL7_STRING_H
