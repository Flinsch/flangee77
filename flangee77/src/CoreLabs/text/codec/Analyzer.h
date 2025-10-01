#ifndef CL7_TEXT_CODEC_ANALYZER_H
#define CL7_TEXT_CODEC_ANALYZER_H

#include "./EncodeResult.h"
#include "./DecodeResult.h"
#include "./DefaultErrorHandler.h"

#include "./traits.h"



namespace cl7::text::codec {



template <typename Tchar, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Analyzer
{

public:
    using Codec = typename traits<Tchar>::codec_type;

    using string_view_type = typename traits<Tchar>::string_view_type;



    Analyzer(const ErrorHandler* error_handler = nullptr)
        : _default_error_handler()
        , _error_handler(error_handler ? error_handler : &_default_error_handler)
    {
    }



    /**
     * Returns the number of code points that a given input represents.
     */
    size_t determine_length(string_view_type input) const
    {
        if constexpr (!Codec::variable_length_encoding)
        {
            assert(Codec::determine_encode_length({codepoint::max_unicode}) == 1);
            return input.size();
        }

        size_t length = 0;

        size_t input_offset = 0;
        while (input_offset < input.size())
        {
            input_offset += Codec::determine_decode_length(input.substr(input_offset));
            ++length;
        }
        assert(input_offset == input.size());

        return length;
    }

    /**
     * Returns the number of code units that a given input would have in the target
     * encoding.
     */
    template <typename Tchar_target>
    size_t determine_target_length(string_view_type input) const
    {
        using TargetCodec = typename traits<Tchar_target>::codec_type;

        if constexpr (!Codec::variable_length_encoding && !TargetCodec::variable_length_encoding)
        {
            assert(Codec::determine_encode_length({codepoint::max_unicode}) == 1);
            assert(TargetCodec::determine_encode_length({codepoint::max_unicode}) == 1);
            return input.size();
        }

        codepoint replacement;
        if constexpr (std::is_same_v<TargetCodec, AsciiCodec>)
            replacement = _error_handler->replacement_ascii;
        else
            replacement = _error_handler->replacement_unicode;

        ErrorStatus error_status;

        size_t target_length = 0;

        size_t input_offset = 0;
        while (input_offset < input.size())
        {
            const auto decode_result = Codec::decode_one(error_status, input.substr(input_offset), {});
            target_length += TargetCodec::determine_encode_length(decode_result.codepoint, replacement);
            input_offset += decode_result.input_read.size();
        }
        assert(input_offset == input.size());

        return target_length;
    }



    /**
     * Performs validation and returns a boolean indicating whether the input
     * conforms to the expected encoding format.
     */
    bool validate(string_view_type input) const
    {
        return validate_with_status(input).error_count == 0;
    }

    /**
     * Performs validation and returns a status object indicating whether the input
     * conforms to the expected encoding format.
     */
    ErrorStatus validate_with_status(string_view_type input) const
    {
        ErrorStatus error_status;

        size_t input_offset = 0;
        while (input_offset < input.size())
        {
            const auto decode_result = Codec::decode_one(error_status, input.substr(input_offset), *_error_handler);
            input_offset += decode_result.input_read.size();
        }
        assert(input_offset == input.size());

        return error_status;
    }



private:
    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Analyzer



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_ANALYZER_H
