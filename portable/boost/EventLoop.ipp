//
// Created by neo on 5/26/24.
//

#ifndef DROGON_EVENTLOOP_IPP
#define DROGON_EVENTLOOP_IPP

#include <boost/asio.hpp>
#include <utility>

namespace drogon
{

namespace asio = boost::asio;

class EventLoopImpl
{
  public:
    explicit EventLoopImpl() : workGuard_(asio::make_work_guard(ioContext_))
    {
        ioContext_.stop();
    }

    asio::io_context &getIoContext()
    {
        return ioContext_;
    }

    void setIndex(size_t index)
    {
        index_ = index;
    }

    [[nodiscard]] std::size_t index() const
    {
        return index_;
    }

    void resetTimerQueue()
    {
        ioContext_.reset();
    }

    void resetAfterFork()
    {
        ioContext_.notify_fork(asio::io_context::fork_child);
        ioContext_.reset();
    }

    bool isInLoopThread()
    {
        return ioContext_.get_executor().running_in_this_thread();
    }

    void loop()
    {
        if (ioContext_.stopped()) {
            ioContext_.restart();
        }
        ioContext_.run();
        // TODO: run quit function
    }

    bool isRunning()
    {
        return !ioContext_.stopped();
    }

    void dispatch(const EventLoop::Func &cb)
    {
        ioContext_.dispatch(cb);
    }

    void post(const EventLoop::Func &cb)
    {
        ioContext_.post(cb);
    }

    void quit()
    {
        ioContext_.stop();
    }

    void runAfter(std::chrono::duration<double> delay,
                  const EventLoop::Func &cb)
    {
        auto timer = std::make_shared<asio::steady_timer>(ioContext_);
        timer->expires_after(
            std::chrono::duration_cast<std::chrono::milliseconds>(delay));
        timer->async_wait([&, timer](const std::error_code &ec) {
            if (!ec)
            {
                cb();
            }
        });
    }

    void runAfter(double delay, const EventLoop::Func &cb)
    {
        auto timer = std::make_shared<asio::steady_timer>(ioContext_);
        timer->expires_after(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::duration<double>(delay)));
        timer->async_wait([&](const std::error_code &ec) {
            if (!ec)
            {
                cb();
            }
        });
    }

    void runAt(const Date &date, const EventLoop::Func &cb)
    {
        auto timer = std::make_shared<asio::system_timer>(ioContext_);
        timer->expires_at(date.timePoint());
        timer->async_wait([&](const std::error_code &ec) {
            if (!ec)
            {
                cb();
            }
        });
    }

    EventLoop::TimerId runEvery(double interval, const EventLoop::Func &cb)
    {
        auto timer = std::make_shared<asio::steady_timer>(ioContext_);
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::duration<double>(interval));
        run(timer, duration, cb);
        return timer;
    }

    void invalidateTimer(const std::shared_ptr<void> &timerId)
    {
        std::static_pointer_cast<asio::steady_timer>(timerId)->cancel();
    }

    void runOnQuit(const EventLoop::Func &cb)
    {
        // TODO: add quit function
    }

    static void abortNotInLoopThread()
    {
        LOG_FATAL
            << "It is forbidden to run loop on threads other than event-loop "
               "thread";
        exit(1);
    }

    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    };

  private:
    void run(std::shared_ptr<asio::steady_timer> timer,
             std::chrono::milliseconds duration,
             const EventLoop::Func &cb)
    {
        timer->expires_after(duration);
        timer->async_wait([timer, duration, callback = cb, this](const std::error_code &ec) {
            if (!ec)
            {
                callback();
                run(timer, duration, callback);
            }
        });
    }

  private:
    asio::io_context ioContext_;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard_;
    size_t index_{};
};

}  // namespace drogon

#endif  // DROGON_EVENTLOOP_IPP
