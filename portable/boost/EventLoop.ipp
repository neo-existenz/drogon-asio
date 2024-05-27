//
// Created by neo on 5/26/24.
//

#ifndef DROGON_EVENTLOOP_IPP
#define DROGON_EVENTLOOP_IPP

#include <boost/asio.hpp>

namespace drogon
{

namespace asio = boost::asio;

class EventLoopImpl
{
  public:
    using TimerId = uint64_t;
    static constexpr TimerId InvalidTimerId = 0;

    static EventLoopImpl **LocalThreadEventLoop()
    {
        static thread_local EventLoopImpl *localEventLoop;
        return &localEventLoop;
    }

    EventLoopImpl() : workGuard_(asio::make_work_guard(ioContext_))
    {
        *LocalThreadEventLoop() = this;
    }

    asio::io_context &getIoContext()
    {
        return ioContext_;
    }

    static EventLoopImpl *getEventLoopOfCurrentThread()
    {
        return *LocalThreadEventLoop();
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
    }

    void resetAfterFork()
    {
    }

    bool isInLoopThread()
    {
        return ioContext_.get_executor().running_in_this_thread();
    }

    void moveToCurrentThread()
    {
        if (!ioContext_.stopped())
        {
            throw std::runtime_error("EventLoop cannot be moved when running");
        }
    }

    void loop()
    {
        ioContext_.run();
    }

    bool isRunning()
    {
        return !ioContext_.stopped();
    }

    template <class Func>
    void queueInLoop(Func func)
    {
        ioContext_.post(func);
    }

    template <typename Functor>
    inline void runInLoop(Functor &&f)
    {
        if (isInLoopThread())
        {
            f();
        }
        else
        {
            queueInLoop(std::forward<Functor>(f));
        }
    }

    void runAfter(std::chrono::duration<double> delay, const EventLoop::Func &cb)
    {
    }
    void runAfter(double delay, const EventLoop::Func &cb)
    {
        return runAt(Date::date().after(delay), cb);
    }

    void runAt(const Date &date, const EventLoop::Func &cb)
    {
    }

    void dispatch(const EventLoop::Func &cb)
    {
        ioContext_.dispatch(cb);
    }

    void post(const EventLoop::Func &cb) {
        ioContext_.post(cb);
    }

    TimerId runEvery(double interval, const EventLoop::Func &cb)
    {
        return 0;
    }

    void invalidateTimer(TimerId timerId)
    {
    }

    void runOnQuit(const EventLoop::Func &cb)
    {
    }

    void quit()
    {
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
    asio::io_context ioContext_;
    asio::executor_work_guard<asio::io_context::executor_type> workGuard_;
    size_t index_{};
};

}

#endif  // DROGON_EVENTLOOP_IPP
