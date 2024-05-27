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
        : eventLoop_(nullptr), loopThreadName_(std::move(threadName))
    {
    }

    EventLoop *getLoop()
    {
        return eventLoop_.get();
    }

    void start()
    {
        thread_ = std::thread(
            [this] { eventLoop_->getImpl()->getIoContext().run(); });
    }

    void run()
    {
        eventLoop_->loop();
    }

    void stop()
    {
        eventLoop_->getImpl()->getIoContext().stop();
        if (thread_.joinable())
        {
            thread_.join();
        }
    }

    void wait()
    {
    }

    [[nodiscard]] size_t getIndex() const
    {
        return index_;
    }

  private:
    std::shared_ptr<EventLoop> eventLoop_;
    std::string loopThreadName_;
    std::thread thread_;
    size_t index_{};
};

}

#endif  // DROGON_EVENTLOOPTHREAD_IPP
