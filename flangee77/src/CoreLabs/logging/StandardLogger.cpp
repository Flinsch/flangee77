#include "StandardLogger.h"

#include "./CoutLogHandler.h"



namespace cl7::logging {



    StandardLogger::StandardLogger()
    {
        _logger.add_log_handler(std::make_shared<CoutLogHandler>());
    }



} // namespace cl7::logging
