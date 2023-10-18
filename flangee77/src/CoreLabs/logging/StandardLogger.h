#pragma once
#ifndef CL7_LOGGING_STANDARDLOGGER_H
#define CL7_LOGGING_STANDARDLOGGER_H
#include <CoreLabs/creational/Singleton.h>

#include "./Logger.h"



namespace cl7 {
namespace logging {



class StandardLogger final
    : public creational::Singleton<StandardLogger>
{
    friend creational::Singleton<StandardLogger>;



    // #############################################################################
    // Construction / Destruction
    // #############################################################################
private:
    /**
     * Default constructor.
     */
    StandardLogger();

    /**
     * Destructor.
     */
    ~StandardLogger() = default;

private:
    /** Copy constructor. */
    StandardLogger(const StandardLogger&) = delete;
    /** Copy assignment operator. */
    StandardLogger& operator = (const StandardLogger&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The actual logger.
     */
    Logger _logger;



    // #############################################################################
    // Dereferenciation
    // #############################################################################
public:
    /**
     * Returns the actual logger.
     */
    static Logger& instance() { return creational::Singleton<StandardLogger>::instance()._logger; }

    /**
     * Returns the actual logger.
     */
    Logger* operator -> () { return &_logger; }

}; // class StandardLogger final



} // namespace logging
} // namespace cl7

#endif // CL7_LOGGING_STANDARDLOGGER_H
