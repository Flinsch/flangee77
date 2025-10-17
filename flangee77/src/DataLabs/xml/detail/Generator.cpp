#include "Generator.h"



namespace dl7::xml::detail {



    Generator::Generator(syntax::Diagnostics* diagnostics, const Format& format)
        : AstProcessor(diagnostics)
        , _format(format)
    {
    }



    cl7::u8string Generator::process(const Document& root)
    {
        cl7::u8osstream oss;



        return std::move(oss).str();
    }



} // namespace dl7::xml::detail
