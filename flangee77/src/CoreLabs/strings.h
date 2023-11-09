#pragma once
#ifndef CL7_STRINGS_H
#define CL7_STRINGS_H

#include <CoreLabs/string.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_span.h>

#include <bit>



/**
 * In addition to what I have already said (see string.h), let me say a few words
 * regarding the functions provided here:
 * 
 * Within the given framework we want to provide some basic functionality to deal
 * with different encodings and Unicode transformation formats and to convert
 * between them. We do not want to reinvent the wheel or to push it further than
 * necessary, so we really only limit ourselves to selected aspects that the
 * framework uses internally itself, without having to include external libraries.
 * If you need more extensive or robust functionality, you could/should include and
 * use 3rd party libraries in your projects yourself, like International Components
 * for Unicode (ICU) or Boost.Locale, for example.
 */



namespace cl7 {
namespace strings {



    astring to_ascii(const astring_view& as);
    astring to_ascii(const u8string_view& u8s);
    astring to_ascii(const u16string_view& u16s);
    astring to_ascii(const u32string_view& u32s);
    astring to_ascii(const wstring_view& ws);

    astring to_latin1(const astring_view& as);
    astring to_latin1(const u8string_view& u8s);
    astring to_latin1(const u16string_view& u16s);
    astring to_latin1(const u32string_view& u32s);
    astring to_latin1(const wstring_view& ws);

    u8string to_utf8(const astring_view& as);
    u8string to_utf8(const u8string_view& u8s);
    u8string to_utf8(const u16string_view& u16s);
    u8string to_utf8(const u32string_view& u32s);
    u8string to_utf8(const wstring_view& ws);

    u16string to_utf16(const astring_view& as);
    u16string to_utf16(const u8string_view& u8s);
    u16string to_utf16(const u16string_view& u16s);
    u16string to_utf16(const u32string_view& u32s);
    u16string to_utf16(const wstring_view& ws);

    u32string to_utf32(const astring_view& as);
    u32string to_utf32(const u8string_view& u8s);
    u32string to_utf32(const u16string_view& u16s);
    u32string to_utf32(const u32string_view& u32s);
    u32string to_utf32(const wstring_view& ws);

    wstring to_utfx(const astring_view& as);
    wstring to_utfx(const u8string_view& u8s);
    wstring to_utfx(const u16string_view& u16s);
    wstring to_utfx(const u32string_view& u32s);
    wstring to_utfx(const wstring_view& ws);

    string from_ascii(const astring_view& as);
    string from_latin1(const astring_view& as);
    string from_utf8(const u8string_view& u8s);
    string from_utf16(const u16string_view& u16s);
    string from_utf32(const u32string_view& u32s);
    string from_utfx(const wstring_view& ws);

    astring to_ascii(const byte_span& bys);
    astring to_latin1(const byte_span& bys);
    u8string to_utf8_unchecked(const byte_span& bys);
    u16string to_utf16_unchecked(const byte_span& bys);
    u32string to_utf32_unchecked(const byte_span& bys);
    wstring to_utfx_unchecked(const byte_span& bys);

    string from_bytes(const byte_span& bys);

    byte_vector to_bytes(const astring_view& as);
    byte_vector to_bytes(const u8string_view& u8s, bool add_bom = false);
    byte_vector to_bytes(const u16string_view& u16s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(const u32string_view& u32s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(const wstring_view& ws, bool add_bom = false, std::endian endian = std::endian::native);

    bool check_ascii(const astring_view& as, bool log_warning = false);
    bool parse_utf8(const u8string_view& u8s, u32string& u32s, bool log_warning = false);
    bool parse_utf16(const u16string_view& u16s, u32string& u32s, bool log_warning = false);
    bool check_utf32(const u32string_view& u32s, bool log_warning = false);

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(const u8string_view& u8s);
    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(const u16string_view& u16s);



    template <typename Tchar>
    bool is_whitespace(Tchar c)
    {
        // https://www.unicode.org/Public/UCD/latest/ucd/PropList.txt
        const auto u32c = static_cast<u32char_type>( c );
        return 
            (u32c >= 0x0009 && u32c <= 0x000d) ||   // tab ... carriage return
            u32c == 0x0020 ||                       // space
            u32c == 0x0085 ||                       // next line
            u32c == 0x00a0 ||                       // no-break space
            u32c == 0x1680 ||                       // Ogham space mark
            (u32c >= 0x2000 && u32c <= 0x200a) ||   // en quad ... hair space
            u32c == 0x2028 ||                       // line separator
            u32c == 0x2029 ||                       // paragraph separator
            u32c == 0x202f ||                       // narrow no-break space
            u32c == 0x205f ||                       // medium mathematical space
            u32c == 0x3000;                         // ideographic space
        return true;

        static_assert( !std::is_same<Tchar, u8char_type>::value, "Not implemented for UTF-8 and its variable-length character encoding." );
    }

    template <> inline
    bool is_whitespace(achar_type c)
    {
        return 
            (c >= 0x0009 && c <= 0x000d) || // tab ... carriage return
            c == 0x0020 ||                  // space
            c == 0x0085 ||                  // next line
            c == 0x00a0;                    // no-break space
    }



    template <class Tstring>
    void ltrim(Tstring& s)
    {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](decltype(s[0]) c) {
            return !is_whitespace(c);
        }));
    }

    template <class Tstring>
    void rtrim(Tstring& s)
    {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](decltype(s[0]) c) {
            return !is_whitespace(c);
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



    template <class Tstring, typename Tchar>
    Tstring lpadded(Tstring s, size_t min_length, Tchar c)
    {
        lpad( s, min_length, c );
        return s;
    }

    template <class Tstring, typename Tchar>
    Tstring rpadded(Tstring s, size_t min_length, Tchar c)
    {
        rpad( s, min_length, c );
        return s;
    }



    template <class Tstring, typename Tchar>
    Tstring to_hex(unsigned long long val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
    {
        static constexpr auto c0 = Tchar('0');
        if ( val == 0 )
            return pad_zeros ? Tstring( pad_zeros, c0 ) : Tstring();
        Tstring s;
        s.reserve( 16 );
        while ( val )
        {
            Tchar x = static_cast<Tchar>( val & 0xf );
            val >>= 4;
            Tchar c = x < 0xa ? c0+x : ca+x-0xa;
            s += c;
        }
        rpad( s, pad_zeros, c0 );
        std::reverse( s.begin(), s.end() );
        return s;
    }

    template <class Tstring, typename Tchar>
    Tstring to_0xhex(unsigned long long val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
    {
        static constexpr auto c0 = Tchar('0');
        static constexpr auto cx = Tchar('x');
        Tstring s = to_hex<Tstring>( val, ca, pad_zeros );
        s.insert( s.begin(), 2, c0 );
        s[ 1 ] = cx;
        return s;
    }

    string to_hex(unsigned long long val, char_type ca = TEXT('A'), unsigned pad_zeros = 0);
    string to_0xhex(unsigned long long val, char_type ca = TEXT('A'), unsigned pad_zeros = 0);



} // namespace strings
} // namespace cl7

#endif // CL7_STRINGS_H
