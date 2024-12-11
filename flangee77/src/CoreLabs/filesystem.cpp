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
        auto* full_path_ptr = static_cast<cl7::char_type*>(_full_path);
        ::GetModuleFileName(nullptr, full_path_ptr, MAX_PATH);

        cl7::string_view full_path(full_path_ptr);
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
        auto* path_ptr = static_cast<cl7::char_type*>(_path);

        if (first_call)
        {
            first_call = false;

            DWORD length = ::GetCurrentDirectory(MAX_PATH, path_ptr);
            if (length == 0 || _path[length - 1] != TEXT('\\'))
                _path[length] = TEXT('\\');
        }

        return {path_ptr};
    }

    /**
     * Returns the current directory.
     */
    cl7::string get_current_directory()
    {
        cl7::char_type _path[MAX_PATH + 2] = {0};
        auto* path_ptr = static_cast<cl7::char_type*>(_path);

        DWORD length = ::GetCurrentDirectory(MAX_PATH, path_ptr);
        if (length == 0 || _path[length - 1] != TEXT('\\'))
            _path[length] = TEXT('\\');

        return {path_ptr};
    }

    /**
     * Returns the user directory.
     */
    cl7::string get_user_directory()
    {
        static cl7::char_type _path[MAX_PATH + 2] = {0};
        static bool first_call = true;
        auto* path_ptr = static_cast<cl7::char_type*>(_path);

        if (first_call)
        {
            first_call = false;

            DWORD length = 0;
            cl7::wchar_type* tmp_ptr = nullptr;
            HRESULT hresult = ::SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, nullptr, &tmp_ptr);

            if (hresult == S_OK)
            {
                cl7::string path = cl7::strings::from_utfx(tmp_ptr);
                for (const cl7::char_type* p = path.c_str(); *p; ++p)
                    _path[length++] = *p;
            }

            if (tmp_ptr) ::CoTaskMemFree(tmp_ptr);
            if (length == 0 || _path[length - 1] != TEXT('\\'))
                _path[length] = TEXT('\\');
        }

        return {path_ptr};
    }



} // namespace cl7::filesystem
