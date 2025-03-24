#ifndef CL7_STRINGS_LOGGINGERRORHANDLER_H
#define CL7_STRINGS_LOGGINGERRORHANDLER_H
#include "./ErrorHandler.h"

#include <CoreLabs/logging/LogContext.h>



namespace cl7::strings {



class LoggingErrorHandler
    : public ErrorHandler
{
public:
    ~LoggingErrorHandler() override = default;



    LoggingErrorHandler(cl7::logging::LogContext log_context = {});



    cl7::logging::LogContext get_log_context() const { return _log_context; }



protected:

    CodepointResult _check_adjust_ascii(ErrorStatus& error_status, codepoint codepoint) const override;
    CodepointResult _check_adjust_unicode(ErrorStatus& error_status, codepoint codepoint) const override;

    CodepointResult _on_exhausted_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result) const override;
    CodepointResult _on_insufficient_output_space(ErrorStatus& error_status, Encoding encoding, CodepointResult codepoint_result, size_t required, size_t available) const override;

    DecodeResult<cl7::achar_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::astring_view input_read) const override;
    DecodeResult<cl7::u8char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u8string_view input_read) const override;
    DecodeResult<cl7::u16char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u16string_view input_read) const override;
    DecodeResult<cl7::u32char_t> _on_invalid_code_unit(ErrorStatus& error_status, cl7::u32string_view input_read) const override;

    DecodeResult<cl7::u8char_t> _on_invalid_sequence(ErrorStatus& error_status, cl7::u8string_view input_read) const override;

    DecodeResult<cl7::u8char_t> _on_incomplete_sequence(ErrorStatus& error_status, cl7::u8string_view input_read, size_t expected_length) const override;
    DecodeResult<cl7::u16char_t> _on_incomplete_sequence(ErrorStatus& error_status, cl7::u16string_view input_read) const override;

    DecodeResult<cl7::u8char_t> _on_overlong_encoding(ErrorStatus& error_status, cl7::u8string_view input_read) const override;

    DecodeResult<cl7::u16char_t> _on_unpaired_surrogate(ErrorStatus& error_status, cl7::u16string_view input_read) const override;



private:

    void _try_log_warning(cl7::u8string_view message) const;



    cl7::logging::LogContext _log_context;

}; // class LoggingErrorHandler



} // namespace cl7::strings

#endif // CL7_STRINGS_LOGGINGERRORHANDLER_H
