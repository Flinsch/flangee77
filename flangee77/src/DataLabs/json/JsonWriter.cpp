#include "JsonWriter.h"

#include "./detail/Generator.h"



namespace dl7::json {



    /**
     * Generates a string representing the given JSON object/value based on the
     * specified format.
     */
    cl7::u8string JsonWriter::to_string(const Json& json, const Format& format)
    {
        syntax::Diagnostics diagnostics;

        detail::Generator generator{&diagnostics, format};

        return generator.process(json);
    }



} // namespace dl7::json
