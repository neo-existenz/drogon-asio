//
// Created by neo on 5/9/24.
//

#ifndef DROGON_LOGGER_HPP
#define DROGON_LOGGER_HPP

#include <drogon/portable/Common.hpp>

#include <boost/log/trivial.hpp>

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

#define LOG_WARN BOOST_LOG_TRIVIAL(warning)
#define LOG_DEBUG BOOST_LOG_TRIVIAL(debug)
#define LOG_INFO BOOST_LOG_TRIVIAL(info)
#define LOG_ERROR BOOST_LOG_TRIVIAL(error)
#define LOG_FATAL BOOST_LOG_TRIVIAL(fatal)
#define LOG_SYSERR BOOST_LOG_TRIVIAL(trace)
#define LOG_TRACE BOOST_LOG_TRIVIAL(trace)

#endif  // DROGON_LOGGER_HPP
