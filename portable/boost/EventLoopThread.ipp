//
// Created by neo on 5/26/24.
//

#ifndef DROGON_EVENTLOOPTHREAD_IPP
#define DROGON_EVENTLOOPTHREAD_IPP

#include <drogon/portable/portable.hpp>

namespace drogon
{

class EventLoopThreadImpl
{
  public:
    explicit EventLoopThreadImpl(std::string threadName = "EventLoopThread")
        : loopThreadName_(std::move(threadName))
    {
        eventLoop_ = std::make_shared<EventLoop>();
    }

    ~EventLoopThreadImpl()
    {
        stop();
    }

    EventLoop *getLoop()
    {
        return eventLoop_.get();
    }

    void start()
    {
        thread_ = std::thread([this] {
            eventLoop_->moveToCurrentThread();
            run();
        });
    }

    void run()
    {
        eventLoop_->loop();
    }

    void stop()
    {
        eventLoop_->quit();
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    void wait()
    {
        thread_.join();
    }

  private:
    std::shared_ptr<EventLoop> eventLoop_;
    std::string loopThreadName_;
    std::thread thread_;
    size_t index_{};
};

}  // namespace drogon

#endif  // DROGON_EVENTLOOPTHREAD_IPP
