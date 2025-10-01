#include "errors.h"

#include <CoreLabs/text/codec.h>
#include <CoreLabs/text/transform.h>
#include <CoreLabs/sstream.h>

#include <windows.h>



namespace cl7::errors {



    cl7::u8string with_context(cl7::u8string_view message, cl7::u8string_view context)
    {
        cl7::u8osstream oss;
        oss << message << u8" " << context;
        return oss.str();
    }

    cl7::u8string system_result(unsigned long error_code)
    {
        // Extract from microsoft.com for the FormatMessage function:
        // "This buffer cannot be larger than 64K bytes."
        constexpr unsigned buffer_size = 65536 / sizeof(cl7::u8char_t);
        static wchar_t buffer[buffer_size];
        auto* buffer_ptr = static_cast<wchar_t*>(buffer);

        unsigned length = ::FormatMessageW(
            FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            nullptr,
            error_code,
            0,
            buffer_ptr,
            buffer_size,
            nullptr);

        cl7::u8osstream oss;
        oss << std::hex << u8"0x" << error_code;
        oss << u8" ";
        if (length)
            oss << cl7::text::codec::to_utf8(cl7::text::transform::trimmed(cl7::text::codec::reinterpret_utf16(std::wstring_view(buffer_ptr, length))));
        else
            oss << u8"An unknown error occurred.";
        return oss.str();
    }

    cl7::u8string system_result(unsigned long error_code, cl7::u8string_view context)
    {
        return with_context(system_result(error_code), context);
    }



} // namespace cl7::errors
