#ifndef CL7_IO_UTF8READER_H
#define CL7_IO_UTF8READER_H

#include "./IReadable.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class Utf8Reader
{

public:
    /**
     * Prepares a UTF-8 reader for reading from the specified readable object.
     */
    explicit Utf8Reader(IReadable* readable) noexcept;

    /**
     * Reads all available/remaining data without checking for valid UTF-8.
     */
    cl7::u8string read_all() const;

    /**
     * Attempts to read the next line without checking for valid UTF-8.
     */
    cl7::u8string read_line() const;

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) without checking for valid UTF-8.
     * However, due to the logic used to check for whitespace, it is practically
     * guaranteed that only valid UTF-8 code points will be processed.
     */
    cl7::u8string read_token() const;

    /**
     * Attempts to read a single Unicode codepoint.
     */
    cl7::u8string_view read_codepoint() const;

    /**
     * Attempts to read a single character. Returns 0 if not possible.
     */
    cl7::u8char_t read_char() const;

    /**
     * Reads all available/remaining data with UTF-8 validation/"correction".
     */
    cl7::u8string read_all_validated() const;

    /**
     * Attempts to read the next line with UTF-8 validation/"correction".
     */
    cl7::u8string read_line_validated() const;

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) with UTF-8 validation/"correction".
     */
    cl7::u8string read_token_validated() const;

    /**
     * Attempts to "peek" a single Unicode codepoint without extracting it.
     */
    cl7::u8string_view peek_codepoint() const;

    /**
     * "Peeks" a single character without extracting it. Returns 0 if not possible.
     */
    cl7::u8char_t peek_char() const;

private:
    /** The readable source object. */
    IReadable* _readable;

}; // class Utf8Reader



} // namespace cl7::io

#endif // CL7_IO_UTF8READER_H
