#pragma once
#ifndef PL7_ARGUMENTBAG_H
#define PL7_ARGUMENTBAG_H

#include <CoreLabs/string.h>

#include <vector>
#include <unordered_set>
#include <unordered_map>



namespace pl7 {



/**
 * 
 */
class ArgumentBag
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    ArgumentBag() = default;

    /**
     * Explicit constructor.
     */
    ArgumentBag(int argc, cl7::char_type* argv[]);

    /**
     * Explicit constructor.
     */
    ArgumentBag(const std::vector<cl7::string_view>& arguments);

    /**
     * Explicit constructor.
     */
    ArgumentBag(const std::vector<cl7::string>& arguments);



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    cl7::string _empty_value;
    std::vector<cl7::string> _empty_values;

    /**
     * The "list" of command-line arguments.
     */
    std::vector<cl7::string> _arguments;

    /**
     * The "list" of positional arguments (arguments that don't start with a "-").
     */
    std::vector<cl7::string> _positional_arguments;

    /**
     * The set of flags ("options" without a value).
     */
    std::unordered_set<cl7::string, cl7::string_hash<>, std::equal_to<>> _flags;

    /**
     * The "list" of option names (actual options with assigned values).
     */
    std::vector<cl7::string> _option_names;

    /**
     * The map/assignment of option names to option values.
     */
    std::unordered_map<cl7::string, std::vector<cl7::string>, cl7::string_hash<>, std::equal_to<>> _option_values;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the "list" of command-line arguments.
     */
    const std::vector<cl7::string>& get_arguments() const { return _arguments; }

    /**
     * Returns the "list" of positional arguments (arguments that don't start with a "-").
     */
    const std::vector<cl7::string>& get_positional_arguments() const { return _positional_arguments; }

    /**
     * Returns the set of flags ("options" without a value).
     */
    const std::unordered_set<cl7::string, cl7::string_hash<>, std::equal_to<>>& get_flags() const { return _flags; }

    /**
     * Returns the "list" of option names (actual options with assigned values).
     */
    const std::vector<cl7::string>& get_option_names() const { return _option_names; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Checks whether the specified flags was passed to the application.
     */
    bool has_flag(cl7::string_view flag_name) const;

    /**
     * Checks whether the specified option was passed to the application.
     */
    bool has_option(cl7::string_view option_name) const;

    /**
     * Checks whether the specified option has a single assigned value.
     */
    bool has_single_value(cl7::string_view option_name) const;

    /**
     * Returns the option value found for the specified option, or an empty string
     * if not found. If multiple values are assigned, the last value is returned.
     */
    const cl7::string& get_value(cl7::string_view option_name) const;

    /**
     * Returns a "list" of option values found for the specified option, or an empty
     * "list" if not found.
     */
    const std::vector<cl7::string>& get_values(cl7::string_view option_name) const;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    void _parse_arguments();
    cl7::string_view _parse_short_option_or_flags(cl7::string_view argument);
    cl7::string_view _parse_any_option(cl7::string_view argument);
    void _try_add_flag(cl7::string_view flag_name);
    void _add_flag(cl7::string_view flag_name);
    void _add_option_value(cl7::string_view option_name, cl7::string_view option_value);
    static void _try_resolve_quotation(cl7::string& option_value);
    static void _resolve_quotation(cl7::string& option_value, cl7::char_type ch);

}; // class ArgumentBag



} // namespace pl7

#endif // PL7_ARGUMENTBAG_H
