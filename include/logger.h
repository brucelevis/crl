/*
 * inputsystem.h
 *
 *  Created on: Dec 15, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#ifndef INCLUDE_LOGGER_H_
#define INCLUDE_LOGGER_H_

#include <memory>
#include <string>

//! Logger class used for logging stuff
/*!
 * This logger class is used to log information
 * It is a singleton since only one logger should exist
 */
class Logger
{
public:
	//! Loglevel enum
    enum class Level 
    {
        LCRITICAL   = 0, /*< Critical message */
        LINFO       = 1, /*< Info message */
        LERROR      = 2, /*< Error message */
        LWARNING    = 3, /*< Warning message */
        LDEBUG      = 4  /*< Debug message */
    };

    ~Logger();

    //! Gets the instance of the logger
    /*!
     * \return A std::shared_ptr to the logging instance
     */
    static std::shared_ptr<Logger> Instance();

    //! Logs one line of text (defaults to loglevel info)
    /*!
     * \param str Line to log
     */
    void logLine(const std::string& str);

    //! Logs a string with the specified level
    /*!
     * \param level The loglevel of the string
     * \param str The string to log
     */
    void logLine(Level level, const std::string& str);

    //! Sets the loglevel
    /*!
     * Ensures only relevant information is logged
     * Everything below the loglevel is logged
     * Everything above is not
     * Example: If loglevel == Level::LERROR warnings and debug info won't be logged
     *
     * \param level The loglevel to set
     */
    void setLogLevel(Level level);
private:
    static std::shared_ptr<Logger> logger; //! Logger instance
    Level logLevel; //! Current loglevel

    Logger();
};

#endif /* INCLUDE_LOGGER_H_ */
