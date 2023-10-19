#include "strings.h"

#include <CoreLabs/logging.h>



// Make sure a wchar is either 2 bytes (UTF-16) or 4 bytes (UTF-32).
static_assert( sizeof(cl7::wchar_type) == 2 || sizeof(cl7::wchar_type) == 4 );

// Additionally, make sure that the limits are defined properly so that we can
// use them for case distinctions at compile time (where we cannot use sizeof).
static_assert( WCHAR_MAX == UINT16_MAX || WCHAR_MAX == UINT32_MAX );

// Verify byte order mark (BOM).
// This check is actually a "tautology" and should pass on all systems. But
// it's good for our psyche to simply leave it here; it doesn't do any harm.
static_assert( cl7::u32char_type(U'\ufeff') == 0xfeff );



namespace cl7 {
namespace strings {



    constexpr unsigned _replacement_ascii = 0x1a;//0x20;
    constexpr unsigned _replacement_unicode = 0xfffd;



    void _try_log_warning(bool log_warning, const cl7::string_view& message)
    {
        if ( log_warning )
            LOG_WARNING( message );
    }

    bool _try_log_warning_always_return_false(bool log_warning, const cl7::string_view& message)
    {
        _try_log_warning( log_warning, message );
        return false;
    }

    achar_type _check_adjust_latin1(u32char_type u32c, bool log_warning = false)
    {
        if ( u32c > 0xff )
        {
            _try_log_warning( log_warning, TEXT("Invalid ASCII/Latin-1 character: ") + to_0xhex(u32c) + TEXT(" (") + to_string(uint32_t(u32c)) + TEXT(")") );
            u32c = _replacement_ascii;
        }

        return static_cast<achar_type>( u32c );
    }

    u32char_type _check_adjust_unicode(u32char_type u32c, bool log_warning = false)
    {
        if ( u32c > 0x10ffff )
        {
            _try_log_warning( log_warning, TEXT("Invalid Unicode code point: ") + to_0xhex(u32c) );
            u32c = _replacement_unicode;
        }
        else if ( u32c >= 0xd800 && u32c <= 0xdfff )
        {
            _try_log_warning( log_warning, TEXT("Unpaired Unicode surrogate: ") + to_0xhex(u32c) );
            // Just pass the code point unchanged. This might even "make sense":
            // Windows allows unpaired surrogates in filenames and other places,
            // which generally means they have to be supported by software in
            // spite of their exclusion from the Unicode standard. Furthermore,
            // UCS-2, UTF-8, and UTF-32 can encode these code points in trivial
            // and obvious ways, and a large amount of software does so, even
            // though the standard states that such arrangements should be treated
            // as encoding errors. Well, at least we generated an error message.
            // And be prepared for further error messages when converting back and
            // forth between different transformation formats.
        }

        return u32c;
    }



    astring to_ascii(const astring_view& as)
    {
        check_ascii( as, true );
        return astring(as);
    }

    astring to_ascii(const u8string_view& u8s)
    {
        return to_ascii( to_latin1( u8s ) );
    }

    astring to_ascii(const u16string_view& u16s)
    {
        return to_ascii( to_latin1( u16s ) );
    }

    astring to_ascii(const u32string_view& u32s)
    {
        return to_ascii( to_latin1( u32s ) );
    }

    astring to_ascii(const wstring_view& ws)
    {
        return to_ascii( to_latin1( ws ) );
    }

    astring to_latin1(const astring_view& as)
    {
        return astring(as);
    }

    astring to_latin1(const u8string_view& u8s)
    {
        u32string u32s;
        parse_utf8( u8s, u32s, true );
        return to_latin1( u32s );
    }

    astring to_latin1(const u16string_view& u16s)
    {
        u32string u32s;
        parse_utf16( u16s, u32s, true );
        return to_latin1( u32s );
    }

    astring to_latin1(const u32string_view& u32s)
    {
        astring as( u32s.size(), achar_type(0) );

        for ( size_t i = 0; i < u32s.size(); ++i )
        {
            as[ i ] = _check_adjust_latin1( u32s[ i ], true );
        }

        return as;
    }

    astring to_latin1(const wstring_view& ws)
    {
#if WCHAR_MAX == UINT16_MAX
        return to_latin1( u16string_view( reinterpret_cast<const u16char_type*>(ws.data()), ws.size() ) );
#elif WCHAR_MAX == UINT32_MAX
        return to_latin1( u32string_view( reinterpret_cast<const u32char_type*>(ws.data()), ws.size() ) );
#else
        static_assert( false );
#endif
    }

    u8string to_utf8(const astring_view& as)
    {
        return to_utf8( from_latin1( as ) );
    }

