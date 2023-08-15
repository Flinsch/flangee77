#pragma once
#ifndef CL7_STRINGS_H
#define CL7_STRINGS_H

#include <CoreLabs/string.h>



namespace cl7 {
namespace strings {



    template <class Tstring>
    void ltrim(Tstring& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](decltype(s[0]) c) {
            return !std::isspace(c);
        }));
    }

    template <class Tstring>
    void rtrim(Tstring& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](decltype(s[0]) c) {
            return !std::isspace(c);
        }).base(), s.end());
    }

    template <class Tstring>
    void trim(Tstring& s)
    {
        rtrim( s );
        ltrim( s );
    }



    template <class Tstring>
    Tstring ltrimmed(Tstring s)
    {
        ltrim( s );
        return s;
    }

    template <class Tstring>
    Tstring rtrimmed(Tstring s)
    {
        rtrim( s );
        return s;
    }

    template <class Tstring>
    Tstring trimmed(Tstring s)
    {
        trim( s );
        return s;
    }



    template <class Tstring, typename Tchar>
    void lpad(Tstring& s, size_t min_length, Tchar c)
    {
        if ( min_length > s.length() )
            s.insert( s.begin(), min_length - s.length(), c );
    }

    template <class Tstring, typename Tchar>
    void rpad(Tstring& s, size_t min_length, Tchar c)
    {
        if ( min_length > s.length() )
            s.append( min_length - s.length(), c );
    }



    template <class Tstring>
    Tstring lpadded(Tstring s)
    {
        lpad( s );
        return s;
    }

    template <class Tstring>
    Tstring rpadded(Tstring s)
    {
        rpad( s );
        return s;
    }



} // namespace strings
} // namespace cl7

#endif // CL7_STRINGS_H
