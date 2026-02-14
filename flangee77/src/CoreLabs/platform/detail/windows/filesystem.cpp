#include "filesystem.h"
#if F77_IS_WINDOWS

#include <CoreLabs/text/codec.h>
#include <CoreLabs/auto_invoke.h>

#include <windows.h>
#include <shlobj.h>



namespace cl7::platform::detail::windows::filesystem {



    cl7::u8string get_module_directory()
    {
        wchar_t _full_path[MAX_PATH + 1];

        ::GetModuleFileNameW(nullptr, _full_path, MAX_PATH);

        std::wstring_view full_path{_full_path};
        size_t p = full_path.find_last_of(L'\\');
        assert(p != std::wstring_view::npos);

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(full_path.substr(0, p + 1)));
    }

    cl7::u8string get_working_directory()
    {
        //return get_module_directory();
        return get_initial_directory();
    }

    cl7::u8string get_initial_directory()
    {
        static wchar_t _path[MAX_PATH + 2] = {0};
        static DWORD length = 0;
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            length = ::GetCurrentDirectoryW(MAX_PATH, _path);

            if (length == 0 || _path[length - 1] != L'\\')
                _path[length++] = L'\\';
        }

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(std::wstring_view{_path, length}));
    }

    cl7::u8string get_current_directory()
    {
        wchar_t _path[MAX_PATH + 2] = {0};

        DWORD length = ::GetCurrentDirectoryW(MAX_PATH, _path);

        if (length == 0 || _path[length - 1] != L'\\')
            _path[length++] = L'\\';

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(std::wstring_view{_path, length}));
    }

    cl7::u8string get_user_directory()
    {
        static wchar_t _path[MAX_PATH + 2] = {0};
        static DWORD length = 0;
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            wchar_t* tmp = nullptr;
            auto auto_free_tmp = cl7::finally([&tmp] { if (tmp) ::CoTaskMemFree(tmp); tmp = nullptr; });
            HRESULT hresult = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &tmp);

            if (hresult == S_OK)
            {
                assert(tmp);
                std::wstring_view path{tmp}; // SHGetKnownFolderPath stores a null-terminated Unicode string.
                for (const wchar_t* p = path.data(); *p; ++p)
                    _path[length++] = *p;
            }

            if (length == 0 || _path[length - 1] != L'\\')
                _path[length++] = L'\\';
        }

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(std::wstring_view{_path, length}));
    }

    cl7::u8string get_temp_directory()
    {
        wchar_t _path[MAX_PATH + 2] = {0};

        DWORD length = ::GetTempPathW(MAX_PATH, _path);

        if (length == 0 || _path[length - 1] != L'\\')
            _path[length++] = L'\\';

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(std::wstring_view{_path, length}));
    }



} // namespace cl7::platform::detail::windows::filesystem

#endif // F77_IS_WINDOWS