    u8string to_utf8(const u8string_view& u8s)
    {
        return u8string(u8s);
    }

    u8string to_utf8(const u16string_view& u16s)
    {
        return to_utf8( from_utf16( u16s ) );
    }

    u8string to_utf8(const u32string_view& u32s)
    {
        size_t size = 0;
        for ( u32char_type u32c : u32s )
        {
            u32c = _check_adjust_unicode( u32c );

            if ( u32c <= 0x7f )
                size += 1;
            else if ( u32c <= 0x7ff )
                size += 2;
            else if ( u32c <= 0xffff )
                size += 3;
            else
                size += 4;
        }

        u8string u8s( size, u8char_type(0) );

        size_t i = 0;
        for ( u32char_type u32c : u32s )
        {
            u32c = _check_adjust_unicode( u32c, true );

            if ( u32c <= 0x7f )
            {
                u8s[ i++ ] = u8char_type(u32c);
            }
            else if ( u32c <= 0x7ff )
            {
                u8s[ i++ ] = u8char_type(0xc0 | ((u32c >> 6) & 0x1f));
                u8s[ i++ ] = u8char_type(0x80 | (u32c & 0x3f));
            }
            else if ( u32c <= 0xffff )
            {
                u8s[ i++ ] = u8char_type(0xe0 | ((u32c >> 12) & 0x0f));
                u8s[ i++ ] = u8char_type(0x80 | ((u32c >> 6) & 0x3f));
                u8s[ i++ ] = u8char_type(0x80 | (u32c & 0x3f));
            }
            else
            {
                u8s[ i++ ] = u8char_type(0xf0 | ((u32c >> 18) & 0x07));
                u8s[ i++ ] = u8char_type(0x80 | ((u32c >> 12) & 0x3f));
                u8s[ i++ ] = u8char_type(0x80 | ((u32c >> 6) & 0x3f));
                u8s[ i++ ] = u8char_type(0x80 | (u32c & 0x3f));
            }
        } // for ...
        assert( i == size );

        return u8s;
    }

    u8string to_utf8(const wstring_view& ws)
    {
#if WCHAR_MAX == UINT16_MAX
        return to_utf8( u16string_view( reinterpret_cast<const u16char_type*>(ws.data()), ws.size() ) );
#elif WCHAR_MAX == UINT32_MAX
        return to_utf8( u32string_view( reinterpret_cast<const u32char_type*>(ws.data()), ws.size() ) );
#else
        static_assert( false );
#endif
    }

    u16string to_utf16(const astring_view& as)
    {
        return to_utf16( from_latin1( as ) );
    }

    u16string to_utf16(const u8string_view& u8s)
    {
        return to_utf16( from_utf8( u8s ) );
    }

    u16string to_utf16(const u16string_view& u16s)
    {
        return u16string(u16s);
    }

    u16string to_utf16(const u32string_view& u32s)
    {
        size_t size = 0;
        for ( u32char_type u32c : u32s )
        {
            u32c = _check_adjust_unicode( u32c );

            if ( u32c <= 0xffff )
                size += 1;
            else
                size += 2;
        }

        u16string u16s( size, u16char_type(0) );

        size_t i = 0;
        for ( u32char_type u32c : u32s )
        {
            u32c = _check_adjust_unicode( u32c, true );

            if ( u32c <= 0xffff )
            {
                u16s[ i++ ] = u16char_type(u32c);
            }
            else
            {
                u16s[ i++ ] = u16char_type(0xd800 | ((u32c >> 10) & 0x3ff));
                u16s[ i++ ] = u16char_type(0xdc00 | (u32c & 0x3ff));
            }
        } // for ...
        assert( i == size );

        return u16s;
    }

    u16string to_utf16(const wstring_view& ws)
    {
#if WCHAR_MAX == UINT16_MAX
        return to_utf16( u16string_view( reinterpret_cast<const u16char_type*>(ws.data()), ws.size() ) );
#elif WCHAR_MAX == UINT32_MAX
        return to_utf16( u32string_view( reinterpret_cast<const u32char_type*>(ws.data()), ws.size() ) );
#else
        static_assert( false );
#endif
    }

    u32string to_utf32(const astring_view& as)
    {
        u32string u32s( as.size(), u32char_type(0) );

        for ( size_t i = 0; i < as.size(); ++i )
        {
            u32s[ i ] = as[ i ]; // No check required because everything from 0 to 255 is valid anyway.
        }

        return u32s;
    }

    u32string to_utf32(const u8string_view& u8s)
    {
        u32string u32s;
        parse_utf8( u8s, u32s, true );
        return u32s;
    }

