//
// Created by neo on 5/3/24.
//

#ifndef DROGON_EVENTLOOP_H
#define DROGON_EVENTLOOP_H

#include <drogon/portable/pch.hpp>
#include <drogon/portable/Date.hpp>
#include <drogon/portable/Logger.hpp>

namespace drogon
{

class EventLoopImpl;
class EventLoopThreadImpl;
class EventLoopThreadPoolImpl;

class EventLoop
{
  public:
    using TimerId = uint64_t;
    using Func = std::function<void()>;

    static constexpr TimerId InvalidTimerId = 0;

    virtual ~EventLoop();

    EventLoop();

    void setIndex(size_t index);

    [[nodiscard]] std::size_t index() const;

    void resetTimerQueue();

    void resetAfterFork();

    bool isInLoopThread();

    void moveToCurrentThread();

    void loop();

    bool isRunning();

    void runAfter(std::chrono::duration<double> delay, const Func &cb);

    void runAfter(double delay, const Func &cb);

    void runAt(const Date &date, const Func &cb);

    void dispatch(const Func &cb);

    void post(const Func &cb);

    TimerId runEvery(double interval, const Func &cb);

    void invalidateTimer(TimerId timerId);

    void runOnQuit(const Func &cb);

    void quit();

    void assertInLoopThread();

    template <class Func>
    void queueInLoop(Func func)
    {
        post(func);
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

    static EventLoop **LocalThreadEventLoop()
    {
        static thread_local EventLoop *localEventLoop;
        return &localEventLoop;
    }

    static EventLoop *getEventLoopOfCurrentThread()
    {
        return *LocalThreadEventLoop();
    }

    static void abortNotInLoopThread()
    {
        LOG_FATAL
            << "It is forbidden to run loop on threads other than event-loop "
               "thread";
        exit(1);
    }

    [[nodiscard]] EventLoopImpl *getImpl() const
    {
        return impl_;
    }

  protected:
    EventLoopImpl *impl_;
};

class EventLoopThread
{
  public:
    virtual ~EventLoopThread();

    explicit EventLoopThread(const std::string &threadName = "EventLoopThread");

    EventLoop *getLoop();

    void start();

    void stop();

    void wait();

    void run();

    [[nodiscard]] size_t getIndex() const;

  protected:
    EventLoopThreadImpl *impl_;
};

class EventLoopThreadPool
{
  public:
    virtual ~EventLoopThreadPool();

    explicit EventLoopThreadPool(
        size_t threadNum,
        const std::string &name = "EventLoopThreadPool");

    size_t size();

    EventLoop *getLoop(size_t id);

    EventLoop *getNextLoop();

    std::vector<EventLoop *> getLoops();

    void start();

    void stop();

    void wait();

  protected:
    EventLoopThreadPoolImpl *impl_;
};

}  // namespace drogon

#endif  // DROGON_EVENTLOOP_H
