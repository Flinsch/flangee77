#include "StandardLogger.h"

#include "./CoutLogHandler.h"



namespace cl7 {
namespace logging {



    // #############################################################################
    // Construction / Destruction
    // #############################################################################

    /**
     * Default constructor.
     */
    StandardLogger::StandardLogger()
        : _logger()
    {
        _logger.add_log_handler( std::make_shared<CoutLogHandler>() );
    }



} // namespace logging
} // namespace cl7
