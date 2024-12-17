#include "ArgumentBag.h"

#include <CoreLabs/strings.h>

#include <regex>
#include <utility>



namespace pl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ArgumentBag::ArgumentBag(int argc, cl7::char_type* argv[])
        : ArgumentBag()
    {
        for (int i = 0; i < argc; ++i)
        {
            _arguments.emplace_back(argv[i]);
            _try_resolve_quotation(_arguments.back());
        }

        _parse_arguments();
    }

    ArgumentBag::ArgumentBag(const std::vector<cl7::string_view>& arguments)
        : ArgumentBag()
    {
        for (const auto& argument : arguments)
        {
            _arguments.emplace_back(argument);
            _try_resolve_quotation(_arguments.back());
        }

        _parse_arguments();
    }

    ArgumentBag::ArgumentBag(const std::vector<cl7::string>& arguments)
        : ArgumentBag()
    {
        for (const auto& argument : arguments)
        {
            _arguments.emplace_back(argument);
            _try_resolve_quotation(_arguments.back());
        }

        _parse_arguments();
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Checks whether the specified flags was passed to the application.
     */
    bool ArgumentBag::has_flag(cl7::string_view flag_name) const
    {
        return _flags.contains(flag_name);
    }

    /**
     * Checks whether the specified option was passed to the application.
     */
    bool ArgumentBag::has_option(cl7::string_view option_name) const
    {
        auto it = _option_values.find(option_name);
        return it != _option_values.end();
    }

    /**
     * Checks whether the specified option has a single assigned value.
     */
    bool ArgumentBag::has_single_value(cl7::string_view option_name) const
    {
        const std::vector<cl7::string>& values = get_values(option_name);
        return values.size() == 1;
    }

    /**
     * Returns the option value found for the specified option, or an empty string
     * if not found. If multiple values are assigned, the last value is returned.
     */
    const cl7::string& ArgumentBag::get_value(cl7::string_view option_name) const
    {
        const std::vector<cl7::string>& values = get_values(option_name);
        if (values.empty())
            return _empty_value;

        return values.back();
    }

    /**
     * Returns a list of option values found for the specified option, or an empty
     * list if not found.
     */
    const std::vector<cl7::string>& ArgumentBag::get_values(cl7::string_view option_name) const
    {
        auto it = _option_values.find(option_name);
        if (it == _option_values.end())
            return _empty_values;

        return it->second;
    }



    // #############################################################################
    // Helpers
    // #############################################################################

    void ArgumentBag::_parse_arguments()
    {
        cl7::string_view last_name{};

        for (const auto& argument : _arguments)
        {
            if (argument.starts_with(TEXT("-")))
            {
                _try_add_flag(last_name);
                if (argument.starts_with(TEXT("--")))
                    last_name = _parse_any_option(argument);
                else
                    last_name = _parse_short_option_or_flags(argument);
                continue;
            }

            if (!last_name.empty())
            {
                // next might be value
                _add_option_value(last_name, argument);
                last_name = {};
                continue;
            }

            _positional_arguments.push_back(argument);
        } // for each argument

        _try_add_flag(last_name);
    }

    cl7::string_view ArgumentBag::_parse_short_option_or_flags(cl7::string_view argument)
    {
        assert(!argument.empty());
        assert(argument[0] == TEXT('-'));

        if (argument.length() < 2)
            return {}; // No actual option/flag name

        assert(argument[1] != TEXT('-'));

        // Is this a single-character option/flag name?
        // Return the name to handle any subsequent
        // assignment or treat it as a flag afterward.
        if (argument.length() == 2)
            return argument.substr(1);

        // Is this an explicit assignment?
        // Then treat it as a potential option.
        if (argument.length() > 2 && argument[2] == TEXT('='))
            return _parse_any_option(argument);

        // Parse as many flags as there are.
        for (size_t i = 1; i < argument.length(); ++i)
            _add_flag(argument.substr(i, 1));

        // All flags processed. No subsequent assignment to be considered.
        return {};
    }

    cl7::string_view ArgumentBag::_parse_any_option(cl7::string_view argument)
    {
        assert(!argument.empty());
        assert(argument[0] == TEXT('-'));

        size_t l = argument.find_first_not_of(TEXT('-'));
        if (l == cl7::string_view::npos)
            return {}; // No actual option name.

        size_t r = argument.find_first_of(TEXT('='), l + 1);
        if (r == cl7::string_view::npos)
            r = argument.length();

        const cl7::string_view option_name = argument.substr(l, r - l);
        if (option_name.empty())
            return {}; // Still no actual option name.

        // No value assignment? Return the name to handle any
        // subsequent assignment or treat it as a flag afterward.
        if (r + 1 >= argument.length())
            return option_name;

        const cl7::string_view option_value = argument.substr(r + 1);
        assert(!option_value.empty());

        // Add option-value assignment.
        _add_option_value(option_name, option_value);

        // Option processed. No subsequent assignment to be considered.
        return {};
    }

    void ArgumentBag::_try_add_flag(cl7::string_view flag_name)
    {
        if (flag_name.empty())
            return;

        _add_flag(flag_name);
    }

    void ArgumentBag::_add_flag(cl7::string_view flag_name)
    {
        assert(!flag_name.empty());

        _flags.insert(cl7::string(flag_name));
    }

    void ArgumentBag::_add_option_value(cl7::string_view option_name, cl7::string_view option_value)
    {
        assert(!option_name.empty());
        assert(!option_value.empty());

        auto it = _option_values.find(option_name);
        if (it == _option_values.end())
        {
            _option_names.emplace_back(option_name);
            it = _option_values.emplace(option_name, std::vector<cl7::string>{}).first;
        }

        auto* values = &it->second;
        values->emplace_back(option_value);

        _try_resolve_quotation(values->back());
    }

    void ArgumentBag::_try_resolve_quotation(cl7::string& option_value)
    {
        if (option_value.length() < 2)
            return;

        if (option_value.front() == TEXT('"') && option_value.back() == TEXT('"'))
        {
            std::basic_regex<cl7::char_type> regex{TEXT(R"(^"[^"\\]*(?:\\.[^"\\]*)*"$)")};
            std::match_results<const cl7::char_type*> m;
            if (std::regex_match(option_value.data(), m, regex))
                _resolve_quotation(option_value, TEXT('"'));
        }

        if (option_value.front() == TEXT('\'') && option_value.back() == TEXT('\''))
        {
            std::basic_regex<cl7::char_type> regex{TEXT(R"(^'[^'\\]*(?:\\.[^'\\]*)*'$)")};
            std::match_results<const cl7::char_type*> m;
            if (std::regex_match(option_value.data(), m, regex))
                _resolve_quotation(option_value, TEXT('\''));
        }
    }

    void ArgumentBag::_resolve_quotation(cl7::string& option_value, cl7::char_type ch)
    {
        assert(option_value.length() >= 2);

        cl7::string tmp = option_value.substr(1, option_value.length() - 2);

        option_value.clear();
        for (size_t i = 0; i < tmp.length(); ++i)
        {
            const bool escape =
                tmp[i] == TEXT('\\') &&
                i + 1 < tmp.length() &&
                tmp[i + 1] == TEXT('"');

            if (escape)
                ++i;

            option_value += tmp[i];
        } // for ...
    }



} // namespace pl7
