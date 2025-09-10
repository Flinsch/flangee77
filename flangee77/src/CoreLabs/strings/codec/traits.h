#ifndef CL7_STRINGS_CODEC_TRAITS_H
#define CL7_STRINGS_CODEC_TRAITS_H

#include "./Encoding.h"

#include "./AsciiCodec.h"
#include "./Utf8Codec.h"
#include "./Utf16Codec.h"
#include "./Utf32Codec.h"

#include <CoreLabs/string.h>



namespace cl7::strings::codec {



template <typename Tchar>
struct traits
{
    using char_type = Tchar;
    using string_type = std::basic_string<char_type>;
    using string_view_type = std::basic_string_view<char_type>;
    using string_span_type = std::span<char_type>;
    using codec_type = void;
};



template <>
struct traits<cl7::achar_t>
{
    using char_type = cl7::achar_t;
    using string_type = std::basic_string<char_type>;
    using string_view_type = std::basic_string_view<char_type>;
    using string_span_type = std::span<char_type>;
    using codec_type = AsciiCodec;
};

template <>
struct traits<cl7::u8char_t>
{
    using char_type = cl7::u8char_t;
    using string_type = std::basic_string<char_type>;
    using string_view_type = std::basic_string_view<char_type>;
    using string_span_type = std::span<char_type>;
    using codec_type = Utf8Codec;
};

template <>
struct traits<cl7::u16char_t>
{
    using char_type = cl7::u16char_t;
    using string_type = std::basic_string<char_type>;
    using string_view_type = std::basic_string_view<char_type>;
    using string_span_type = std::span<char_type>;
    using codec_type = Utf16Codec;
};

template <>
struct traits<cl7::u32char_t>
{
    using char_type = cl7::u32char_t;
    using string_type = std::basic_string<char_type>;
    using string_view_type = std::basic_string_view<char_type>;
    using string_span_type = std::span<char_type>;
    using codec_type = Utf32Codec;
};



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_TRAITS_H
