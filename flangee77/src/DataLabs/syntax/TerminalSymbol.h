#pragma once
#ifndef DL7_SYNTAX_SYMBOL_H
#define DL7_SYNTAX_SYMBOL_H

#include <CoreLabs/string.h>

#include <regex>



namespace dl7::syntax {



struct TerminalSymbol
{

    /**
     * 0 is reserved for the "EOF" symbol. Any negative value indicates an
     * unrecognized character. Custom IDs can have any positive value.
     */
    using ID = int;

    /** The name of the symbol. */
    cl7::astring name;
    /** The terminal symbol ID. */
    ID id;

    TerminalSymbol(cl7::astring_view name, ID id);
    virtual ~TerminalSymbol() = default;

    virtual bool is_literal() const = 0;
    virtual size_t try_match_prefix(cl7::u8string_view source) const = 0;

}; // struct TerminalSymbol



struct LiteralSymbol
    : public TerminalSymbol
{

    /** The static string literal of the symbol. */
    cl7::u8string literal;

    LiteralSymbol(cl7::astring_view name, ID id, cl7::u8string_view literal);
    ~LiteralSymbol() override = default;

    bool is_literal() const override { return true; }
    size_t try_match_prefix(cl7::u8string_view source) const override;

}; // struct LiteralSymbol



struct PatternSymbol
    : public TerminalSymbol
{

    /** The dynamic regex pattern of the symbol. */
    cl7::astring pattern;
    /** The bitmask that contains options that govern how the regular expression behaves ('optimize' is always applied). */
    std::regex_constants::syntax_option_type syntax_options;
    /** The bitmask that specifies additional regular expression matching options ('match_continuous' is always applied). */
    std::regex_constants::match_flag_type match_flags;
    /** The regular expression object to be used for matching. */
    std::regex regex;
    /** The (optional) literal prefix of the symbol. */
    cl7::u8string literal_prefix;

    PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    PatternSymbol(cl7::astring_view name, ID id, cl7::astring_view pattern, cl7::u8string_view literal_prefix, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    ~PatternSymbol() override = default;

    bool is_literal() const override { return false; }
    size_t try_match_prefix(cl7::u8string_view source) const override;

}; // struct PatternSymbol



} // namespace dl7::syntax

#endif // DL7_SYNTAX_SYMBOL_H
