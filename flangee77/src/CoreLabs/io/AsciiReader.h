#ifndef CL7_IO_ASCIIREADER_H
#define CL7_IO_ASCIIREADER_H

#include "./IReadable.h"

#include "CoreLabs/string.h"



namespace cl7::io {



class AsciiReader
{

public:
    /**
     * Prepares a ASCII reader for reading from the specified readable object.
     */
    explicit AsciiReader(IReadable* readable) noexcept;

    /**
     * Reads all available/remaining data without checking for valid ASCII.
     */
    cl7::astring read_all() const;

    /**
     * Attempts to read the next line without checking for valid ASCII.
     */
    cl7::astring read_line() const;

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) without checking for valid ASCII.
     */
    cl7::astring read_token() const;

    /**
     * Attempts to read a single character. Returns 0 if not possible.
     */
    cl7::achar_t read_char() const;

    /**
     * Reads all available/remaining data with ASCII validation/"correction".
     */
    cl7::astring read_all_validated() const;

    /**
     * Attempts to read the next line with ASCII validation/"correction".
     */
    cl7::astring read_line_validated() const;

    /**
     * Attempts to read a single non-whitespace token/word (potentially skipping
     * preceding whitespace) with ASCII validation/"correction".
     */
    cl7::astring read_token_validated() const;

    /**
     * "Peeks" a single character without extracting it. Returns 0 if not possible.
     */
    cl7::achar_t peek_char() const;

private:
    /** The readable source object. */
    IReadable* _readable;

}; // class AsciiReader



} // namespace cl7::io

#endif // CL7_IO_ASCIIREADER_H
