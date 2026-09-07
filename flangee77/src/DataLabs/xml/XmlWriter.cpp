#include "XmlWriter.h"

#include "./detail/Generator.h"



namespace dl7::xml {



    XmlWriter::XmlWriter(const Format& format)
        : _format(format)
    {
    }



    /**
     * Generates a string representing the given XML document based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string XmlWriter::to_string(const Document& document)
    {
        _diagnostics.clear();

        detail::Generator generator{&_diagnostics, _format};

        return generator.process(document);
    }



} // namespace dl7::xml
