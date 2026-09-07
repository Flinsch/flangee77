#include "YamlWriter.h"

#include "./detail/Generator.h"



namespace dl7::yaml {



    YamlWriter::YamlWriter(const Format& format)
        : _format(format)
    {
    }



    /**
     * Generates a string representing the given YAML node/document based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string YamlWriter::to_string(const Yaml& yaml)
    {
        _diagnostics.clear();

        detail::Generator generator{&_diagnostics, _format};

        return generator.process(yaml);
    }



} // namespace dl7::yaml
