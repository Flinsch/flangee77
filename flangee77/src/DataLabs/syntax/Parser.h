#pragma once
#ifndef DL7_SYNTAX_PARSER_H
#define DL7_SYNTAX_PARSER_H

#include "./Grammar.h"
#include "./Token.h"

#include <CoreLabs/string.h>

#include <vector>



namespace dl7 {
namespace syntax {



class Parser
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    Parser(const Grammar& grammar);

    /**
     * Destructor.
     */
    virtual ~Parser() = default;

private:
    /** Default constructor. */
    Parser() = delete;
    /** Copy constructor. */
    Parser(const Parser&) = delete;
    /** Copy assignment operator. */
    Parser& operator = (const Parser&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The underlying grammar.
     */
    const Grammar& _grammar;



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * 
     */
    bool parse(cl7::u8string_view text);

    /**
     * 
     */
    bool parse(const std::vector<Token>& tokens);



    // #############################################################################
    // Prototypes
    // #############################################################################
private:
    //bool _parse_next(Token current, Token next);

}; // class Parser



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_PARSER_H
