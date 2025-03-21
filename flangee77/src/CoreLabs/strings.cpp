#include "strings.h"

#include <CoreLabs/logging.h>
#include <CoreLabs/utilities.h>



// Make sure a wchar is either 2 bytes (UTF-16) or 4 bytes (UTF-32).
static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);

// Additionally, make sure that the limits are defined properly so that we can
// use them for case distinctions at compile time (where we cannot use sizeof).
static_assert(WCHAR_MAX == UINT16_MAX || WCHAR_MAX == UINT32_MAX);

// Verify byte order mark (BOM).
// This check is actually a "tautology" and should pass on all systems. But
// it's good for our psyche to simply leave it here; it doesn't do any harm.
static_assert(cl7::u32char_t{U'\ufeff'} == 0xfeff);



namespace cl7::strings {



    constexpr unsigned _replacement_ascii = 0x1a;//0x20;
    constexpr unsigned _replacement_unicode = 0xfffd;



    void _try_log_warning(bool log_warning, cl7::u8string_view message)
    {
        if (log_warning)
            LOG_WARNING(message);
    }

    bool _try_log_warning_always_return_false(bool log_warning, cl7::u8string_view message)
    {
        _try_log_warning(log_warning, message);
        return false;
    }

    achar_t _check_adjust_ascii(u32char_t u32c, bool log_warning = false)
    {
        if (u32c > 0x7f)
        {
            _try_log_warning(log_warning, u8"Invalid ASCII character: " + to_0xhex(u32c) + u8" (" + to_string(uint32_t{u32c}) + u8")");
            u32c = _replacement_ascii;
        }

        return static_cast<achar_t>(u32c);
    }

