
#ifndef DROGON_EVENTLOOP_H
#define DROGON_EVENTLOOP_H

#include <drogon/portable/Common.hpp>
#include <drogon/portable/Utils.hpp>

namespace drogon
{

class EventLoopImpl;
class EventLoopThreadImpl;
class EventLoopThreadPoolImpl;

class EventLoop
{
  public:
    using TimerId = std::shared_ptr<void>;
    using Func = std::function<void()>;

    static TimerId InvalidTimerId;

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

    void invalidateTimer(const TimerId &timerId);

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

    struct LocalThreadEventLoop
    {
        static thread_local EventLoop *LocalEventLoop;

        static EventLoop *get()
        {
            return LocalEventLoop;
        }

        static void set(EventLoop *loop)
        {
            LocalEventLoop = loop;
        }
    };

    static EventLoop *getEventLoopOfCurrentThread()
    {
        return EventLoop::LocalThreadEventLoop::get();
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
