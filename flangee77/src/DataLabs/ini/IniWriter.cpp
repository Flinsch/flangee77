#include "IniWriter.h"

#include "./detail/Generator.h"



namespace dl7::ini {



    /**
     * Generates a string representing the given `Ini` object based on the specified
     * format.
     */
    cl7::u8string IniWriter::to_string(const Ini& ini, const Format& format)
    {
        syntax::Diagnostics diagnostics;

        detail::Generator generator{&diagnostics, format};

        return generator.process(ini);
    }



} // namespace dl7::ini
