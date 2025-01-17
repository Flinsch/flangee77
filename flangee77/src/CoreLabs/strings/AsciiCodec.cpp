#include "AsciiCodec.h"



namespace cl7::strings {



    /**
     * Processes the specified Unicode code point and stores its corresponding
     * character encoding. Returns true if the code point is valid and can be
     * represented in ASCII.
     */
    bool AsciiCodecImpl::init(codepoint codepoint)
    {
        if (codepoint.is_valid_ascii())
        {
            _ac = static_cast<cl7::achar_type>(codepoint.value);
            return true;
        }

        _ac = static_cast<cl7::achar_type>(codepoint::replacement_ascii);
        return false;
    }

    /**
     * Processes the specified code unit and updates the current state accordingly.
     * After that we have either a valid, invalid, or incomplete Unicode code point.
     */
    CodecResult AsciiCodecImpl::input(cl7::achar_type ac, const CodecResult& base_result)
    {
        _ac = ac;

        codepoint codepoint{static_cast<unsigned>(_ac)};
        if (codepoint.is_valid_ascii())
            return {.codepoint=codepoint, .state=CodecState::Valid};

        return {.codepoint=codepoint::replacement_unicode, .state=CodecState::Invalid};
    }



} // namespace cl7::strings
