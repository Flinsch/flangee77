#pragma once
#ifndef PL7_ARGUMENTBAG_H
#define PL7_ARGUMENTBAG_H

#include <CoreLabs/string.h>

#include <vector>
#include <map>



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
     * Explicit constructor.
     */
    ArgumentBag(int argc, cl7::char_type* argv[]);

    /**
     * Destructor.
     */
    ~ArgumentBag() = default;

private:
    /** Default constructor. */
    ArgumentBag() = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    const cl7::string _empty_value;
    const std::vector<cl7::string> _empty_values;

    /**
     * The list of command-line arguments.
     */
    std::vector<cl7::string> _arguments;

    /**
     * The list of positional arguments (arguments that don't start with a "-").
     */
    std::vector<cl7::string> _positional_arguments;

    /**
     * The list of option names.
     */
    std::vector<cl7::string> _option_names;

    /**
     * The map of option names to option values.
     */
    std::map<cl7::string, std::vector<cl7::string>, std::less<>> _option_values;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the list of command-line arguments.
     */
    const std::vector<cl7::string>& get_arguments() const { return _arguments; }

    /**
     * Returns the list of positional arguments (arguments that don't start with a "-").
     */
    const std::vector<cl7::string>& get_positional_arguments() const { return _positional_arguments; }

    /**
     * Returns the list of option names.
     */
    const std::vector<cl7::string>& get_option_names() const { return _option_names; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Checks whether the specified option was passed to the application.
     */
    bool has_option(const cl7::string_view& option_name) const;

    /**
     * Returns the option value found for the specified option, or an empty string
     * if not found.
     */
    const cl7::string& get_value(const cl7::string_view& option_name) const;

    /**
     * Returns a list of option values found for the specified option, or an empty
     * list if not found.
     */
    const std::vector<cl7::string>& get_values(const cl7::string_view& option_name) const;

}; // class ArgumentBag



} // namespace pl7

#endif // PL7_ARGUMENTBAG_H
