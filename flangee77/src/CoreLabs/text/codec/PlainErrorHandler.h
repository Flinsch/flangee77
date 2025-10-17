#ifndef CL7_TEXT_CODEC_PLAINERRORHANDLER_H
#define CL7_TEXT_CODEC_PLAINERRORHANDLER_H
#include "./ErrorHandler.h"



namespace cl7::text::codec {



class PlainErrorHandler
    : public ErrorHandler
{
public:
    ~PlainErrorHandler() override = default;

}; // class PlainErrorHandler



} // namespace cl7::text::codec

#endif // CL7_TEXT_CODEC_PLAINERRORHANDLER_H
