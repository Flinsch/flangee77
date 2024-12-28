#include "filesystem.h"

#include <CoreLabs/strings.h>
#include <CoreLabs/auto_invoke.h>

#include <windows.h>
#include <shlobj.h>



namespace cl7::filesystem {



    /**
     * Returns the working directory of the module instance.
     */
    cl7::u8string get_working_directory()
    {
        /*wchar_t _full_path[MAX_PATH + 1];
        ::GetModuleFileNameW(nullptr, _full_path, MAX_PATH);

        std::wstring_view full_path{_full_path};
        size_t p = full_path.find_last_of(L'\\');
        return cl7::strings::to_utf8(full_path.substr(0, p + 1));*/
        return get_initial_directory();
    }

    /**
     * Returns the initial directory (where the execution started).
     */
    cl7::u8string get_initial_directory()
    {
        static wchar_t _path[MAX_PATH + 2] = {0};
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            DWORD length = ::GetCurrentDirectoryW(MAX_PATH, _path);

            if (length == 0 || _path[length - 1] != u8'\\')
                _path[length] = u8'\\';
        }

        return cl7::strings::to_utf8(_path);
    }

    /**
     * Returns the current directory.
     */
    cl7::u8string get_current_directory()
    {
        wchar_t _path[MAX_PATH + 2] = {0};

        DWORD length = ::GetCurrentDirectoryW(MAX_PATH, _path);

        if (length == 0 || _path[length - 1] != u8'\\')
            _path[length] = u8'\\';

        return cl7::strings::to_utf8(_path);
    }

    /**
     * Returns the user directory.
     */
    cl7::u8string get_user_directory()
    {
        static cl7::u8char_type _path[MAX_PATH + 2] = {0};
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            DWORD length = 0;
            wchar_t* tmp = nullptr;
            auto auto_free_tmp = cl7::finally([&tmp] { if (tmp) ::CoTaskMemFree(tmp); tmp = nullptr; });
            HRESULT hresult = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &tmp);

            if (hresult == S_OK)
            {
                assert(tmp);
                cl7::u8string path = cl7::strings::to_utf8(tmp);
                for (const cl7::u8char_type* p = path.c_str(); *p; ++p)
                    _path[length++] = *p;
            }

            if (length == 0 || _path[length - 1] != u8'\\')
                _path[length] = u8'\\';
        }

        return {_path};
    }



} // namespace cl7::filesystem
