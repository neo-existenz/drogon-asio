//
// Created by neo on 5/6/24.
//

#ifndef DROGON_DATE_HPP
#define DROGON_DATE_HPP

#include <drogon/portable/Common.hpp>

#define MICRO_SECONDS_PRE_SEC 1000000
namespace drogon
{

class Date
{
  public:
    using Clock = std::chrono::system_clock;
    using Duration = std::chrono::microseconds;
    using TimePoint = std::chrono::time_point<Clock, Duration>;

    Date()
    {
        timePoint_ = std::chrono::time_point_cast<Duration>(Clock::now());
    }

    explicit Date(int64_t timestamp)
    {
        timePoint_ = std::chrono::time_point_cast<Duration>(
            Clock::from_time_t(timestamp));
    }

    explicit Date(const TimePoint &timePoint)
    {
        timePoint_ = timePoint;
    }

    static Date now()
    {
        return Date(std::chrono::time_point_cast<Duration>(Clock::now()));
    }

    static Date date()
    {
        return now();
    }

    [[nodiscard]] Date after(double second) const
    {
        auto time = timePoint_ + std::chrono::duration_cast<Duration>(
                                     std::chrono::duration<double>(second));
        return Date(time);
    }

    [[nodiscard]] int64_t microSecondsSinceEpoch() const
    {
        return timePoint_.time_since_epoch().count();
    }

    bool operator<(const Date &date) const
    {
        return timePoint_ < date.timePoint_;
    }

    bool operator>(const Date &date) const
    {
        return timePoint_ > date.timePoint_;
    }

    [[nodiscard]] std::string toDbStringLocal() const
    {
        char buf[32];
        auto tm = toTmStruct();
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        return buf;
    }

    [[nodiscard]] std::string toDbString() const
    {
        char buf[32];
        auto tm = toTmStruct();
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);
        return buf;
    }

    [[nodiscard]] struct tm toTmStruct() const
    {
        struct tm tm
        {
        };
        time_t time = std::chrono::system_clock::to_time_t(timePoint_);
        localtime_r(&time, &tm);
        return tm;
    }

    [[nodiscard]] std::string toFormattedString(bool showMicroseconds) const {
        char buf[64]; // Buffer to hold the formatted string
        auto tm = toTmStruct(); // Convert to tm struct for formatting

        // Format the date and time without microseconds
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", &tm);

        std::string formattedString(buf);

        // If microseconds should be shown, append them
        if (showMicroseconds) {
            int64_t microseconds = microSecondsSinceEpoch() % MICRO_SECONDS_PRE_SEC;
            char microBuf[32];
            snprintf(microBuf, sizeof(microBuf), ".%06ld", microseconds);
            formattedString += microBuf;
        }

        return formattedString;
    }

    void toCustomedFormattedString(const std::string &fmt,
                                                 char *buf,
                                                 std::size_t size) const
    {
        auto tm = toTmStruct();
        strftime(buf, size, fmt.data(), &tm);
    }

    [[nodiscard]] TimePoint timePoint() const
    {
        return timePoint_;
    }
  private:
    TimePoint timePoint_;
};

}  // namespace drogon

#endif  // DROGON_DATE_HPP
