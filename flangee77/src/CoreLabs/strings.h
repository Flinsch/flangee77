#pragma once
#ifndef CL7_STRINGS_H
#define CL7_STRINGS_H

#include <CoreLabs/string.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>

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



    enum class Encoding
    {
        Unknown,

        ASCII,
        Latin1,
        UTF8,
        UTF16,
        UTF32,

#if WCHAR_MAX == UINT16_MAX
        UTFx = UTF16,
#elif WCHAR_MAX == UINT32_MAX
        UTFx = UTF32,
#endif

#ifdef UNICODE
        Default = UTFx,
#else
        Default = Latin1,
#endif
    };



    astring to_ascii(astring_view as);
    astring to_ascii(u8string_view u8s);
    astring to_ascii(u16string_view u16s);
    astring to_ascii(u32string_view u32s);
    astring to_ascii(wstring_view ws);

    astring to_latin1(astring_view as);
    astring to_latin1(u8string_view u8s);
    astring to_latin1(u16string_view u16s);
    astring to_latin1(u32string_view u32s);
    astring to_latin1(wstring_view ws);

    u8string to_utf8(astring_view as);
    u8string to_utf8(u8string_view u8s);
    u8string to_utf8(u16string_view u16s);
    u8string to_utf8(u32string_view u32s);
    u8string to_utf8(wstring_view ws);

    u16string to_utf16(astring_view as);
    u16string to_utf16(u8string_view u8s);
    u16string to_utf16(u16string_view u16s);
    u16string to_utf16(u32string_view u32s);
    u16string to_utf16(wstring_view ws);

    u32string to_utf32(astring_view as);
    u32string to_utf32(u8string_view u8s);
    u32string to_utf32(u16string_view u16s);
    u32string to_utf32(u32string_view u32s);
    u32string to_utf32(wstring_view ws);

    wstring to_utfx(astring_view as);
    wstring to_utfx(u8string_view u8s);
    wstring to_utfx(u16string_view u16s);
    wstring to_utfx(u32string_view u32s);
    wstring to_utfx(wstring_view ws);

    string from_ascii(astring_view as);
    string from_latin1(astring_view as);
    string from_utf8(u8string_view u8s);
    string from_utf16(u16string_view u16s);
    string from_utf32(u32string_view u32s);
    string from_utfx(wstring_view ws);

    astring to_ascii(byte_view bys);
    astring to_latin1(byte_view bys);
    u8string to_utf8_unchecked(byte_view bys);
    u16string to_utf16_unchecked(byte_view bys);
    u32string to_utf32_unchecked(byte_view bys);
    wstring to_utfx_unchecked(byte_view bys);

    string from_bytes(byte_view bys);

    byte_vector to_bytes(astring_view as);
    byte_vector to_bytes(u8string_view u8s, bool add_bom = false);
    byte_vector to_bytes(u16string_view u16s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(u32string_view u32s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(wstring_view ws, bool add_bom = false, std::endian endian = std::endian::native);

    bool check_ascii(astring_view as, bool log_warning = false);
    bool parse_utf8(u8string_view u8s, u32string& u32s, bool log_warning = false);
    bool parse_utf16(u16string_view u16s, u32string& u32s, bool log_warning = false);
    bool check_utf32(u32string_view u32s, bool log_warning = false);

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(u8string_view u8s);
    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(u16string_view u16s);

    Encoding detect_encoding(byte_view bys);



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



    template <class Tstring, typename Tchar, typename Tval>
    Tstring to_hex(Tval val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
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

    template <class Tstring, typename Tchar, typename Tval>
    Tstring to_0xhex(Tval val, Tchar ca = Tchar('A'), unsigned pad_zeros = 0)
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



    /**
     * Calculates the Levenshtein distance between two strings. The difference is
     * calculated in terms of characters, not in terms of (Unicode) code points.
     */
    template <class Tstring_view>
    size_t levenshtein(const Tstring_view& s1, const Tstring_view& s2)
    {
        const size_t size1 = s1.size();
        const size_t size2 = s2.size();
        if ( size1 == 0 ) return size2;
        if ( size2 == 0 ) return size1;

        const size_t len = size2 + 1;
        const size_t total_size = (size1 + 1) * (size2 + 1);

        auto mat = std::make_unique_for_overwrite<size_t[]>( total_size );

        for ( size_t i = 0; i <= size1; ++i )
            mat[ i * len + 0 ] = i;
        for ( size_t j = 0; j <= size2; ++j )
            mat[ 0 * len + j ] = j;

        for ( size_t i = 1; i <= size1; ++i )
        {
            for ( size_t j = 1; j <= size2; ++j )
            {
                const size_t cost = s1[ i - 1 ] == s2[ j - 1 ] ? 0 : 1;
                mat[ i * len + j ] = (std::min)( { 
                    mat[ (i - 1) * len + (j) ] + 1,
                    mat[ (i) * len + (j - 1) ] + 1,
                    mat[ (i - 1) * len + (j - 1) ] + cost,
                } );
            } // for ... j
        } // for ... i

        return mat[ size1 * len + size2 ];
    }

    /**
     * Calculates a normalized Levenshtein distance between two strings on a single
     * scale from 0 ("identical") to 1 ("nothing in common"). The difference is
     * calculated in terms of characters, not in terms of (Unicode) code points.
     */
    template <class Tstring_view, typename Tfloat = float>
        requires( std::is_floating_point_v<Tfloat> )
    Tfloat levenshtein_normalized(const Tstring_view& s1, const Tstring_view& s2)
    {
        const size_t size = (std::max)( s1.size(), s2.size() );
        if ( size == 0 ) return 0.0f;
        return static_cast<Tfloat>( levenshtein( s1, s2 ) ) / static_cast<Tfloat>( size );
    }



} // namespace strings
} // namespace cl7

#endif // CL7_STRINGS_H
