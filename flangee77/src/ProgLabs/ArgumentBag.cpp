#include "ArgumentBag.h"

#include <CoreLabs/strings.h>



namespace pl7 {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    ArgumentBag::ArgumentBag(int argc, cl7::char_type* argv[])
        : _arguments()
        , _positional_arguments()
        , _option_names()
        , _option_values()
    {
        bool next_is_value = false;
        std::vector<cl7::string>* values = NULL;

        for ( int i = 0; i < argc; ++i )
        {
            _arguments.push_back( cl7::string( argv[ i ] ) );
            const cl7::string& argument = _arguments.back();

            if ( next_is_value )
            {
                assert( values );
                values->push_back( argument );
                next_is_value = false;
                values = NULL;
                continue;
            }

            assert( argument.length() > 0 );
            if ( argument[0] != TEXT('-') )
            {
                _positional_arguments.push_back( argument );
                continue;
            }

            size_t l = argument.find_first_not_of( TEXT('-') );
            if ( l == argument.npos )
                continue;

            size_t r = argument.find_first_of( TEXT('='), l + 1 );
            if ( r == argument.npos )
                r = argument.length();

            const cl7::string option_name = cl7::strings::trimmed( argument.substr( l, r - l ) );
            if ( option_name.length() == 0 )
                continue;

            auto it = _option_values.find( option_name );
            if ( it == _option_values.end() )
            {
                _option_names.push_back( option_name );
                it = _option_values.emplace( option_name, std::vector<cl7::string>{} ).first;
            }

            values = &it->second;

            if ( r >= argument.length() )
            {
                next_is_value = argument.length() == 2 && ((argument[1] >= TEXT('A') && argument[1] <= TEXT('Z')) || (argument[1] >= TEXT('a') && argument[1] <= TEXT('z')));
                continue;
            }

            const cl7::string option_value = cl7::strings::trimmed( argument.substr( r + 1 ) );
            if ( option_value.length() == 0 )
                continue;

            values->push_back( option_value );
        } // for each argument
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * Checks whether the specified option was passed to the application.
     */
    bool ArgumentBag::has_option(const cl7::string& option_name) const
    {
        auto it = _option_values.find( option_name );
        return it != _option_values.end();
    }

    /**
     * Returns the option value found for the specified option, or an empty string
     * if not found.
     */
    const cl7::string& ArgumentBag::get_value(const cl7::string& option_name) const
    {
        auto it = _option_values.find( option_name );
        if ( it == _option_values.end() )
            return _empty_value;

        const std::vector<cl7::string>& values = it->second;
        if ( values.size() == 0 )
            return _empty_value;

        return values.back();
    }

    /**
     * Returns a list of option values found for the specified option, or an empty
     * list if not found.
     */
    const std::vector<cl7::string>& ArgumentBag::get_values(const cl7::string& option_name) const
    {
        auto it = _option_values.find( option_name );
        if ( it == _option_values.end() )
            return _empty_values;

        return it->second;
    }



} // namespace pl7
