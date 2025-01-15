#ifndef DL7_JSON_JSONWRITER_H
#define DL7_JSON_JSONWRITER_H

#include "./Json.h"



namespace dl7::json {



class JsonWriter
{

public:
    struct Format
    {
        enum class Style : unsigned
        {
            SingleLine = 0,
            MultiLine = 1,
        };

        struct SingleLineOptions
        {
            bool compact = false;
        };

        struct MultiLineOptions
        {
            enum class Indentation : unsigned
            {
                Tabs = 0,
                Spaces2 = 2,
                Spaces4 = 4,
            };

            enum class LineEnding
            {
                CRLF,   // CRLF (`\r\n`): Windows, DOS
                LF,     // LF (`\n`): Unix, Linux, macOS, and "modern" line-ending styles in general
                CR,     // CR (`\r`): Legacy Mac
            };

            Indentation indentation = Indentation::Spaces4;
            LineEnding line_ending = LineEnding::LF;
            bool add_trailing_commas = false;
            bool add_empty_line = false;
        };

        Style style = Style::MultiLine;
        SingleLineOptions single_line_options;
        MultiLineOptions multi_line_options;
        bool escape_unicode = false;
        bool allow_single_quotes = false;
    }; // struct Format

    static constexpr Format DEFAULT_MULTI_LINE_FORMAT = {Format::Style::MultiLine, {}, {.indentation=Format::MultiLineOptions::Indentation::Spaces4, .line_ending=Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas=false, .add_empty_line=false}, false, false};
    static constexpr Format DEFAULT_PRETTY_PRINT_FORMAT = {Format::Style::MultiLine, {}, {.indentation=Format::MultiLineOptions::Indentation::Spaces4, .line_ending=Format::MultiLineOptions::LineEnding::LF, .add_trailing_commas=true, .add_empty_line=true}, false, false};
    static constexpr Format DEFAULT_SINGLE_LINE_FORMAT = {Format::Style::SingleLine, {.compact=false}, {}, false, false};
    static constexpr Format DEFAULT_COMPACT_FORMAT = {Format::Style::SingleLine, {.compact=true}, {}, false, false};
    static constexpr Format DEFAULT_FORMAT = DEFAULT_PRETTY_PRINT_FORMAT;



    static cl7::u8string to_string(const Json& json, const Format& format = DEFAULT_FORMAT);

}; // class JsonWriter



} // namespace dl7::json

#endif // DL7_JSON_JSONWRITER_H
