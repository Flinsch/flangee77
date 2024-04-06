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
        Block(HtmlLogHandler* owner, cl7::string_view block_class);
        ~Block();

        HtmlLogHandler* const _owner;
        const cl7::string _block_class;
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
     * Escapes the specified "raw" text.
     */
    cl7::string _escape(cl7::string_view text) const;

    /**
     * Writes the specified "raw" text to the log file without escaping it. If
     * specified, the file is initially truncated.
     */
    void _write_raw(cl7::string_view text, bool truncate = false);

    /**
     * Writes the specified "raw" UTF-8 text to the log file without escaping it. If
     * specified, the file is initially truncated.
     */
    void _write_raw(cl7::u8string_view utf8, bool truncate = false);

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    void _write_source_location(const cl7::char_type* file_path, unsigned line_number, const cl7::char_type* function_name);

}; // class HtmlLogHandler



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_HTMLLOGHANDLER_H