    u32char_t _check_adjust_unicode(u32char_t u32c, bool log_warning = false)
    {
        if (u32c > 0x10ffff)
        {
            _try_log_warning(log_warning, u8"Invalid Unicode code point: " + to_0xhex(u32c));
            u32c = _replacement_unicode;
        }
        else if (u32c >= 0xd800 && u32c <= 0xdfff)
        {
            _try_log_warning(log_warning, u8"Unpaired Unicode surrogate: " + to_0xhex(u32c));
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

    template <class uxstring, typename uxchar_type>
    uxstring _to_utfx(astring_view as)
    {
        uxstring uxs(as.size(), uxchar_type{0});

        for (size_t i = 0; i < as.size(); ++i)
        {
            uxs[i] = as[i] & 0xff; // No check required because everything from 0 to 255 is valid anyway.
        }

        return uxs;
    }



    astring to_ascii(astring_view as)
    {
        check_ascii(as, true);
        return astring(as);
    }

    astring to_ascii(u8string_view u8s)
    {
        u32string u32s;
        parse_utf8(u8s, u32s, true);
        return to_ascii(u32s);
    }

    astring to_ascii(u16string_view u16s)
    {
        u32string u32s;
        parse_utf16(u16s, u32s, true);
        return to_ascii(u32s);
    }

    astring to_ascii(u32string_view u32s)
    {
        astring as(u32s.size(), achar_t{0});

        for (size_t i = 0; i < u32s.size(); ++i)
        {
            as[i] = _check_adjust_ascii(u32s[i], true);
        }

        return as;
    }

    u8string to_utf8(astring_view as)
    {
        //return {as.begin(), as.end()}; // Should we trust that there isn't Latin-1 or something like that in here?
        return to_utf8(to_utf32(as)); // _to_utfx cannot be used here because characters above 0x7f must be encoded.
    }

    u8string to_utf8(u8string_view u8s)
    {
        return u8string(u8s);
    }

    u8string to_utf8(u16string_view u16s)
    {
        return to_utf8(to_utf32(u16s));
    }

    u8string to_utf8(u32string_view u32s)
    {
        size_t size = 0;
        for (u32char_t u32c : u32s)
        {
            u32c = _check_adjust_unicode(u32c);

            if (u32c <= 0x7f)
                size += 1;
            else if (u32c <= 0x7ff)
                size += 2;
            else if (u32c <= 0xffff)
                size += 3;
            else
                size += 4;
        }

        u8string u8s(size, u8char_t{0});

        size_t i = 0;
        for (u32char_t u32c : u32s)
        {
            u32c = _check_adjust_unicode(u32c, true);

            if (u32c <= 0x7f)
            {
                u8s[i++] = static_cast<u8char_t>(u32c);
            }
            else if (u32c <= 0x7ff)
            {
                u8s[i++] = static_cast<u8char_t>(0xc0 | ((u32c >> 6) & 0x1f));
                u8s[i++] = static_cast<u8char_t>(0x80 | (u32c & 0x3f));
            }
            else if (u32c <= 0xffff)
            {
                u8s[i++] = static_cast<u8char_t>(0xe0 | ((u32c >> 12) & 0x0f));
                u8s[i++] = static_cast<u8char_t>(0x80 | ((u32c >> 6) & 0x3f));
                u8s[i++] = static_cast<u8char_t>(0x80 | (u32c & 0x3f));
            }
            else
            {
                u8s[i++] = static_cast<u8char_t>(0xf0 | ((u32c >> 18) & 0x07));
                u8s[i++] = static_cast<u8char_t>(0x80 | ((u32c >> 12) & 0x3f));
                u8s[i++] = static_cast<u8char_t>(0x80 | ((u32c >> 6) & 0x3f));
                u8s[i++] = static_cast<u8char_t>(0x80 | (u32c & 0x3f));
            }
        } // for ...
        assert(i == size);

        return u8s;
    }

    u16string to_utf16(astring_view as)
    {
        return _to_utfx<u16string, u16char_t>(as);
    }

    u16string to_utf16(u8string_view u8s)
    {
        return to_utf16(to_utf32(u8s));
    }

    u16string to_utf16(u16string_view u16s)
    {
        return u16string(u16s);
    }

    u16string to_utf16(u32string_view u32s)
    {
        size_t size = 0;
        for (u32char_t u32c : u32s)
        {
            u32c = _check_adjust_unicode(u32c);

            if (u32c <= 0xffff)
                size += 1;
            else
                size += 2;
        }

        u16string u16s(size, u16char_t{0});

        size_t i = 0;
        for (u32char_t u32c : u32s)
        {
            u32c = _check_adjust_unicode(u32c, true);

            if (u32c <= 0xffff)
            {
                u16s[i++] = static_cast<u16char_t>(u32c);
            }
            else
            {
                u32char_t u32c_ = u32c - 0x10000;
                assert((u32c_ >> 10) <= 0x3ff);
                u16s[i++] = static_cast<u16char_t>(0xd800 | (u32c_ >> 10));
                u16s[i++] = static_cast<u16char_t>(0xdc00 | (u32c_ & 0x3ff));
            }
        } // for ...
        assert(i == size);

        return u16s;
    }

    u32string to_utf32(astring_view as)
    {
        return _to_utfx<u32string, u32char_t>(as);
    }

    u32string to_utf32(u8string_view u8s)
    {
        u32string u32s;
        parse_utf8(u8s, u32s, true);
        return u32s;
    }

    u32string to_utf32(u16string_view u16s)
    {
        u32string u32s;
        parse_utf16(u16s, u32s, true);
        return u32s;
    }

    u32string to_utf32(u32string_view u32s)
    {
        return u32string(u32s);
    }

#if WCHAR_MAX == UINT16_MAX
    u16string to_utfx(astring_view as) { return to_utf16(as); }
    u16string to_utfx(u8string_view u8s) { return to_utf16(u8s); }
    u16string to_utfx(u16string_view u16s) { return to_utf16(u16s); }
    u16string to_utfx(u32string_view u32s) { return to_utf16(u32s); }
#endif
#if WCHAR_MAX == UINT32_MAX
    u32string to_utfx(astring_view as) { return to_utf32(as); }
    u32string to_utfx(u8string_view u8s) { return to_utf32(u8s); }
    u32string to_utfx(u16string_view u16s) { return to_utf32(u16s); }
    u32string to_utfx(u32string_view u32s) { return to_utf32(u32s); }
#endif

    astring to_ascii_unchecked(byte_view bys)
    {
        return {reinterpret_cast<const achar_t*>(bys.data()), bys.size()};
    }

    u8string to_utf8_unchecked(byte_view bys)
    {
        const auto* data = reinterpret_cast<const u8char_t*>(bys.data());
        size_t size = bys.size();

        if (size >= 3)
        {
            bool has_bom = data[0] == 0xef && data[1] == 0xbb && data[2] == 0xbf;
            if (has_bom)
            {
                data += 3;
                size -= 3;
            }
        }

        return {data, size};
    }

    u16string to_utf16_unchecked(byte_view bys)
    {
        const auto* data = reinterpret_cast<const u16char_t*>(bys.data());
        size_t size = bys.size() / 2;

        bool byteswap = false;
        if (size >= 1)
        {
            bool has_bom = false;
            if (data[0] == 0xfeff)
            {
                has_bom = true;
                byteswap = false;
            }
            else if (data[0] == 0xfffe)
            {
                has_bom = true;
                byteswap = true;
            }
            if (has_bom)
            {
                data += 1;
                size -= 1;
            }
        }

        if (!byteswap)
            return {data, size};

        u16string u16s(size, u16char_t{0});

        for (size_t i = 0; i < size; ++i)
        {
            //u16s[i] = std::byteswap(data[i]);
            //u16s[i] = cl7::utilities::swap_bytes(data[i]);
            u16s[i] = ((data[i] & 0x00ff) << 8) | ((data[i] & 0xff00) >> 8);
        } // for ...

        return u16s;
    }

    u32string to_utf32_unchecked(byte_view bys)
    {
        const auto* data = reinterpret_cast<const u32char_t*>(bys.data());
        size_t size = bys.size() / 4;

        bool byteswap = false;
        if (size >= 1)
        {
            bool has_bom = false;
            if (data[0] == 0x0000'feff)
            {
                has_bom = true;
                byteswap = false;
            }
            else if (data[0] == 0xfffe'0000)
            {
                has_bom = true;
                byteswap = true;
            }
            if (has_bom)
            {
                data += 1;
                size -= 1;
            }
        }

        if (!byteswap)
            return {data, size};

        u32string u32s(size, u32char_t{0});

        for (size_t i = 0; i < size; ++i)
        {
            //u32s[i] = std::byteswap(data[i]);
            //u32s[i] = cl7::utilities::swap_bytes(data[i]);
            u32s[i] = ((data[i] & 0x0000'00ff) << 24) | ((data[i] & 0x0000'ff00) << 8) | ((data[i] & 0x00ff'0000) >> 8) | ((data[i] & 0xff00'0000) >> 24);
        } // for ...

        return u32s;
    }

    byte_vector to_bytes(astring_view as)
    {
        const auto* const data = reinterpret_cast<const std::byte*>(as.data());
        const size_t size = as.size();

        return {data, data + size};
    }

    byte_vector to_bytes(u8string_view u8s, bool add_bom)
    {
        const auto* const data = reinterpret_cast<const std::byte*>(u8s.data());
        const size_t size = u8s.size();

        if (!add_bom)
            return {data, data + size};

        byte_vector bys(size + 3);
        bys[0] = std::byte{0xef};
        bys[1] = std::byte{0xbb};
        bys[2] = std::byte{0xbf};
        for (size_t i = 0; i < size; ++i)
        {
            bys[i + 3] = data[i];
        } // for ...

        return bys;
    }

    byte_vector to_bytes(u16string_view u16s, bool add_bom, std::endian endian)
    {
        const auto* const data = reinterpret_cast<const std::byte*>(u16s.data());
        const size_t size = u16s.size() * 2;

        const bool byteswap = endian != std::endian::native;

        if (!add_bom && !byteswap)
            return {data, data + size};

        const size_t ofs = add_bom ? 2 : 0;
        byte_vector bys(size + ofs);
        if (byteswap)
        {
            if (add_bom)
                *reinterpret_cast<u16char_t*>(bys.data()) = 0xfffe;
            for (size_t i = 0; i < size; i += 2)
            {
                bys[ofs + i + 0] = data[i + 1];
                bys[ofs + i + 1] = data[i + 0];
            } // for ...
        }
        else
        {
            if (add_bom)
                *reinterpret_cast<u16char_t*>(bys.data()) = 0xfeff;
            for (size_t i = 0; i < size; i += 2)
            {
                bys[ofs + i + 0] = data[i + 0];
                bys[ofs + i + 1] = data[i + 1];
            } // for ...
        }

        return bys;
    }

    byte_vector to_bytes(u32string_view u32s, bool add_bom, std::endian endian)
    {
        const auto* const data = reinterpret_cast<const std::byte*>(u32s.data());
        const size_t size = u32s.size() * 4;

        const bool byteswap = endian != std::endian::native;

        if (!add_bom && !byteswap)
            return {data, data + size};

        const size_t ofs = add_bom ? 4 : 0;
        byte_vector bys(size + ofs);
        if (byteswap)
        {
            if (add_bom)
                *reinterpret_cast<u32char_t*>(bys.data()) = 0xfffe'0000;
            for (size_t i = 0; i < size; i += 4)
            {
                bys[ofs + i + 0] = data[i + 3];
                bys[ofs + i + 1] = data[i + 2];
                bys[ofs + i + 2] = data[i + 1];
                bys[ofs + i + 3] = data[i + 0];
            } // for ...
        }
        else
        {
            if (add_bom)
                *reinterpret_cast<u32char_t*>(bys.data()) = 0x0000'feff;
            for (size_t i = 0; i < size; i += 4)
            {
                bys[ofs + i + 0] = data[i + 0];
                bys[ofs + i + 1] = data[i + 1];
                bys[ofs + i + 2] = data[i + 2];
                bys[ofs + i + 3] = data[i + 3];
            } // for ...
        }

        return bys;
    }

    bool check_ascii(astring_view as, bool log_warning)
    {
        for (achar_t ac : as)
        {
            // Explicitly cast to unsigned, otherwise the
            // following check would always be false anyway.
            const auto u8c = static_cast<uint8_t>(ac);
            if (u8c > 0x7f)
                return _try_log_warning_always_return_false(log_warning, u8"Invalid ASCII character: " + to_0xhex(u8c) + u8" (" + to_string(u8c) + u8")");
        }

        return true;
    }

    bool check_utf8(u8string_view u8s, bool log_warning)
    {
        unsigned continuation = 0;

        for (u8char_t u8c : u8s)
        {
            if (continuation)
            {
                if (u8c >= 0x80 && u8c <= 0xbf) // (expected) continuation byte of a sequence
                {
                    // Everything's fine: Nothing to do here.
                }
                else // bad code unit
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Bad UTF-8 continuation byte: " + to_0xhex(u8c));
                }
                --continuation;
            }
            else // => no sequence continuation (yet)
            {
                if (u8c <= 0x7f) // "regular" character (ASCII compatible)
                {
                    // Nothing to do here.
                }
                else if (u8c >= 0xc0 && u8c <= 0xdf) // start of 2-byte sequence
                {
                    if (u8c < 0xc2) // invalid 2-byte sequence that represents code range from 0 to 127
                        return _try_log_warning_always_return_false(log_warning, u8"Start of invalid 2-byte UTF-8 sequence representing code range from 0 to 127.");
                    continuation = 1;
                }
                else if (u8c >= 0xe0 && u8c <= 0xef) // start of 3-byte sequence
                {
                    continuation = 2;
                }
                else if (u8c >= 0xf0 && u8c <= 0xf7) // start of 4-byte sequence
                {
                    if (u8c > 0xf4) // invalid 4-byte sequence that represents code range above 0x140000
                        return _try_log_warning_always_return_false(log_warning, u8"Start of invalid 4-byte UTF-8 sequence representing code range above 0x140000.");
                    continuation = 3;
                }
                else if (u8c >= 0xf8 && u8c <= 0xfb) // start of invalid 5-byte sequence
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Start of invalid 5-byte UTF-8 sequence.");
                }
                else if (u8c >= 0xfc && u8c <= 0xfd) // start of invalid 6-byte sequence
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Start of invalid 6-byte UTF-8 sequence.");
                }
                else if (u8c >= 0x80 && u8c <= 0xbf) // unexpected continuation byte of unknown sequence
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Unexpected UTF-8 continuation byte: " + to_0xhex(u8c));
                }
                else // invalid code unit (probably 0xfe or 0xff, because we should have already covered all others)
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Invalid UTF-8 code unit: " + to_0xhex(u8c));
                }
            } // no sequence continuation (yet)
        } // for ...
        if (continuation)
        {
            return _try_log_warning_always_return_false(log_warning, u8"Incomplete UTF-8 sequence: " + to_string(continuation) + u8" byte(s) missing");
        }