    u32string to_utf32(const u16string_view& u16s)
    {
        u32string u32s;
        parse_utf16( u16s, u32s, true );
        return u32s;
    }

    u32string to_utf32(const u32string_view& u32s)
    {
        return u32string(u32s);
    }

    u32string to_utf32(const wstring_view& ws)
    {
#if WCHAR_MAX == UINT16_MAX
        return to_utf32( u16string_view( reinterpret_cast<const u16char_type*>(ws.data()), ws.size() ) );
#elif WCHAR_MAX == UINT32_MAX
        return to_utf32( u32string_view( reinterpret_cast<const u32char_type*>(ws.data()), ws.size() ) );
#else
        static_assert( false );
#endif
    }

    wstring to_utfx(const astring_view& as)
    {
#if WCHAR_MAX == UINT16_MAX
        auto uxs = to_utf16( as );
#elif WCHAR_MAX == UINT32_MAX
        auto uxs = to_utf32( as );
#else
        static_assert( false );
#endif
        return wstring( wstring_view( reinterpret_cast<const wchar_type*>(uxs.data()), uxs.size() ) );
    }

    wstring to_utfx(const u8string_view& u8s)
    {
#if WCHAR_MAX == UINT16_MAX
        auto uxs = to_utf16( u8s );
#elif WCHAR_MAX == UINT32_MAX
        auto uxs = to_utf32( u8s );
#else
        static_assert( false );
#endif
        return wstring( wstring_view( reinterpret_cast<const wchar_type*>(uxs.data()), uxs.size() ) );
    }

    wstring to_utfx(const u16string_view& u16s)
    {
#if WCHAR_MAX == UINT16_MAX
        auto uxs = u16s;
#elif WCHAR_MAX == UINT32_MAX
        auto uxs = to_utf32( u16s );
#else
        static_assert( false );
#endif
        return wstring( wstring_view( reinterpret_cast<const wchar_type*>(uxs.data()), uxs.size() ) );
    }

    wstring to_utfx(const u32string_view& u32s)
    {
#if WCHAR_MAX == UINT16_MAX
        auto uxs = to_utf16( u32s );
#elif WCHAR_MAX == UINT32_MAX
        auto uxs = u32s;
#else
        static_assert( false );
#endif
        return wstring( wstring_view( reinterpret_cast<const wchar_type*>(uxs.data()), uxs.size() ) );
    }

    wstring to_utfx(const wstring_view& ws)
    {
        return wstring(ws);
    }

    string from_ascii(const astring_view& as)
    {
        return from_latin1( as );
    }

    string from_latin1(const astring_view& as)
    {
#ifdef UNICODE
        return to_utfx( as );
#else
        return string(as);
#endif
    }

    string from_utf8(const u8string_view& u8s)
    {
#ifdef UNICODE
        return to_utfx( u8s );
#else
        return to_latin1( u8s );
#endif
    }

    string from_utf16(const u16string_view& u16s)
    {
#ifdef UNICODE
        return to_utfx( u16s );
#else
        return to_latin1( u16s );
#endif
    }

    string from_utf32(const u32string_view& u32s)
    {
#ifdef UNICODE
        return to_utfx( u32s );
#else
        return to_latin1( u32s );
#endif
    }

    string from_utfx(const wstring_view& ws)
    {
#if WCHAR_MAX == UINT16_MAX
        return from_utf16( u16string_view( reinterpret_cast<const u16char_type*>(ws.data()), ws.size() ) );
#elif WCHAR_MAX == UINT32_MAX
        return from_utf32( u32string_view( reinterpret_cast<const u32char_type*>(ws.data()), ws.size() ) );
#else
        static_assert( false );
#endif
    }

    bool check_ascii(const astring_view& as, bool log_warning)
    {
        for ( achar_type ac : as )
        {
            if ( ac > 0x7f )
                return _try_log_warning_always_return_false( log_warning, TEXT("Invalid ASCII character: ") + to_0xhex(ac) + TEXT(" (") + to_string(uint8_t(ac)) + TEXT(")") );
        }

        return true;
    }

