#include "codec.h"

#include "./codec/Transcoder.h"



// Make sure a wchar is either 2 bytes (UTF-16) or 4 bytes (UTF-32).
static_assert(sizeof(wchar_t) == 2 || sizeof(wchar_t) == 4);

// Additionally, make sure that the limits are defined properly so that we can
// use them for case distinctions at compile time (where we cannot use sizeof).
static_assert(WCHAR_MAX == UINT16_MAX || WCHAR_MAX == UINT32_MAX);

// Verify byte order mark (BOM).
// This check is actually a "tautology" and should pass on all systems. But
// it's good for our psyche to simply leave it here; it doesn't do any harm.
static_assert(cl7::u32char_t{U'\ufeff'} == 0xfeff);



namespace cl7::text::codec {



    astring to_ascii(astring_view as)
    {
        Analyzer<achar_t>().validate(as);
        return astring(as);
    }

    astring to_ascii(u8string_view u8s)
    {
        return Transcoder<u8char_t, achar_t>().transcode(u8s);
    }

    astring to_ascii(u16string_view u16s)
    {
        return Transcoder<u16char_t, achar_t>().transcode(u16s);
    }

    astring to_ascii(u32string_view u32s)
    {
        return Transcoder<u32char_t, achar_t>().transcode(u32s);
    }

    u8string to_utf8(astring_view as)
    {
        return Transcoder<achar_t, u8char_t>().transcode(as);
    }

    u8string to_utf8(u8string_view u8s)
    {
        Analyzer<u8char_t>().validate(u8s);
        return u8string(u8s);
    }

    u8string to_utf8(u16string_view u16s)
    {
        return Transcoder<u16char_t, u8char_t>().transcode(u16s);
    }

    u8string to_utf8(u32string_view u32s)
    {
        return Transcoder<u32char_t, u8char_t>().transcode(u32s);
    }

    u16string to_utf16(astring_view as)
    {
        return Transcoder<achar_t, u16char_t>().transcode(as);
    }

    u16string to_utf16(u8string_view u8s)
    {
        return Transcoder<u8char_t, u16char_t>().transcode(u8s);
    }

    u16string to_utf16(u16string_view u16s)
    {
        Analyzer<u16char_t>().validate(u16s);
        return u16string(u16s);
    }

    u16string to_utf16(u32string_view u32s)
    {
        return Transcoder<u32char_t, u16char_t>().transcode(u32s);
    }

    u32string to_utf32(astring_view as)
    {
        return Transcoder<achar_t, u32char_t>().transcode(as);
    }

    u32string to_utf32(u8string_view u8s)
    {
        return Transcoder<u8char_t, u32char_t>().transcode(u8s);
    }

    u32string to_utf32(u16string_view u16s)
    {
        return Transcoder<u16char_t, u32char_t>().transcode(u16s);
    }

    u32string to_utf32(u32string_view u32s)
    {
        Analyzer<u32char_t>().validate(u32s);
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
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Analyzer<achar_t>(&error_handler).validate(as);
    }

    bool check_utf8(u8string_view u8s, bool log_warning)
    {
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Analyzer<u8char_t>(&error_handler).validate(u8s);
    }

    bool check_utf16(u16string_view u16s, bool log_warning)
    {
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Analyzer<u16char_t>(&error_handler).validate(u16s);
    }

    bool check_utf32(u32string_view u32s, bool log_warning)
    {
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Analyzer<u32char_t>(&error_handler).validate(u32s);
    }

    bool parse_utf8(u8string_view u8s, u32string& u32s, bool log_warning)
    {
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Transcoder<u8char_t, u32char_t>(&error_handler).transcode_with_status(u8s, u32s).error_count == 0;
    }

    bool parse_utf16(u16string_view u16s, u32string& u32s, bool log_warning)
    {
        LoggingErrorHandler error_handler{{.logging_enabled = log_warning, .logger = nullptr}};
        return Transcoder<u16char_t, u32char_t>(&error_handler).transcode_with_status(u16s, u32s).error_count == 0;
    }

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(u8string_view u8s)
    {
        return Analyzer<u8char_t>().determine_length(u8s);
    }

    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(u16string_view u16s)
    {
        return Analyzer<u16char_t>().determine_length(u16s);
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



} // namespace cl7::text::codec
