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

    ~HtmlLogHandler() noexcept override;



private:

    struct Block // NOLINT(cppcoreguidelines-special-member-functions)
    {
        cl7::u8string block_class;

        Block(cl7::u8string_view block_class);
        ~Block() noexcept;
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
     * Escapes the specified "raw" std::string and returns it as UTF-8 encoded text.
     */
    static cl7::u8string _escape(std::string_view s);

    /**
     * Escapes the specified "raw" UTF-8 encoded text.
     */
    static cl7::u8string _escape(cl7::u8string_view u8s);

    /**
     * Writes the specified "raw" UTF-8 encoded text to the log file without
     * escaping it. If specified, the file is initially truncated.
     */
    static void _write_raw(cl7::u8string_view raw, bool truncate = false);

    /**
     * Writes certain information about the source code location from which the log
     * entry came to the log file.
     */
    static void _write_source_location(const char* file_path, unsigned line_number, const char* function_name);



    // #############################################################################
    // Attributes
    // #############################################################################

    BlockPtr _block_ptr;

}; // class HtmlLogHandler



} // namespace cl7::logging

#endif // CL7_LOGGING_HTMLLOGHANDLER_H
