#include "filesystem.h"

#include <CoreLabs/strings.h>

#include <windows.h>
#include <shlobj.h>



namespace cl7::filesystem {



    /**
     * Returns the working directory of the module instance.
     */
    cl7::string get_working_directory()
    {
        /*cl7::char_type _full_path[MAX_PATH + 1];
        ::GetModuleFileName(nullptr, _full_path, MAX_PATH);

        cl7::string_view full_path(_full_path);
        size_t p = full_path.find_last_of(TEXT('\\'));
        return cl7::string{full_path.substr(0, p + 1)};*/
        return get_initial_directory();
    }

    /**
     * Returns the initial directory (where the execution started).
     */
    cl7::string get_initial_directory()
    {
        static cl7::char_type _path[MAX_PATH + 2] = {0};
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            DWORD length = ::GetCurrentDirectory(MAX_PATH, _path);
            if (length == 0 || _path[length - 1] != TEXT('\\'))
                _path[length] = TEXT('\\');
        }

        return {_path};
    }

    /**
     * Returns the current directory.
     */
    cl7::string get_current_directory()
    {
        cl7::char_type _path[MAX_PATH + 2] = {0};

        DWORD length = ::GetCurrentDirectory(MAX_PATH, _path);
        if (length == 0 || _path[length - 1] != TEXT('\\'))
            _path[length] = TEXT('\\');

        return {_path};
    }

    /**
     * Returns the user directory.
     */
    cl7::string get_user_directory()
    {
        static cl7::char_type _path[MAX_PATH + 2] = {0};
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;

            DWORD length = 0;
            cl7::wchar_type* tmp = nullptr;
            HRESULT hresult = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &tmp);

            if (hresult == S_OK)
            {
                cl7::string path = cl7::strings::from_utfx(tmp);
                for (const cl7::char_type* p = path.c_str(); *p; ++p)
                    _path[length++] = *p;
            }

            if (tmp) ::CoTaskMemFree(tmp);
            if (length == 0 || _path[length - 1] != TEXT('\\'))
                _path[length] = TEXT('\\');
        }

        return {_path};
    }



} // namespace cl7::filesystem
