#include "TomlWriter.h"

#include "./detail/Generator.h"



namespace dl7::toml {



    /**
     * Generates a string representing the given `Toml` object based on the specified
     * format.
     */
    cl7::u8string TomlWriter::to_string(const Toml& toml, const Format& format)
    {
        syntax::Diagnostics diagnostics;

        detail::Generator generator{&diagnostics, format};

        return generator.process(toml);
    }



} // namespace dl7::toml
