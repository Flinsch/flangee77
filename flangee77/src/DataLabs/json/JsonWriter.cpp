#include "JsonWriter.h"

#include "./detail/Generator.h"



namespace dl7::json {



    JsonWriter::JsonWriter(const Format& format)
        : _format(format)
    {
    }



    /**
     * Generates a string representing the given JSON object/value based on the
     * format this writer was given. Whatever there is to complain about ends
     * up in the diagnostics, which are cleared beforehand.
     */
    cl7::u8string JsonWriter::to_string(const Json& json)
    {
        _diagnostics.clear();

        detail::Generator generator{&_diagnostics, _format};

        return generator.process(json);
    }



} // namespace dl7::json
