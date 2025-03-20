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

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    ArgumentBag() = default;

    ArgumentBag(int argc, char* argv[]);
    ArgumentBag(int argc, wchar_t* argv[]);
    ArgumentBag(int argc, cl7::u8char_t* argv[]);
    ArgumentBag(const std::vector<cl7::u8string_view>& arguments);
    ArgumentBag(const std::vector<cl7::u8string>& arguments);



    // #############################################################################
    // Properties
    // #############################################################################

    /**
     * Returns the "list" of command-line arguments.
     */
    const std::vector<cl7::u8string>& get_arguments() const { return _arguments; }

    /**
     * Returns the "list" of positional arguments (arguments that don't start with a "-").
     */
    const std::vector<cl7::u8string>& get_positional_arguments() const { return _positional_arguments; }

    /**
     * Returns the set of flags ("options" without a value).
     */
    const std::unordered_set<cl7::u8string, cl7::string_hash<>, std::equal_to<>>& get_flags() const { return _flags; }

    /**
     * Returns the "list" of option names (actual options with assigned values).
     */
    const std::vector<cl7::u8string>& get_option_names() const { return _option_names; }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Checks whether the specified flags was passed to the application.
     */
    bool has_flag(cl7::u8string_view flag_name) const;

    /**
     * Checks whether the specified option was passed to the application.
     */
    bool has_option(cl7::u8string_view option_name) const;

    /**
     * Checks whether the specified option has a single assigned value.
     */
    bool has_single_value(cl7::u8string_view option_name) const;

    /**
     * Returns the option value found for the specified option, or an empty string
     * if not found. If multiple values are assigned, the last value is returned.
     */
    const cl7::u8string& get_value(cl7::u8string_view option_name) const;

    /**
     * Returns a "list" of option values found for the specified option, or an empty
     * "list" if not found.
     */
    const std::vector<cl7::u8string>& get_values(cl7::u8string_view option_name) const;



private:

    // #############################################################################
    // Helpers
    // #############################################################################

    void _parse_arguments();
    cl7::u8string_view _parse_short_option_or_flags(cl7::u8string_view argument);
    cl7::u8string_view _parse_any_option(cl7::u8string_view argument);
    void _try_add_flag(cl7::u8string_view flag_name);
    void _add_flag(cl7::u8string_view flag_name);
    void _add_option_value(cl7::u8string_view option_name, cl7::u8string_view option_value);
    static void _try_resolve_quotation(cl7::u8string& option_value);
    static void _resolve_quotation(cl7::u8string& option_value, cl7::u8char_t ch);



    // #############################################################################
    // Attributes
    // #############################################################################

    cl7::u8string _empty_value;
    std::vector<cl7::u8string> _empty_values;

    /**
     * The "list" of command-line arguments.
     */
    std::vector<cl7::u8string> _arguments;

    /**
     * The "list" of positional arguments (arguments that don't start with a "-").
     */
    std::vector<cl7::u8string> _positional_arguments;

    /**
     * The set of flags ("options" without a value).
     */
    std::unordered_set<cl7::u8string, cl7::string_hash<>, std::equal_to<>> _flags;

    /**
     * The "list" of option names (actual options with assigned values).
     */
    std::vector<cl7::u8string> _option_names;

    /**
     * The map/assignment of option names to option values.
     */
    std::unordered_map<cl7::u8string, std::vector<cl7::u8string>, cl7::string_hash<>, std::equal_to<>> _option_values;

}; // class ArgumentBag



} // namespace pl7

#endif // PL7_ARGUMENTBAG_H
