#pragma once
#ifndef DL7_SYNTAX_SYMBOL_H
#define DL7_SYNTAX_SYMBOL_H

#include <CoreLabs/string.h>

#include <regex>



namespace dl7 {
namespace syntax {



struct TerminalSymbol
{

    /**
     * 0 is reserved for the "EOF" symbol. Any negative value indicates an
     * unrecognized character. Custom IDs can have any positive value.
     */
    typedef unsigned ID;

    /** The name of the symbol. */
    const cl7::astring name;
    /** The terminal symbol ID. */
    const ID id;

    TerminalSymbol(cl7::astring_view name, ID id);
    virtual ~TerminalSymbol() = default;

    virtual bool is_literal() const = 0;
    virtual size_t try_match_prefix(cl7::u8string_view source) const = 0;

}; // struct TerminalSymbol



struct LiteralSymbol
    : public TerminalSymbol
{

    /** The static string literal of the symbol. */
    const cl7::u8string literal;

    LiteralSymbol(cl7::astring_view name, ID id, cl7::u8string_view literal);
    virtual ~LiteralSymbol() = default;

    virtual bool is_literal() const { return true; }
    virtual size_t try_match_prefix(cl7::u8string_view source) const;

}; // struct LiteralSymbol



struct PatternSymbol
    : public TerminalSymbol
{

    /** The dynamic regex pattern of the symbol. */
    const cl7::astring pattern;
    /** The bitmask that contains options that govern how the regular expression behaves ('optimize' is always applied). */
    const std::regex_constants::syntax_option_type syntax_options;
    /** The bitmask that specifies additional regular expression matching options ('match_continuous' is always applied). */
    const std::regex_constants::match_flag_type match_flags;
    /** The regular expression object to be used for matching. */
    const std::regex regex;
    /** The (optional) literal prefix of the symbol. */
    const cl7::u8string literal_prefix;

    PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, cl7::u8string_view literal_prefix, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    virtual ~PatternSymbol() = default;

    virtual bool is_literal() const { return false; }
    virtual size_t try_match_prefix(cl7::u8string_view source) const;

}; // struct PatternSymbol



} // namespace syntax
} // namespace dl7

#endif // DL7_SYNTAX_SYMBOL_H
