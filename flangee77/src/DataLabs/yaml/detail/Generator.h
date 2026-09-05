#ifndef DL7_YAML_DETAIL_GENERATOR_H
#define DL7_YAML_DETAIL_GENERATOR_H
#include <DataLabs/syntax/AstProcessor.h>

#include "../Yaml.h"
#include "../Format.h"

#include <CoreLabs/sstream.h>



namespace dl7::yaml::detail {



class Generator
    : public syntax::AstProcessor<Yaml, cl7::u8string>
{

public:
    explicit Generator(syntax::Diagnostics* diagnostics, const Format& format);



    cl7::u8string process(const Yaml& yaml) override;



private:

    Format _format;

}; // class Generator



} // namespace dl7::yaml::detail

#endif // DL7_YAML_DETAIL_GENERATOR_H