        return true;
    }

    bool check_utf16(u16string_view u16s, bool log_warning)
    {
        bool surrogate = false;
        u16char_t prev = 0;

        for (u16char_t u16c : u16s)
        {
            if (surrogate)
            {
                if (u16c >= 0xdc00 && u16c <= 0xdfff) // (expected) trailing/low surrogate
                {
                    // Everything's fine: Nothing to do here.
                }
                else if (u16c >= 0xd800 && u16c <= 0xdbff) // (another) leading/high surrogate
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogates: " + to_0xhex(prev) + u8" " + to_0xhex(u16c));
                }
                else // "regular" character (but invalid here)
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogate: " + to_0xhex(prev));
                }
                surrogate = false;
            }
            else // => no surrogate (yet)
            {
                if (u16c <= 0xd7ff || u16c >= 0xe000) // "regular" character
                {
                    // Nothing to do here.
                }
                else if (u16c >= 0xd800 && u16c <= 0xdbff) // leading/high surrogate
                {
                    // Everything's fine: remember the leading/high surrogate.
                    // This is the only case where we don't pass a character (yet).
                    prev = u16c;
                    surrogate = true;
                }
                else //if (u16c >= 0xdc00 && u16c <= 0xdfff) // (unexpected) trailing/low surrogate
                {
                    return _try_log_warning_always_return_false(log_warning, u8"Unpaired trailing/low UTF-16 surrogate: " + to_0xhex(u16c));
                }
            } // no surrogate (yet)
        } // for ...
        if (surrogate)
        {
            return _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogate: " + to_0xhex(prev));
        }

        return true;
    }

    bool check_utf32(u32string_view u32s, bool log_warning)
    {
        for (u32char_t u32c : u32s)
        {
            if (u32c > 0x10ffff || (u32c >= 0xd800 && u32c <= 0xdfff))
                return _try_log_warning_always_return_false(log_warning, u8"Invalid Unicode code point: " + to_0xhex(u32c));
        }

        return true;
    }

    bool parse_utf8(u8string_view u8s, u32string& u32s, bool log_warning)
    {
        u32s.resize(utf8_length(u8s), u32char_t{0});

        bool is_valid = true;

        unsigned continuation = 0;
        u32char_t u32c = 0;
        bool is_char_valid = false;

        size_t i = 0;
        for (u8char_t u8c : u8s)
        {
            if (continuation)
            {
                if (u8c >= 0x80 && u8c <= 0xbf) // (expected) continuation byte of a sequence
                {
                    // Everything's fine: merge byte into character "buffer".
                    u32c = (u32c << 6) | (u8c & 0x3f);
                }
                else // bad code unit
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false(log_warning, u8"Bad UTF-8 continuation byte: " + to_0xhex(u8c));
                }
                if (--continuation == 0) // end of sequence
                {
                    if (is_char_valid && u32c != _replacement_unicode && _check_adjust_unicode(u32c) != _replacement_unicode) // Play it safe because even valid constellations of specific sequences can lead to invalid code points.
                        u32s[i++] = u32c;
                    else
                        u32s[i++] = _replacement_unicode;
                }
            }
            else // => no sequence continuation (yet)
            {
                if (u8c <= 0x7f) // "regular" character (ASCII compatible)
                {
                    u32s[i++] = static_cast<u32char_t>(u8c);
                }
                else if (u8c >= 0xc0 && u8c <= 0xdf) // start of 2-byte sequence
                {
                    if (u8c < 0xc2) // invalid 2-byte sequence that represents code range from 0 to 127
                        is_valid = is_char_valid = _try_log_warning_always_return_false(log_warning, u8"Start of invalid 2-byte UTF-8 sequence representing code range from 0 to 127.");
                    else
                        is_char_valid = true;
                    u32c = u8c & 0x1f;
                    continuation = 1;
                }
                else if (u8c >= 0xe0 && u8c <= 0xef) // start of 3-byte sequence
                {
                    is_char_valid = true;
                    u32c = u8c & 0x0f;
                    continuation = 2;
                }
                else if (u8c >= 0xf0 && u8c <= 0xf7) // start of 4-byte sequence
                {
                    if (u8c > 0xf4) // invalid 4-byte sequence that represents code range above 0x140000
                        is_valid = is_char_valid = _try_log_warning_always_return_false(log_warning, u8"Start of invalid 4-byte UTF-8 sequence representing code range above 0x140000.");
                    else
                        is_char_valid = true;
                    u32c = u8c & 0x07;
                    continuation = 3;
                }
                else if (u8c >= 0xf8 && u8c <= 0xfb) // start of invalid 5-byte sequence
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false(log_warning, u8"Start of invalid 5-byte UTF-8 sequence.");
                    u32c = u8c & 0x03;
                    continuation = 4;
                }
                else if (u8c >= 0xfc && u8c <= 0xfd) // start of invalid 6-byte sequence
                {
                    is_valid = is_char_valid = _try_log_warning_always_return_false(log_warning, u8"Start of invalid 6-byte UTF-8 sequence.");
                    u32c = u8c & 0x01;
                    continuation = 5;
                }
                else if (u8c >= 0x80 && u8c <= 0xbf) // unexpected continuation byte of unknown sequence
                {
                    is_valid = _try_log_warning_always_return_false(log_warning, u8"Unexpected UTF-8 continuation byte: " + to_0xhex(u8c));
                    u32s[i++] = _replacement_unicode;
                }
                else // invalid code unit (probably 0xfe or 0xff, because we should have already covered all others)
                {
                    is_valid = _try_log_warning_always_return_false(log_warning, u8"Invalid UTF-8 code unit: " + to_0xhex(u8c));
                    u32s[i++] = _replacement_unicode;
                }
            } // no sequence continuation (yet)
        } // for ...
        if (continuation)
        {
            is_valid = _try_log_warning_always_return_false(log_warning, u8"Incomplete UTF-8 sequence: " + to_string(continuation) + u8" byte(s) missing");
            u32s[i++] = _replacement_unicode;
        }
        assert(i == u32s.size());

        return is_valid;
    }

    bool parse_utf16(u16string_view u16s, u32string& u32s, bool log_warning)
    {
        u32s.resize(utf16_length(u16s), u32char_t{0});

        bool is_valid = true;

        bool surrogate = false;
        u16char_t prev = 0;

        size_t i = 0;
        for (u16char_t u16c : u16s)
        {
            if (surrogate)
            {
                if (u16c >= 0xdc00 && u16c <= 0xdfff) // (expected) trailing/low surrogate
                {
                    // Everything's fine: put the two surrogates back together.
                    u32s[i++] = ((static_cast<u32char_t>(prev & 0x3ff) << 10) | (u16c & 0x3ff)) + 0x10000;
                }
                else if (u16c >= 0xd800 && u16c <= 0xdbff) // (another) leading/high surrogate
                {
                    is_valid = _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogates: " + to_0xhex(prev) + u8" " + to_0xhex(u16c));
                    u32s[i++] = _replacement_unicode;
                }
                else // "regular" character (but invalid here)
                {
                    is_valid = _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogate: " + to_0xhex(prev));
                    // Consistently, we might also have to fall back to the replacement
                    // character here, but then we would "waste" the valid character we
                    // hold in our hands. So we just pass it in any case.
                    u32s[i++] = static_cast<u32char_t>(u16c);
                }
                // Also in the case of a leading/high surrogate set the flag to
                // false as not to distort the skipping. In such case the structure
                // is messed up anyway, so we can't rely on the current code unit
                // being a valid leading/high surrogate either.
                surrogate = false;
            }
            else // => no surrogate (yet)
            {
                if (u16c <= 0xd7ff || u16c >= 0xe000) // "regular" character
                {
                    u32s[i++] = static_cast<u32char_t>(u16c);
                }
                else if (u16c >= 0xd800 && u16c <= 0xdbff) // leading/high surrogate
                {
                    // Everything's fine: remember the leading/high surrogate.
                    // This is the only case where we don't pass a character (yet).
                    prev = u16c;
                    surrogate = true;
                }
                else //if (u16c >= 0xdc00 && u16c <= 0xdfff) // (unexpected) trailing/low surrogate
                {
                    is_valid = _try_log_warning_always_return_false(log_warning, u8"Unpaired trailing/low UTF-16 surrogate: " + to_0xhex(u16c));
                    u32s[i++] = _replacement_unicode;
                }
            } // no surrogate (yet)
        } // for ...
        if (surrogate)
        {
            is_valid = _try_log_warning_always_return_false(log_warning, u8"Unpaired leading/high UTF-16 surrogate: " + to_0xhex(prev));
            u32s[i++] = _replacement_unicode;
        }
        assert(i == u32s.size());

        return is_valid;
    }

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(u8string_view u8s)
    {
        const size_t size = u8s.size();
        size_t len = 0;

        for (size_t i = 0; i < size; ++i, ++len)
        {
            const u8char_t u8c = u8s[i];
            if (u8c >= 0xc0 && u8c <= 0xdf) // 0xc0 and 0xc1 are actually invalid, but the following byte still has to be skipped.
                ++i; // Skip 1 continuation byte of a 2-byte sequence.
            else if (u8c >= 0xe0 && u8c <= 0xef)
                i += 2; // Skip 2 continuation bytes of a 3-byte sequence.
            else if (u8c >= 0xf0 && u8c <= 0xf7) // 0xf5 to 0xf7 are actually invalid, but the following 3 bytes still have to be skipped.
                i += 3; // Skip 3 continuation bytes of a 4-byte sequence.
            else if (u8c >= 0xf8 && u8c <= 0xfb)
                i += 4; // Invalid, but the following 4 bytes still have to be skipped.
            else if (u8c >= 0xfc && u8c <= 0xfd)
                i += 5; // Invalid, but the following 5 bytes still have to be skipped.

            // We don't need to check (other) invalid sequences because they would
            // be represented as a single replacement character. In terms of code
            // points, this case would be identical to the regular case of a single
            // valid character between 0x00 and 0x7f.
        }

        return len;
    }

    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(u16string_view u16s)
    {
        const size_t size = u16s.size();
        size_t len = 0;

        for (size_t i = 0; i < size; ++i, ++len)
        {
            const u16char_t u16c = u16s[i];
            if (u16c >= 0xd800 && u16c <= 0xdbff)
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

    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    std::string_view reinterpret_utf8(u8string_view u8s)
    {
        static_assert(sizeof(char) == sizeof(u8char_t));
        return {reinterpret_cast<const char*>(u8s.data()), u8s.size()};
    }

    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    u8string_view reinterpret_utf8(std::string_view s)
    {
        static_assert(sizeof(u8char_t) == sizeof(char));
        return {reinterpret_cast<const u8char_t*>(s.data()), s.size()};
    }

#if WCHAR_MAX == UINT16_MAX
    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    std::wstring_view reinterpret_utf16(u16string_view u16s)
    {
        static_assert(sizeof(wchar_t) == sizeof(u16char_t));
        return {reinterpret_cast<const wchar_t*>(u16s.data()), u16s.size()};
    }

    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    u16string_view reinterpret_utf16(std::wstring_view ws)
    {
        static_assert(sizeof(u16char_t) == sizeof(wchar_t));
        return {reinterpret_cast<const u16char_t*>(ws.data()), ws.size()};
    }

    std::wstring_view reinterpret_utfx(u16string_view u16s) { return reinterpret_utf16(u16s); }
    u16string_view reinterpret_utfx(std::wstring_view ws) { return reinterpret_utf16(ws); }
#endif
#if WCHAR_MAX == UINT32_MAX
    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    std::wstring_view reinterpret_utf32(u32string_view u32s)
    {
        static_assert(sizeof(wchar_t) == sizeof(u32char_t));
        return {reinterpret_cast<const wchar_t*>(u32s.data()), u32s.size()};
    }

    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    u32string_view reinterpret_utf32(std::wstring_view ws)
    {
        static_assert(sizeof(u32char_t) == sizeof(wchar_t));
        return {reinterpret_cast<const u32char_t*>(ws.data()), ws.size()};
    }

    std::wstring_view reinterpret_utfx(u32string_view u32s) { return reinterpret_utf32(u32s); }
    u32string_view reinterpret_utfx(std::wstring_view ws) { return reinterpret_utf32(ws); }
#endif

    Encoding detect_encoding(byte_view bys)
    {
        const size_t size = bys.size();

        if (size == 0)
            return Encoding::Unknown;

        if (size % 4 == 0)
        {
            if (check_utf32(to_utf32_unchecked(bys)))
                return Encoding::UTF32;
        }

        if (size % 2 == 0)
        {
            if (check_utf16(to_utf16_unchecked(bys)))
                return Encoding::UTF16;
        }

        // We check ASCII before UTF-8 because ASCII is a subset of UTF-8.
        if (check_ascii(to_ascii_unchecked(bys)))
            return Encoding::ASCII;

        if (check_utf8(to_utf8_unchecked(bys)))
            return Encoding::UTF8;

        return Encoding::Unknown;
    }



    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * This function enforces strict matching rules:
     * - For single-byte whitespace, c1 and c2 must be 0.
     * - For two-byte whitespace, c2 must be 0.
     */
    size_t is_whitespace_strict(u8char_t c0, u8char_t c1, u8char_t c2)
    {
        if (c1 == 0 && c2 == 0)
        {
            if ((c0 >= 0x09 && c0 <= 0x0d) || // tab ... carriage return
                c0 == 0x20)                   // space
                return 1;
        }

        if (c2 == 0)
        {
            if (c0 == 0xc2 && c1 == 0x85) return 2; // next line
            if (c0 == 0xc2 && c1 == 0xa0) return 2; // no-break space
        }

        if (c0 == 0xe1 && c1 == 0x9a && c2 == 0x80) return 3; // Ogham space mark

        if (c0 == 0xe2 && c1 == 0x80)
        {
            if (c2 >= 0x80 && c2 <= 0x8a) return 3; // en quad ... hair space
            if (c2 == 0xa8) return 3; // line separator
            if (c2 == 0xa9) return 3; // paragraph separator
            if (c2 == 0xaf) return 3; // narrow no-break space
        }

        if (c0 == 0xe2 && c1 == 0x81 && c2 == 0x9f) return 3; // medium mathematical space
        if (c0 == 0xe3 && c1 == 0x80 && c2 == 0x80) return 3; // ideographic space

        return 0;
    }

    /**
     * Checks whether the specified Unicode code point is whitespace and, if yes,
     * returns the number of corresponding UTF-8 characters (code units).
     * Unlike the strict version, this function does not require c1 or c2 to be 0
     * for shorter combinations.
     */
    size_t is_whitespace_relaxed(u8char_t c0, u8char_t c1, u8char_t c2)
    {
        if ((c0 >= 0x09 && c0 <= 0x0d) || // tab ... carriage return
            c0 == 0x20)                   // space
            return 1;

        if (c0 == 0xc2 && c1 == 0x85) return 2; // next line
        if (c0 == 0xc2 && c1 == 0xa0) return 2; // no-break space

        if (c0 == 0xe1 && c1 == 0x9a && c2 == 0x80) return 3; // Ogham space mark

        if (c0 == 0xe2 && c1 == 0x80)
        {
            if (c2 >= 0x80 && c2 <= 0x8a) return 3; // en quad ... hair space
            if (c2 == 0xa8) return 3; // line separator
            if (c2 == 0xa9) return 3; // paragraph separator
            if (c2 == 0xaf) return 3; // narrow no-break space
        }

        if (c0 == 0xe2 && c1 == 0x81 && c2 == 0x9f) return 3; // medium mathematical space
        if (c0 == 0xe3 && c1 == 0x80 && c2 == 0x80) return 3; // ideographic space

        return 0;
    }



    /**
     * Checks whether the specified UTF-8 string starts with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_prefix(u8string_view s)
    {
        const size_t n = s.size();
        if (n >= 3) return is_whitespace_relaxed(s[0], s[1], s[2]);
        if (n >= 2) return is_whitespace_relaxed(s[0], s[1]);
        if (n >= 1) return is_whitespace_relaxed(s[0]);
        return 0;
    }

    /**
     * Checks whether the specified UTF-8 string ends with whitespace and, if yes,
     * returns the number of UTF-8 characters (code units) of the corresponding code
     * point.
     */
    size_t is_whitespace_suffix(u8string_view s)
    {
        const size_t n = s.size();
        size_t k;
        // NOLINTBEGIN(bugprone-assignment-in-if-condition)
        if (n >= 1 && (k = is_whitespace_strict(s[n - 1])) > 0) return k;
        if (n >= 2 && (k = is_whitespace_strict(s[n - 2], s[n - 1])) > 0) return k;
        if (n >= 3 && (k = is_whitespace_strict(s[n - 3], s[n - 2], s[n - 1])) > 0) return k;
        // NOLINTEND(bugprone-assignment-in-if-condition)
        return 0;
    }



    u8string to_hex(unsigned long long val, unsigned pad_zeros, u8char_t ca)
    {
        return to_hex<u8string>(val, pad_zeros, ca);
    }

    u8string to_0xhex(unsigned long long val, unsigned pad_zeros, u8char_t ca)
    {
        return to_0xhex<u8string>(val, pad_zeros, ca);
    }



} // namespace cl7::strings
