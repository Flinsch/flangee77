#ifndef DL7_TOML_DETAIL_GENERATOR_H
#define DL7_TOML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Toml.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::toml::detail {



class Generator
    : public syntax::AstProcessor<Toml, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Toml& toml) override;



private:

    Format _format;

}; // class Generator



} // namespace dl7::toml::detail

#endif // DL7_TOML_DETAIL_GENERATOR_H
