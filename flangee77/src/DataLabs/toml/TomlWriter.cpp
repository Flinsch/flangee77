#include "TomlWriter.h"

#include "./detail/Generator.h"



namespace dl7::toml {



    TomlWriter::TomlWriter(const Format& format)
        : _format(format)
    {
    }



    /**
     * Generates a string representing the given `Toml` object based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string TomlWriter::to_string(const Toml& toml)
    {
        _diagnostics.clear();

        detail::Generator generator{&_diagnostics, _format};

        return generator.process(toml);
    }



} // namespace dl7::toml
