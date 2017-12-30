/*
 * inputsystem.h
 *
 *  Created on: Dec 30, 2017
 *      Author: Joshua Lettink
 *
 *  Copyright (C) 2017 - Joshua Lettink
 */

#include <cstdarg>
#include <cstdio>
#include <sstream>

#include "logger.h"

/* static */ std::shared_ptr<Logger> Logger::logger;

Logger::Logger() :
    #ifndef NDEBUG
    logLevel(Level::LDEBUG)
    #else
    logLevel(Level::LERROR)
    #endif
{

}

Logger::~Logger()
{

}

void Logger::setLogLevel(Level level)
{
    logLevel = level;
}

/* static */ std::shared_ptr<Logger> Logger::Instance()
{
    if(logger == nullptr) 
    {
        logger = std::shared_ptr<Logger>(new Logger());
    }

    return logger;
}

void Logger::logLine(const std::string& str)
{
    logLine(Level::LINFO, str);
}

void Logger::logLine(Level level, const std::string& str)
{
    // if level > loglevel do not log
    if(static_cast<uint64_t>(level) > static_cast<uint64_t>(logLevel))
    {
        return;
    }

    std::stringstream sstream;
    std::string prefix = "";

    switch(level)
    {
        case Level::LINFO:
            prefix = "[INFO]";
            break;
        case Level::LERROR:
            prefix = "[ERROR]";
            break;
        case Level::LCRITICAL:
            prefix = "[CRITICAL]";
            break;
        case Level::LWARNING:
            prefix = "[WARNING]";
            break;
        case Level::LDEBUG:
            prefix = "[DEBUG]";
            break;
    }

    sstream << prefix << ": " << str << "\r" << std::endl;

    printf(sstream.str().c_str());
}