    bool parse_utf8(const u8string_view& u8s, u32string& u32s, bool log_warning)
    {
        u32s.resize( utf8_length( u8s ), u32char_type(0) );

        bool is_valid = true;

        unsigned continuation = 0;
        u32char_type u32c = 0;
        bool is_char_valid = false;

        size_t i = 0;
        for ( u8char_type u8c : u8s )
        {
            if ( continuation )
            {
                if ( u8c >= 0x80 && u8c <= 0xbf ) // (expected) continuation byte of a sequence
                {
                    // Everything's fine: merge byte into character "buffer".
                    u32c = (u32c << 6) | (u8c & 0x3f);
                }
                else // bad code unit
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false( log_warning, TEXT("Bad UTF-8 continuation byte: ") + to_0xhex(u8c) );
                }
                if ( --continuation == 0 ) // end of sequence
                {
                    if ( is_char_valid && u32c != _replacement_unicode && _check_adjust_unicode( u32c ) != _replacement_unicode ) // Play it safe because even valid constellations of specific sequences can lead to invalid code points.
                        u32s[ i++ ] = u32c;
                    else
                        u32s[ i++ ] = _replacement_unicode;
                }
            }
            else // => no sequence continuation (yet)
            {
                if ( u8c <= 0x7f ) // "regular" character (ASCII compatible)
                {
                    u32s[ i++ ] = static_cast<u32char_type>( u8c );
                }
                else if ( u8c >= 0xc0 && u8c <= 0xdf ) // start of 2-byte sequence
                {
                    if ( u8c < 0xc2 ) // invalid 2-byte sequence that represents code range from 0 to 127
                        is_valid = is_char_valid = _try_log_warning_always_return_false( log_warning, TEXT("Start of invalid 2-byte UTF-8 sequence representing code range from 0 to 127.") );
                    else
                        is_char_valid = true;
                    u32c = u8c & 0x1f;
                    continuation = 1;
                }
                else if ( u8c >= 0xe0 && u8c <= 0xef ) // start of 3-byte sequence
                {
                    is_char_valid = true;
                    u32c = u8c & 0x0f;
                    continuation = 2;
                }
                else if ( u8c >= 0xf0 && u8c <= 0xf7 ) // start of 4-byte sequence
                {
                    if ( u8c > 0xf4 ) // invalid 4-byte sequence that represents code range above 0x140000
                        is_valid = is_char_valid = _try_log_warning_always_return_false( log_warning, TEXT("Start of invalid 4-byte UTF-8 sequence representing code range above 0x140000.") );
                    else
                        is_char_valid = true;
                    u32c = u8c & 0x07;
                    continuation = 3;
                }
                else if ( u8c >= 0xf8 && u8c <= 0xfb ) // start of invalid 5-byte sequence
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false( log_warning, TEXT("Start of invalid 5-byte UTF-8 sequence.") );
                    u32c = u8c & 0x03;
                    continuation = 4;
                }
                else if ( u8c >= 0xfc && u8c <= 0xfd ) // start of invalid 6-byte sequence
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false( log_warning, TEXT("Start of invalid 6-byte UTF-8 sequence.") );
                    u32c = u8c & 0x01;
                    continuation = 5;
                }
                else if ( u8c >= 0x80 && u8c <= 0xbf ) // unexpected continuation byte of unknown sequence
                {
                    is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Unexpected UTF-8 continuation byte: ") + to_0xhex(u8c) );
                    u32s[ i++ ] = _replacement_unicode;
                }
                else // invalid code unit (probably 0xfe or 0xff, because we should have already covered all others)
                {
                    is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Invalid UTF-8 code unit: ") + to_0xhex(u8c) );
                    u32s[ i++ ] = _replacement_unicode;
                }
            } // no sequence continuation (yet)
        } // for ...
        if ( continuation )
        {
            is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Incomplete UTF-8 sequence: ") + to_string(continuation) + TEXT(" byte(s) missing") );
            u32s[ i++ ] = _replacement_unicode;
        }
        assert( i == u32s.size() );

        return is_valid;
    }

    bool parse_utf16(const u16string_view& u16s, u32string& u32s, bool log_warning)
    {
        u32s.resize( utf16_length( u16s ), u32char_type(0) );

        bool is_valid = true;

        bool surrogate = false;
        u16char_type prev = 0;

        size_t i = 0;
        for ( u16char_type u16c : u16s )
        {
            if ( surrogate )
            {
                if ( u16c >= 0xdc00 && u16c <= 0xdfff ) // (expected) trailing/low surrogate
                {
                    // Everything's fine: put the two surrogates back together.
                    u32s[ i++ ] = ((prev & 0x3ff) << 10) | (u16c & 0x3ff);
                }
                else if ( u16c >= 0xd800 && u16c <= 0xdbff ) // (another) leading/high surrogate
                {
                    is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Unpaired leading/high UTF-16 surrogates: ") + to_0xhex(prev) + TEXT(" ") + to_0xhex(u16c) );
                    u32s[ i++ ] = _replacement_unicode;
                }
                else // "regular" character (but invalid here)
                {
                    is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Unpaired leading/high UTF-16 surrogate: ") + to_0xhex(prev) );
                    // Consistently, we might also have to fall back to the replacement
                    // character here, but then we would "waste" the valid character we
                    // hold in our hands. So we just pass it in any case.
                    u32s[ i++ ] = static_cast<u32char_type>( u16c );
                }
                // Also in the case of a leading/high surrogate set the flag to
                // false as not to distort the skipping. In such case the structure
                // is messed up anyway, so we can't rely on the current code unit
                // being a valid leading/high surrogate either.
                surrogate = false;
            }
            else // => no surrogate (yet)
            {
                if ( u16c <= 0xd7ff || u16c >= 0xe000 ) // "regular" character
                {
                    u32s[ i++ ] = static_cast<u32char_type>( u16c );
                }
                else if ( u16c >= 0xd800 && u16c <= 0xdbff ) // leading/high surrogate
                {
                    // Everything's fine: remember the leading/high surrogate.
                    // This is the only case where we don't pass a character (yet).
                    prev = u16c;
                    surrogate = true;
                }
                else //if ( u16c >= 0xdc00 && u16c <= 0xdfff ) // (unexpected) trailing/low surrogate
                {
                    is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Unpaired trailing/low UTF-16 surrogate: ") + to_0xhex(u16c) );
                    u32s[ i++ ] = _replacement_unicode;
                }
            } // no surrogate (yet)
        } // for ...
        if ( surrogate )
        {
            is_valid = _try_log_warning_always_return_false( log_warning, TEXT("Unpaired leading/high UTF-16 surrogate: ") + to_0xhex(prev) );
            u32s[ i++ ] = _replacement_unicode;
        }
        assert( i == u32s.size() );

        return is_valid;
    }

    bool check_utf32(const u32string_view& u32s, bool log_warning)
    {
        for ( u32char_type u32c : u32s )
        {
            if ( u32c > 0x10ffff || (u32c >= 0xd800 && u32c <= 0xdfff) )
                return _try_log_warning_always_return_false( log_warning, TEXT("Invalid Unicode code point: ") + to_0xhex(u32c) );
        }

        return true;
    }

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(const u8string_view& u8s)
    {
        const size_t size = u8s.size();
        size_t len = 0;

        for ( size_t i = 0; i < size; ++i, ++len )
        {
            const u8char_type u8c = u8s[ i ];
            if ( u8c >= 0xc0 && u8c <= 0xdf ) // 0xc0 and 0xc1 are actually invalid, but the following byte still has to be skipped.
                ++i; // Skip 1 continuation byte of a 2-byte sequence.
            else if ( u8c >= 0xe0 && u8c <= 0xef )
                i += 2; // Skip 2 continuation bytes of a 3-byte sequence.
            else if ( u8c >= 0xf0 && u8c <= 0xf7 ) // 0xf5 to 0xf7 are actually invalid, but the following 3 bytes still have to be skipped.
                i += 3; // Skip 3 continuation bytes of a 4-byte sequence.
            else if ( u8c >= 0xf8 && u8c <= 0xfb )
                i += 4; // Invalid, but the following 4 bytes still have to be skipped.
            else if ( u8c >= 0xfc && u8c <= 0xfd )
                i += 5; // Invalid, but the following 5 bytes still have to be skipped.

            // We don't need to check (other) invalid sequences because they would
            // be represented as a single replacement character. In terms of code
            // points, this case would be identical to the regular case of a single
            // valid character between 0x00 and 0x7f.
        }

        return len;
    }

    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(const u16string_view& u16s)
    {
        const size_t size = u16s.size();
        size_t len = 0;

        for ( size_t i = 0; i < size; ++i, ++len )
        {
            const u16char_type u16c = u16s[ i ];
            if ( u16c >= 0xd800 && u16c <= 0xdbff )
                ++i; // Skip trailing/low surrogate of a surrogate pair.

            // We don't need to check unpaired surrogates because they would be
            // represented as a single replacement character. In terms of code
            // points, this case would be identical to the regular case of a single
            // valid character between 0x0000 and 0xd7ff or 0xe000 and 0xffff
            // (specifically between 0xe000 and 0xffff because the replacement
            // character is 0xfffd, whatever).
        }

        return len;
    }



    string to_hex(unsigned long long val, char_type ca, unsigned pad_zeros)
    {
        return to_hex<string, char_type>( val, ca, pad_zeros );
    }

    string to_0xhex(unsigned long long val, char_type ca, unsigned pad_zeros)
    {
        return to_0xhex<string, char_type>( val, ca, pad_zeros );
    }



} // namespace strings
} // namespace cl7
