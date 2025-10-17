#include "XmlWriter.h"

#include "./detail/Generator.h"



namespace dl7::xml {



    /**
     * Generates a string representing the given XML document based on the specified
     * format.
     */
    cl7::u8string XmlWriter::to_string(const Document& document, const Format& format)
    {
        syntax::Diagnostics diagnostics;

        detail::Generator generator{&diagnostics, format};

        return generator.process(document);
    }



} // namespace dl7::xml
