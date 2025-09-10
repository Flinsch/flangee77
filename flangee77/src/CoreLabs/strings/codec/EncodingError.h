#ifndef CL7_STRINGS_CODEC_DECODEERROR_H
#define CL7_STRINGS_CODEC_DECODEERROR_H

#include <CoreLabs/root.h>



namespace cl7::strings::codec {



enum struct EncodingError : unsigned
{
    None                    = 0,
    /** A (decoded) code point is beyond the valid ASCII range up to 0x7F. */
    OutOfRangeAscii         = 1 << 0,
    /** A (decoded) code point is beyond the valid Unicode range up to U+10FFFF. */
    OutOfRangeUnicode       = 1 << 1,
    /** A (decoded) code point falls into the forbidden Unicode surrogate range U+D800 through U+DFFF. */
    DisallowedUnicode       = 1 << 2,
    /** There is absolutely no space left in the output buffer. */
    ExhaustedOutputSpace    = 1 << 3,
    /** The encoded representation of a code point does not fit within the available buffer space. */
    InsufficientOutputSpace = 1 << 4,
    /** An individual code unit is invalid or appears in an invalid context. */
    InvalidCodeUnit         = 1 << 5,
    /** A UTF-8 byte sequence is invalid. */
    InvalidSequence         = 1 << 6,
    /** A UTF-8/UTF-16 character sequence is truncated or missing bytes/words necessary for a valid code point. */
    IncompleteSequence      = 1 << 7,
    /** A UTF-8 character is encoded using more bytes than necessary. */
    OverlongEncoding        = 1 << 8,
    /** A UTF-16 "sequence" has an unexpected trailing/low surrogate without a leading/high surrogate. */
    UnpairedSurrogate       = 1 << 9,
};



inline EncodingError operator&(EncodingError a, EncodingError b)
{
    return static_cast<EncodingError>(static_cast<unsigned>(a) & static_cast<unsigned>(b));
}

inline EncodingError operator|(EncodingError a, EncodingError b)
{
    return static_cast<EncodingError>(static_cast<unsigned>(a) | static_cast<unsigned>(b));
}

inline EncodingError& operator|=(EncodingError& a, EncodingError b)
{
    a = a | b;
    return a;
}



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_DECODEERROR_H
