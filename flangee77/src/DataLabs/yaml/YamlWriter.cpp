#include "YamlWriter.h"

#include "./detail/Generator.h"



namespace dl7::yaml {



    /**
     * Generates a string representing the given `Yaml` object based on the specified
     * format.
     */
    cl7::u8string YamlWriter::to_string(const Yaml& yaml, const Format& format)
    {
        syntax::Diagnostics diagnostics;

        detail::Generator generator{&diagnostics, format};

        return generator.process(yaml);
    }



} // namespace dl7::yaml
