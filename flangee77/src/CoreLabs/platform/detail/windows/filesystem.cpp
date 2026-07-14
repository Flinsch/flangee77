#include "filesystem.h"
#if F77_IS_WINDOWS

#include <CoreLabs/text/codec.h>
#include <CoreLabs/auto_invoke.h>

#include <windows.h>
#include <shlobj.h>



namespace cl7::platform::detail::windows::filesystem {



namespace {

    /**
     * Ensures `path` (of the given `length`, capacity `MAX_PATH + 2`) ends in a
     * trailing backslash, and converts it to UTF-8.
     */
    cl7::u8string _finalize_path(wchar_t* path, DWORD length)
    {
        if (length == 0 || path[length - 1] != L'\\')
            path[length++] = L'\\';

        return cl7::text::codec::to_utf8(cl7::text::codec::reinterpret_utfx(std::wstring_view{path, length}));
    }

    /**
     * Retrieves a directory into a freshly stack-allocated buffer via `retrieve`
     * (which writes into the given buffer, of capacity `MAX_PATH`, and returns the
     * resulting length), on every call.
     */
    template <typename Tretrieve>
        requires(requires (const Tretrieve& retrieve, wchar_t* path) {
            { retrieve(path) } -> std::convertible_to<DWORD>;
        })
    cl7::u8string _get_directory(const Tretrieve& retrieve)
    {
        wchar_t path[MAX_PATH + 2] = {0};
        return _finalize_path(path, retrieve(path));
    }

    /**
     * Like `_get_directory`, but only invokes `retrieve` once and caches the
     * result for all later calls. Since this is a template, every distinct
     * `retrieve` (i.e., every caller passing its own lambda) triggers its own
     * instantiation of this function and thus gets its own, independent static
     * cache, without any caller having to write its own caching logic.
     */
    template <typename Tretrieve>
        requires(requires (const Tretrieve& retrieve, wchar_t* path) {
            { retrieve(path) } -> std::convertible_to<DWORD>;
        })
    cl7::u8string _get_cached_directory(const Tretrieve& retrieve)
    {
        static wchar_t _path[MAX_PATH + 2] = {0};
        static DWORD length = 0;
        static bool first_call = true;

        if (first_call)
        {
            first_call = false;
            length = retrieve(_path);
        }

        return _finalize_path(_path, length);
    }

    /**
     * Retrieves the known folder identified by `folder_id` into `path` (of capacity
     * `MAX_PATH`), returning the resulting length, or 0 on failure. Meant to be
     * used as (part of) a `retrieve` callback via those "get directory" functions.
     */
    DWORD _get_known_folder_path(REFKNOWNFOLDERID folder_id, wchar_t* path)
    {
        DWORD length = 0;

        wchar_t* tmp = nullptr;
        auto auto_free_tmp = cl7::finally([&tmp] { if (tmp) ::CoTaskMemFree(tmp); tmp = nullptr; });
        HRESULT hresult = ::SHGetKnownFolderPath(folder_id, KF_FLAG_CREATE, nullptr, &tmp);

        if (hresult == S_OK)
        {
            assert(tmp);
            std::wstring_view src{tmp}; // SHGetKnownFolderPath stores a null-terminated Unicode string.
            for (const wchar_t* p = src.data(); *p; ++p)
                path[length++] = *p;
        }

        return length;
    }

} // namespace



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
        return _get_cached_directory([](wchar_t* path) -> DWORD {
            return ::GetCurrentDirectoryW(MAX_PATH, path);
        });
    }

    cl7::u8string get_current_directory()
    {
        return _get_directory([](wchar_t* path) -> DWORD {
            return ::GetCurrentDirectoryW(MAX_PATH, path);
        });
    }

    cl7::u8string get_user_directory()
    {
        return _get_cached_directory([](wchar_t* path) -> DWORD {
            return _get_known_folder_path(FOLDERID_RoamingAppData, path);
        });
    }

    cl7::u8string get_temp_directory()
    {
        return _get_directory([](wchar_t* path) -> DWORD {
            return ::GetTempPathW(MAX_PATH, path);
        });
    }

    cl7::u8string get_cache_directory()
    {
        return _get_cached_directory([](wchar_t* path) -> DWORD {
            return _get_known_folder_path(FOLDERID_LocalAppData, path);
        });
    }



} // namespace cl7::platform::detail::windows::filesystem

#endif // F77_IS_WINDOWS
