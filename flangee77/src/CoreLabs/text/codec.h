#ifndef CL7_TEXT_CODEC_H
#define CL7_TEXT_CODEC_H

#include "./codec/Encoding.h"

#include <CoreLabs/string.h>
#include <CoreLabs/byte_vector.h>
#include <CoreLabs/byte_view.h>

#include <bit>



namespace cl7::text::codec {



    astring to_ascii(astring_view as);
    astring to_ascii(u8string_view u8s);
    astring to_ascii(u16string_view u16s);
    astring to_ascii(u32string_view u32s);

    u8string to_utf8(astring_view as);
    u8string to_utf8(u8string_view u8s);
    u8string to_utf8(u16string_view u16s);
    u8string to_utf8(u32string_view u32s);

    u16string to_utf16(astring_view as);
    u16string to_utf16(u8string_view u8s);
    u16string to_utf16(u16string_view u16s);
    u16string to_utf16(u32string_view u32s);

    u32string to_utf32(astring_view as);
    u32string to_utf32(u8string_view u8s);
    u32string to_utf32(u16string_view u16s);
    u32string to_utf32(u32string_view u32s);

#if WCHAR_MAX == UINT16_MAX
    u16string to_utfx(astring_view as);
    u16string to_utfx(u8string_view u8s);
    u16string to_utfx(u16string_view u16s);
    u16string to_utfx(u32string_view u32s);
#endif
#if WCHAR_MAX == UINT32_MAX
    u32string to_utfx(astring_view as);
    u32string to_utfx(u8string_view u8s);
    u32string to_utfx(u16string_view u16s);
    u32string to_utfx(u32string_view u32s);
#endif

    astring to_ascii_unchecked(byte_view bys);
    u8string to_utf8_unchecked(byte_view bys);
    u16string to_utf16_unchecked(byte_view bys);
    u32string to_utf32_unchecked(byte_view bys);

    byte_vector to_bytes(astring_view as);
    byte_vector to_bytes(u8string_view u8s, bool add_bom = false);
    byte_vector to_bytes(u16string_view u16s, bool add_bom = false, std::endian endian = std::endian::native);
    byte_vector to_bytes(u32string_view u32s, bool add_bom = false, std::endian endian = std::endian::native);

    bool check_ascii(astring_view as, bool log_warning = false);
    bool check_utf8(u8string_view u8s, bool log_warning = false);
    bool check_utf16(u16string_view u16s, bool log_warning = false);
    bool check_utf32(u32string_view u32s, bool log_warning = false);

    bool parse_utf8(u8string_view u8s, u32string& u32s, bool log_warning = false);
    bool parse_utf16(u16string_view u16s, u32string& u32s, bool log_warning = false);

    /** Calculates the length of the specified UTF-8 string in terms of Unicode code points. */
    size_t utf8_length(u8string_view u8s);
    /** Calculates the length of the specified UTF-16 string in terms of Unicode code points. */
    size_t utf16_length(u16string_view u16s);

    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    std::string_view reinterpret_utf8(u8string_view u8s);
    /** Reinterprets the character format of the specified UTF-8 string. Attention: It is not checked whether a correct UTF-8 encoding is given. */
    u8string_view reinterpret_utf8(std::string_view s);

#if WCHAR_MAX == UINT16_MAX
    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    std::wstring_view reinterpret_utf16(u16string_view u16s);
    /** Reinterprets the character format of the specified UTF-16 string. Attention: It is not checked whether a correct UTF-16 encoding is given. */
    u16string_view reinterpret_utf16(std::wstring_view ws);
#endif
#if WCHAR_MAX == UINT32_MAX
    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    std::wstring_view reinterpret_utf32(u32string_view u32s);
    /** Reinterprets the character format of the specified UTF-32 string. Attention: It is not checked whether a correct UTF-32 encoding is given. */
    u32string_view reinterpret_utf32(std::wstring_view ws);
#endif

#if WCHAR_MAX == UINT16_MAX
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    std::wstring_view reinterpret_utfx(u16string_view u16s);
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    u16string_view reinterpret_utfx(std::wstring_view ws);
#endif
#if WCHAR_MAX == UINT32_MAX
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    std::wstring_view reinterpret_utfx(u32string_view u32s);
    /** Reinterprets the character format of the specified UTF-16 (Windows) or UTF-32 (non-Windows) string. Attention: It is not checked whether a correct UTF-16/UTF-32 encoding is given. */
    u32string_view reinterpret_utfx(std::wstring_view ws);
#endif

    Encoding detect_encoding(byte_view bys);



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_H
