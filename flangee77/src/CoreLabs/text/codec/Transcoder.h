#ifndef CL7_TEXT_CODEC_TRANSCODER_H
#define CL7_TEXT_CODEC_TRANSCODER_H

#include "./Analyzer.h"



namespace cl7::text::codec {



template <typename Tchar_from, typename Tchar_to, class TDefaultErrorHandler = DefaultErrorHandler>
    requires(std::is_default_constructible_v<TDefaultErrorHandler>)
class Transcoder
{

public:
    using InputCodec = typename traits<Tchar_from>::codec_type;
    using OutputCodec = typename traits<Tchar_to>::codec_type;

    using string_view_type = typename traits<Tchar_from>::string_view_type;
    using string_span_type = typename traits<Tchar_to>::string_span_type;
    using string_type = typename traits<Tchar_to>::string_type;



    Transcoder(const ErrorHandler* error_handler = nullptr)
        : _default_error_handler()
        , _error_handler(error_handler ? error_handler : &_default_error_handler)
    {
    }



    /**
     * Returns the number of code units that a given input would have in the target
     * encoding.
     */
    size_t determine_output_length(string_view_type input) const
    {
        return Analyzer<Tchar_from>(_error_handler).template determine_target_length<Tchar_to>(input);
    }



    /**
     * Converts a given input string from the source encoding to the target
     * encoding.
     */
    string_type transcode(string_view_type input) const
    {
        string_type output(determine_output_length(input), typename string_type::value_type());
        transcode_into(input, cl7::make_string_span(output));
        return output;
    }

    /**
     * Converts a given input string from the source encoding to the target encoding
     * and stores the result in the provided output string.
     */
    void transcode_into(string_view_type input, string_type& output) const
    {
        output.resize(determine_output_length(input));
        transcode_into(input, cl7::make_string_span(output));
    }

    /**
     * Converts a given input string from the source encoding to the target encoding
     * and stores the result in the provided output buffer, if space allows. The
     * operation does not null-terminate the output.
     */
    void transcode_into(string_view_type input, string_span_type output) const
    {
        ErrorStatus error_status;
        _do_transcode(error_status, input, output);
    }

    /**
     * Converts a given input string from the source encoding to the target encoding
     * and stores the result in the provided output string, returning a status
     * indicating any errors encountered.
     */
    ErrorStatus transcode_with_status(string_view_type input, string_type& output) const
    {
        output.resize(determine_output_length(input));
        return transcode_with_status(input, cl7::make_string_span(output));
    }

    /**
     * Converts a given input string from the source encoding to the target encoding
     * and stores the result in the provided output buffer, if space allows,
     * returning a status indicating any errors encountered. The operation does not
     * null-terminate the output.
     */
    ErrorStatus transcode_with_status(string_view_type input, string_span_type output) const
    {
        ErrorStatus error_status;
        _do_transcode(error_status, input, output);
        return error_status;
    }



private:
    void _do_transcode(ErrorStatus& error_status, string_view_type input, string_span_type output) const
    {
        size_t output_offset = 0;

        size_t input_offset = 0;
        while (input_offset < input.size())
        {
            const auto decode_result = InputCodec::decode_one(error_status, input.substr(input_offset), *_error_handler);
            const auto encode_result = OutputCodec::encode_one(error_status, decode_result.codepoint, output.subspan(output_offset), *_error_handler);
            input_offset += decode_result.input_read.size();
            output_offset += encode_result.output_written.size();
            // Check output offset separately and not in the while condition,
            // but explicitly after the (first) transcoding step to generate
            // any errors regarding a supposedly empty/exhausted buffer.
            if (output_offset >= output.size())
                break;
        }
        assert(input_offset == input.size());
    }



    TDefaultErrorHandler _default_error_handler;
    const ErrorHandler* _error_handler;

}; // class Transcoder



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_TRANSCODER_H
