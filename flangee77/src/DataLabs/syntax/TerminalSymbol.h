#ifndef DL7_SYNTAX_TERMINALSYMBOL_H
#define DL7_SYNTAX_TERMINALSYMBOL_H

#include "./SymbolID.h"

#include <CoreLabs/string.h>

#include <regex>



namespace dl7::syntax {



/**
 * Represents a terminal symbol in the lexer and/or of a grammar.
 */
struct TerminalSymbol
{

    /** The terminal symbol ID. */
    SymbolID id;

    TerminalSymbol(SymbolID id);
    virtual ~TerminalSymbol() = default;

    /** Returns true if the symbol corresponds to a fixed string, false otherwise. */
    virtual bool is_literal() const = 0;

    /** Attempts to match this symbol at the beginning of the given source text. */
    virtual size_t try_match_next(cl7::u8string_view source) const = 0;

}; // struct TerminalSymbol



/**
 * Represents a terminal symbol that is a fixed string literal.
 */
struct LiteralSymbol
    : public TerminalSymbol
{

    /** The static string literal of the symbol. */
    cl7::u8string literal;

    LiteralSymbol(SymbolID id, cl7::u8string_view literal);
    ~LiteralSymbol() override = default;

    /** Always returns true since this is a literal symbol. */
    bool is_literal() const final { return true; }

    /** Attempts to match the literal at the beginning of the given source text. */
    size_t try_match_next(cl7::u8string_view source) const override;

}; // struct LiteralSymbol



/**
 * Represents a terminal symbol that is not a fixed string literal (for
 * dynamically-matched symbols such as regular expressions or custom matchers).
 */
struct NonLiteralSymbol
    : public TerminalSymbol
{

    NonLiteralSymbol(SymbolID id);
    ~NonLiteralSymbol() override = default;

    /** Always returns false since this is a non-literal symbol. */
    bool is_literal() const final { return false; }

}; // struct NonLiteralSymbol



/**
 * Represents a terminal symbol that is matched using a regular expression.
 */
struct PatternSymbol
    : public NonLiteralSymbol
{

    /** The (optional) literal prefix of the symbol. */
    cl7::u8string literal_prefix;

    /** The dynamic regex pattern of the symbol. */
    std::string pattern;
    /** The bitmask that contains options that govern how the regular expression behaves ('optimize' is always applied). */
    std::regex_constants::syntax_option_type syntax_options;
    /** The bitmask that specifies additional regular expression matching options ('match_continuous' is always applied). */
    std::regex_constants::match_flag_type match_flags;
    /** The regular expression object to be used for matching. */
    std::regex regex;

    PatternSymbol(SymbolID id, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    PatternSymbol(SymbolID id, cl7::u8string_view literal_prefix, std::string_view pattern, std::regex_constants::syntax_option_type syntax_options = std::regex_constants::ECMAScript, std::regex_constants::match_flag_type match_flags = std::regex_constants::match_default);
    ~PatternSymbol() override = default;

    /** Attempts to match the pattern against the beginning of the source text. */
    size_t try_match_next(cl7::u8string_view source) const override;

}; // struct PatternSymbol



/**
 * A concept that defined the interface for a custom prefix matcher used in custom
 * terminal symbols.
 *
 * A custom prefix matcher is a callable object (e.g., a struct with `operator()`)
 * that returns the number of characters it successfully matched at the beginning of
 * a source text. If no match is found, it returns 0.
 */
template <typename TPrefixMatcher>
concept PrefixMatcher = requires(TPrefixMatcher matcher)
{
    { matcher(cl7::u8string_view{}) } -> std::convertible_to<size_t>;
};

/**
 * Represents a terminal symbol that is matched using a user-defined function.
 */
template <PrefixMatcher TPrefixMatcher>
struct CustomSymbol
    : public NonLiteralSymbol
{

    using PrefixMatcher = TPrefixMatcher;

    /** A user-defined function or object that performs the custom matching logic. */
    PrefixMatcher matcher;

    CustomSymbol(SymbolID id, PrefixMatcher matcher)
        : NonLiteralSymbol(id)
        , matcher(matcher)
    {}
    ~CustomSymbol() override = default;

    /** Attempts to match this symbol using the custom matcher. */
    size_t try_match_next(cl7::u8string_view source) const override
    {
        return matcher(source);
    }

}; // struct CustomSymbol



} // namespace dl7::syntax

#endif // DL7_SYNTAX_TERMINALSYMBOL_H
