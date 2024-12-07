#include "TerminalSymbol.h"



namespace dl7 {
namespace syntax {



    static size_t _try_match_prefix(cl7::u8string_view literal, cl7::u8string_view source)
    {
        if ( source.length() < literal.length() )
            return 0;

        if ( literal != source.substr( 0, literal.length() ) )
            return 0;

        return literal.length();
    }



    TerminalSymbol::TerminalSymbol(cl7::astring_view name, ID id)
        : name( name )
        , id( id )
    {
    }



    LiteralSymbol::LiteralSymbol(cl7::astring_view name, ID id, cl7::u8string_view literal)
        : TerminalSymbol( name, id )
        , literal( literal )
    {
    }

    size_t LiteralSymbol::try_match_prefix(cl7::u8string_view source) const
    {
        return _try_match_prefix( literal, source );
    }



    PatternSymbol::PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
        : PatternSymbol( name, id, pattern, {}, syntax_options, match_flags )
    {
    }

    PatternSymbol::PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, cl7::u8string_view literal_prefix, std::regex_constants::syntax_option_type syntax_options, std::regex_constants::match_flag_type match_flags)
        : TerminalSymbol( name, id )
        , pattern( pattern )
        , syntax_options( syntax_options | std::regex_constants::optimize )
        , match_flags( match_flags | std::regex_constants::match_continuous )
        , regex( this->pattern, this->syntax_options )
        , literal_prefix( literal_prefix )
    {
    }

    size_t PatternSymbol::try_match_prefix(cl7::u8string_view source) const
    {
        if ( !literal_prefix.empty() && _try_match_prefix( literal_prefix, source ) == 0 )
            return 0;

        const char* const data = reinterpret_cast<const char*>( source.data() );

        std::cmatch m;
        if ( !std::regex_search( data, data + source.length(), m, regex, match_flags ) )
            return 0;

        assert( m.position() == 0 );
        if ( m.position() != 0 )
            return 0;

        const auto& sm = m[ 0 ];
        assert( sm.matched );
        assert( _try_match_prefix( { reinterpret_cast<const cl7::u8char_type*>( sm.first ), static_cast<size_t>( sm.second - sm.first ) }, source ) == m.length() );
        return m.length();
    }



} // namespace syntax
} // namespace dl7
