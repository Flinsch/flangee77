#pragma once
#ifndef CL7_LOGGING_HTMLLOGHANDLER_H
#define CL7_LOGGING_HTMLLOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7::logging {



class HtmlLogHandler
    : public AbstractLogHandler
{

public:

    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    HtmlLogHandler();

    HtmlLogHandler(const HtmlLogHandler&) = delete;
    HtmlLogHandler& operator=(const HtmlLogHandler&) = delete;
    HtmlLogHandler(HtmlLogHandler&&) = delete;
    HtmlLogHandler& operator=(HtmlLogHandler&&) = delete;

    ~HtmlLogHandler() override;



private:

    struct Block
    {
        std::string block_class;

        Block(std::string_view block_class);
        ~Block();
    };

    using BlockPtr = std::unique_ptr<Block>;



    // #############################################################################
    // Implementations
    // #############################################################################

    /**
     * Writes the specified log entry.
     */
    void _write(const LogEntry& log_entry) override;



    // #############################################################################
    // Helpers
    // #############################################################################

    /**
     * Escapes the specified "raw" text and returns it as UTF-8 encoded std::string.
     */
    static std::string _escape(cl7::string_view text);

    /**
     * Escapes the specified "raw" ASCII/Latin-1 text and returns it as UTF-8
     * encoded std::string.
     */
    static std::string _escape(cl7::astring_view as);

    /**
     * Escapes the specified "raw" UTF-8 text and returns it as UTF-8 encoded
     * std::string.
     */
    static std::string _escape(cl7::u8string_view u8s);

    /**
     * Writes the specified "raw" ASCII/Latin-1 or UTF-8 encoded std::string to the
     * log file without escaping it. If specified, the file is initially truncated.
     */
    static void _write_raw(std::string_view raw, bool truncate = false);

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    static void _write_source_location(const cl7::achar_type* file_path, unsigned line_number, const cl7::achar_type* function_name);



    // #############################################################################
    // Attributes
    // #############################################################################

    BlockPtr _block_ptr;

}; // class HtmlLogHandler



} // namespace cl7::logging

#endif // CL7_LOGGING_HTMLLOGHANDLER_H
