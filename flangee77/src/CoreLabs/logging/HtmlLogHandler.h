#pragma once
#ifndef CL7_LOGGING_HTMLLOGHANDLER_H
#define CL7_LOGGING_HTMLLOGHANDLER_H
#include "./AbstractLogHandler.h"



namespace cl7 {
namespace logging {



class HtmlLogHandler
    : public AbstractLogHandler
{

private:
    struct Block
    {
        Block(HtmlLogHandler* owner, std::string_view block_class);
        ~Block();

        HtmlLogHandler* const _owner;
        const std::string _block_class;
    };

    typedef std::unique_ptr<Block> BlockPtr;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Default constructor.
     */
    HtmlLogHandler();

    /**
     * Destructor.
     */
    virtual ~HtmlLogHandler() override;

private:
    /** Copy constructor. */
    HtmlLogHandler(const HtmlLogHandler&) = delete;
    /** Copy assignment operator. */
    HtmlLogHandler& operator = (const HtmlLogHandler&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    BlockPtr _block_ptr;



    // #############################################################################
    // Implementations
    // #############################################################################
private:
    /**
     * Writes the specified log entry.
     */
    virtual void _write(const LogEntry& log_entry) override;



    // #############################################################################
    // Helpers
    // #############################################################################
private:
    /**
     * Escapes the specified "raw" text and returns it as UTF-8 encoded std::string.
     */
    std::string _escape(cl7::string_view text) const;

    /**
     * Escapes the specified "raw" ASCII/Latin-1 text and returns it as UTF-8
     * encoded std::string.
     */
    std::string _escape(cl7::astring_view as) const;

    /**
     * Escapes the specified "raw" UTF-8 text and returns it as UTF-8 encoded
     * std::string.
     */
    std::string _escape(cl7::u8string_view u8s) const;

    /**
     * Writes the specified "raw" ASCII/Latin-1 or UTF-8 encoded std::string to the
     * log file without escaping it. If specified, the file is initially truncated.
     */
    void _write_raw(std::string_view raw, bool truncate = false);

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    void _write_source_location(const cl7::achar_type* file_path, unsigned line_number, const cl7::achar_type* function_name);

}; // class HtmlLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_HTMLLOGHANDLER_H
