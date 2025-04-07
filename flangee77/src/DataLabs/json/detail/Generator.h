#ifndef DL7_JSON_DETAIL_GENERATOR_H
#define DL7_JSON_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Json.h"
#include "../Format.h"



namespace dl7::json::detail {



class Generator
    : public syntax::AstProcessor<Json, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Json& root) override;



private:
    Format _format;

}; // class Generator



} // namespace dl7::json::detail

#endif // DL7_JSON_DETAIL_GENERATOR_H
