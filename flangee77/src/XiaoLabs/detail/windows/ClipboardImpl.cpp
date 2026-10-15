#include "ClipboardImpl.h"
#if F77_IS_WINDOWS

#include <CoreLabs/platform/errors.h>
#include <CoreLabs/logging.h>
#include <CoreLabs/text/codec.h>

#include <windows.h>

#include <cstring>



namespace {

    /**
     * Opens the clipboard, retrying for a short while on ERROR_ACCESS_DENIED:
     * some other process (e.g., Windows' own clipboard history/cloud sync, or an
     * antivirus clipboard scanner) often grabs it for a few milliseconds right
     * after its content changes, which would otherwise make back-to-back clipboard
     * operations (ours or anyone else's) spuriously fail.
     */
    bool open_clipboard_with_retry()
    {
        constexpr int max_attempts = 10;
        constexpr DWORD retry_delay_ms = 10;

        for (int attempt = 0; attempt < max_attempts; ++attempt)
        {
            if (::OpenClipboard(nullptr))
                return true;

            if (::GetLastError() != ERROR_ACCESS_DENIED)
                return false;

            ::Sleep(retry_delay_ms);
        }

        return false;
    }

} // namespace



namespace xl7::detail::windows {



    cl7::u32string ClipboardImpl::get_text()
    {
        if (!::IsClipboardFormatAvailable(CF_UNICODETEXT))
            return {};

        if (!open_clipboard_with_retry())
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::OpenClipboard"));
            return {};
        }

        cl7::u32string result;

        if (HANDLE handle = ::GetClipboardData(CF_UNICODETEXT))
        {
            if (const auto* data = static_cast<const wchar_t*>(::GlobalLock(handle)))
            {
                result = cl7::text::codec::to_utf32(cl7::text::codec::reinterpret_utf16(std::wstring_view(data)));
                ::GlobalUnlock(handle);
            }
            else
            {
                LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::GlobalLock"));
            }
        }
        else
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::GetClipboardData"));
        }

        ::CloseClipboard();
        return result;
    }

    bool ClipboardImpl::set_text(cl7::u32string_view text)
    {
        const cl7::u16string utf16 = cl7::text::codec::to_utf16(text);
        const std::wstring_view wide = cl7::text::codec::reinterpret_utf16(cl7::u16string_view(utf16));

        if (!open_clipboard_with_retry())
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::OpenClipboard"));
            return false;
        }

        if (!::EmptyClipboard())
        {
            LOG_WARNING(cl7::platform::errors::system_result(::GetLastError(), u8"::EmptyClipboard"));
            ::CloseClipboard();
            return false;
        }

        const size_t byte_size = (wide.size() + 1) * sizeof(wchar_t);
        const HGLOBAL global_handle = ::GlobalAlloc(GMEM_MOVEABLE, byte_size);
        if (!global_handle)
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::GlobalAlloc"));
            ::CloseClipboard();
            return false;
        }

        void* const locked = ::GlobalLock(global_handle);
        if (!locked)
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::GlobalLock"));
            ::GlobalFree(global_handle);
            ::CloseClipboard();
            return false;
        }

        std::memcpy(locked, wide.data(), wide.size() * sizeof(wchar_t));
        static_cast<wchar_t*>(locked)[wide.size()] = L'\0';
        ::GlobalUnlock(global_handle);

        // Ownership of global_handle only passes to the system on success.
        // On failure, it's still ours to free.
        const bool success = ::SetClipboardData(CF_UNICODETEXT, global_handle) != nullptr;
        if (!success)
        {
            LOG_ERROR(cl7::platform::errors::system_result(::GetLastError(), u8"::SetClipboardData"));
            ::GlobalFree(global_handle);
        }

        ::CloseClipboard();
        return success;
    }



} // namespace xl7::detail::windows

#endif // F77_IS_WINDOWS
