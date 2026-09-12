#include "Binder.h"

#include <CoreLabs/text/codec.h>
#include <CoreLabs/text/format.h>

#include <charconv>



namespace dl7::config {



    // #############################################################################
    // Reading
    // #############################################################################

    void Binder::_load_string(const Value& value, string_t& target, const string_t& path)
    {
        if (!value.is_string())
        {
            _error(u8"A string is expected here.", path);
            return;
        }

        target = value.as_string();
    }

    void Binder::_load_boolean(const Value& value, bool& target, const string_t& path)
    {
        if (value.is_boolean())
        {
            target = value.as_boolean();
            return;
        }

        // INI and XML have no types to speak of, so a boolean
        // may well arrive spelled out as a string.
        if (value.is_string())
        {
            if (value.as_string() == u8"true") { target = true; return; }
            if (value.as_string() == u8"false") { target = false; return; }
        }

        _error(u8"A boolean is expected here.", path);
    }



    /**
     * Reads a scalar as an integer, accepting a string that reads as one. Returns
     * false (having reported) if the value is neither.
     */
    bool Binder::_read_integer(const Value& value, integer_t& target, const string_t& path)
    {
        if (value.is_integer())
        {
            target = value.as_integer();
            return true;
        }

        if (value.is_string())
        {
            const std::string_view chars{cl7::text::codec::reinterpret_utf8(value.as_string())};

            const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), target);
            if (ec == std::errc{} && ptr == chars.data() + chars.size())
                return true;
        }

        _error(u8"A whole number is expected here.", path);
        return false;
    }

    /**
     * Reads a scalar as a floating-point number, accepting a whole number as well
     * as a string that reads as one. Returns false (having reported) if the value is
     * none of those.
     */
    bool Binder::_read_float(const Value& value, float_t& target, const string_t& path)
    {
        if (value.is_number())
        {
            target = value.as_number<float_t>();
            return true;
        }

        if (value.is_string())
        {
            const std::string_view chars{cl7::text::codec::reinterpret_utf8(value.as_string())};

            const auto [ptr, ec] = std::from_chars(chars.data(), chars.data() + chars.size(), target, std::chars_format::general);
            if (ec == std::errc{} && ptr == chars.data() + chars.size())
                return true;
        }

        _error(u8"A number is expected here.", path);
        return false;
    }



    // #############################################################################
    // Diagnostics
    // #############################################################################

    /**
     * Extends a field path by one key, which is what the diagnostics point at: the
     * readers' trees hold no source positions, so there is no line to point at
     * instead.
     */
    string_t Binder::_extend(const string_t& path, cl7::u8string_view key)
    {
        if (path.empty())
            return string_t{key};

        return path + u8"." + string_t{key};
    }

    /**
     * Extends a field path by one sequence index.
     */
    string_t Binder::_index(const string_t& path, size_t index)
    {
        return path + u8"[" + cl7::text::format::to_string<string_t>(index) + u8"]";
    }



    void Binder::_error(cl7::u8string_view message, const string_t& path)
    {
        _diagnostics.add(syntax::Diagnostic::Severity::Error, path.empty() ? string_t{message} : path + u8": " + string_t{message});
    }

    void Binder::_warning(cl7::u8string_view message, const string_t& path)
    {
        _diagnostics.add(syntax::Diagnostic::Severity::Warning, path.empty() ? string_t{message} : path + u8": " + string_t{message});
    }



} // namespace dl7::config
