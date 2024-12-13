#pragma once
#ifndef CL7_LOGGING_STANDARDLOGGER_H
#define CL7_LOGGING_STANDARDLOGGER_H
#include <CoreLabs/creational/Singleton.h>

#include "./Logger.h"



namespace cl7::logging {



class StandardLogger final
    : public creational::Singleton<StandardLogger>
{
    friend class creational::Singleton<StandardLogger>;

public:
    StandardLogger(const StandardLogger&) = delete;
    StandardLogger& operator = (const StandardLogger&) = delete;
    StandardLogger(StandardLogger&&) = delete;
    StandardLogger& operator = (StandardLogger&&) = delete;

    static Logger& instance() { return creational::Singleton<StandardLogger>::instance()._logger; }

    Logger* operator -> () { return &_logger; }



private:
    StandardLogger();
    ~StandardLogger() override = default;

    Logger _logger;

}; // class StandardLogger final



} // namespace cl7::logging

#endif // CL7_LOGGING_STANDARDLOGGER_H
