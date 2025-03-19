#ifndef CL7_STRINGS_ENCODEBUFFER_H
#define CL7_STRINGS_ENCODEBUFFER_H

#include <CoreLabs/string.h>



namespace cl7::strings {



template <typename Tchar>
    requires(std::is_integral_v<Tchar>)
struct EncodeBuffer
{
    using char_type = Tchar;
    using string_span_type = std::span<char_type>;
    using string_view_type = std::basic_string_view<char_type>;

    static constexpr size_t max_code_units = 4 / sizeof(char_type);



    char_type data[max_code_units];



    string_span_type string_span() { return {data, max_code_units}; }
};



} // namespace cl7::strings

#endif // CL7_STRINGS_ENCODEBUFFER_H
