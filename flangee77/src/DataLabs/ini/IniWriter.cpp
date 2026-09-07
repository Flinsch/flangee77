#include "IniWriter.h"

#include "./detail/Generator.h"



namespace dl7::ini {



    IniWriter::IniWriter(const Format& format)
        : _format(format)
    {
    }



    /**
     * Generates a string representing the given `Ini` object based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string IniWriter::to_string(const Ini& ini)
    {
        _diagnostics.clear();

        detail::Generator generator{&_diagnostics, _format};

        return generator.process(ini);
    }



} // namespace dl7::ini
