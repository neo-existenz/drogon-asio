//
// Created by neo on 5/9/24.
//

#ifndef DROGON_LOGGER_HPP
#define DROGON_LOGGER_HPP

#include <drogon/portable/pch.hpp>

namespace drogon
{
class Logger
{
  public:
    enum Level
    {
        kTrace = 0,
        kDebug,
        kInfo,
        kWarn,
    };
    static void setLogLevel(int level)
    {
        (void)level;
    }
    static void setDisplayLocalTime(bool on)
    {
        (void)on;
    }
};

}  // namespace drogon

#define LOG_WARN std::cout
#define LOG_DEBUG std::cout
#define LOG_INFO std::cout
#define LOG_ERROR std::cout
#define LOG_FATAL std::cout
#define LOG_SYSERR std::cout
#define LOG_TRACE std::cout

#endif  // DROGON_LOGGER_HPP
