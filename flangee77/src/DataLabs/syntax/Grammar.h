#pragma once
#ifndef DL7_SYNTAX_GRAMMAR_H
#define DL7_SYNTAX_GRAMMAR_H

#include <CoreLabs/string.h>



namespace dl7 {
namespace syntax {



class Grammar
{

public:
    struct Config
    {
        /**
         * The character used as regex delimiter to distinguish regex/pattern-based
         * terminal symbols from literal terminal symbols.
         */
        cl7::u8char_type regex_delimiter = '/';
    }; // struct Config



    // #############################################################################
    // Attributes
    // #############################################################################
public:
    /**
     * The meta-configuration of the grammar.
     */
    Config config;

}; // class Grammar



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_GRAMMAR_H