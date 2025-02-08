#ifndef CL7_STRINGS_ASCIICODEC_H
#define CL7_STRINGS_ASCIICODEC_H
#include "./Codec.h"



namespace cl7::strings {



class AsciiCodecImpl
{

public:
    /**
     * Processes the specified Unicode code point and stores its corresponding
     * character encoding. Returns true if the code point is valid and can be
     * represented in ASCII.
     */
    bool init(codepoint codepoint);

    /**
     * Processes the specified code unit and updates the current state accordingly.
     * After that we have either a valid, invalid, or incomplete Unicode code point.
     */
    CodecResult input(cl7::achar_type ac, const CodecResult& base_result);

    /** Returns the (current) character encoding. */
    std::basic_string_view<cl7::achar_type> get_encoded() const { return {&_ac, 1}; }

private:
    cl7::achar_type _ac = 0;

}; // class AsciiCodecImpl



using AsciiCodec = Codec<cl7::achar_type, AsciiCodecImpl>;



} // namespace cl7::strings

#endif // CL7_STRINGS_ASCIICODEC_H
