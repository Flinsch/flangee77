#ifndef DL7_XML_DETAIL_GENERATOR_H
#define DL7_XML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Document.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::xml::detail {



class Generator
    : public syntax::AstProcessor<Document, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Document& root) override;



private:


    Format _format;

}; // class Generator



} // namespace dl7::xml::detail

#endif // DL7_XML_DETAIL_GENERATOR_H
