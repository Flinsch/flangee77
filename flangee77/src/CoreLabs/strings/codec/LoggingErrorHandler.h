#ifndef CL7_STRINGS_CODEC_LOGGINGERRORHANDLER_H
#define CL7_STRINGS_CODEC_LOGGINGERRORHANDLER_H
#include "./ErrorHandler.h"

#include <CoreLabs/logging/LogContext.h>



namespace cl7::strings::codec {



class LoggingErrorHandler
    : public ErrorHandler
{
public:
    ~LoggingErrorHandler() override = default;



    LoggingErrorHandler(cl7::logging::LogContext log_context = {});



    cl7::logging::LogContext get_log_context() const { return _log_context; }



protected:

    void _on_out_of_range_ascii(codepoint codepoint) const override;
    void _on_out_of_range_unicode(codepoint codepoint) const override;
    void _on_disallowed_unicode(codepoint codepoint) const override;

    void _on_exhausted_output_space(Encoding encoding, CodepointResult codepoint_result) const override;
    void _on_insufficient_output_space(Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const override;

    void _on_invalid_code_unit(cl7::astring_view input_read) const override;
    void _on_invalid_code_unit(cl7::u8string_view input_read) const override;
    void _on_invalid_code_unit(cl7::u16string_view input_read) const override;
    void _on_invalid_code_unit(cl7::u32string_view input_read) const override;

    void _on_invalid_sequence(cl7::u8string_view input_read) const override;

    void _on_incomplete_sequence(cl7::u8string_view input_read, size_t expected_length) const override;
    void _on_incomplete_sequence(cl7::u16string_view input_read) const override;

    void _on_overlong_encoding(cl7::u8string_view input_read) const override;

    void _on_unpaired_surrogate(cl7::u16string_view input_read) const override;



private:

    void _try_log_warning(cl7::u8string_view message) const;



    cl7::logging::LogContext _log_context;

}; // class LoggingErrorHandler



} // namespace cl7::strings::codec

#endif // CL7_STRINGS_CODEC_LOGGINGERRORHANDLER_H
