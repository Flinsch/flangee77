#pragma once
#ifndef PL7_APPLICATION_H
#define PL7_APPLICATION_H

#include "./ArgumentBag.h"



namespace pl7 {



class Application
{

    // #############################################################################
    // Construction / Destruction
    // #############################################################################
public:
    /**
     * Explicit constructor.
     */
    Application(int argc, cl7::char_type* argv[]);

    /**
     * Destructor.
     */
    virtual ~Application(void);

private:
    /** Default constructor. */
    Application(void) = delete;
    /** Copy constructor. */
    Application(const Application&) = delete;
    /** Copy assignment operator. */
    Application& operator = (const Application&) = delete;



    // #############################################################################
    // Attributes
    // #############################################################################
private:
    /**
     * The argument bag holding a list of command-line arguments.
     */
    ArgumentBag _argument_bag;

    /**
     * The exit code of the application instance.
     */
    int _exit_code;



    // #############################################################################
    // Properties
    // #############################################################################
public:
    /**
     * Returns the argument bag holding a list of command-line arguments.
     */
    const ArgumentBag& get_argument_bag() const { return _argument_bag; }

    /**
     * Returns the exit code of the application instance.
     */
    int get_exit_code() const { return _exit_code; }



    // #############################################################################
    // Methods
    // #############################################################################
public:
    /**
     * Runs the application instance.
     * Initializes the framework components, starts the main message loop, and shuts
     * down the framework components.
     */
    bool run();

}; // class Application



} // namespace pl7

#endif // PL7_APPLICATION_H
