#include "Parser.h"



namespace dl7::syntax {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Explicit constructor.
     */
    Parser::Parser(const Grammar* grammar)
        : _grammar(grammar)
    {
        assert(_grammar);
    }



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * 
     */
    bool Parser::parse(cl7::u8string_view text)
    {
        _grammar->config;
        return false;
    }

    /**
     * 
     */
    bool Parser::parse(const std::vector<Token>& tokens)
    {
        _grammar->config;
        return false;
    }



} // namespace dl7::syntax
