#ifndef DL7_SYNTAX_PARSER_H
#define DL7_SYNTAX_PARSER_H

#include "./Grammar.h"
#include "./Token.h"

#include <CoreLabs/string.h>

#include <vector>



namespace dl7::syntax {



class Parser
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    Parser() = delete;

    Parser(const Grammar* grammar);

    Parser(const Parser&) = delete;
    Parser& operator=(const Parser&) = delete;
    Parser(Parser&&) = delete;
    Parser& operator=(Parser&&) = delete;

    virtual ~Parser() = default;



    // #############################################################################
    // Methods
    // #############################################################################

    /**
     * 
     */
    bool parse(cl7::u8string_view text);

    /**
     * 
     */
    bool parse(const std::vector<Token>& tokens);



private:

    // #############################################################################
    // Prototypes
    // #############################################################################

    //bool _parse_next(Token current, Token next);



    // #############################################################################
    // Attributes
    // #############################################################################

    /**
     * The underlying grammar.
     */
    const Grammar* _grammar;

}; // class Parser



} // namespace dl7::syntax

#endif // DL7_SYNTAX_PARSER_H
