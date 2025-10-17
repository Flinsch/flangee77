#ifndef DL7_XML_DETAIL_BUILDER_H
#define DL7_XML_DETAIL_BUILDER_H
#include <DataLabs/syntax/DirectAstBuilder.h>

#include "../Document.h"



namespace dl7::xml::detail {



class Builder
    : public syntax::DirectAstBuilder<Document>
{

public:
    explicit Builder(syntax::Diagnostics* diagnostics);



    std::unique_ptr<Document> build(syntax::TokenReader& token_reader) override;



private:


}; // class Builder



} // namespace dl7::xml::detail

#endif // DL7_XML_DETAIL_BUILDER_H
