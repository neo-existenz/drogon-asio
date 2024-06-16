//
// Created by neo on 5/26/24.
//

#ifndef DROGON_EVENTLOOPTHREADPOOL_IPP
#define DROGON_EVENTLOOPTHREADPOOL_IPP

#include <drogon/portable/Portable.hpp>

namespace drogon
{

class EventLoopThreadPoolImpl
{
  public:
    explicit EventLoopThreadPoolImpl(
        size_t threadNum,
        const std::string &name = "EventLoopThreadPool")
    {
        for (size_t i = 0; i < threadNum; ++i)
        {
            auto thread = std::make_shared<EventLoopThread>(name);
            threads_.emplace_back(thread);
        }
    }

    size_t size()
    {
        return threads_.size();
    }

    EventLoop *getLoop(size_t id)
    {
        if (id < threads_.size())
        {
            return threads_[id]->getLoop();
        }
        return {};
    }

    EventLoop *getNextLoop()
    {
        if (!threads_.empty())
        {
            size_t index = loopIndex_.fetch_add(1, std::memory_order_relaxed);
            EventLoop *loop = threads_[index % threads_.size()]->getLoop();
            return loop;
        }
        return nullptr;
    }

    std::vector<EventLoop *> getLoops()
    {
        std::vector<EventLoop *> vec;
        vec.reserve(threads_.size());
        for (auto &thread : threads_)
        {
            vec.emplace_back(thread->getLoop());
        }
        return vec;
    }

    void start()
    {
        for (auto &thread : threads_)
        {
            thread->start();
        }
    }

    void stop()
    {
        for (auto &thread : threads_)
        {
            thread->stop();
        }
    }

    void wait()
    {
        for (auto &thread : threads_)
        {
            thread->wait();
        }
    }

  private:
    std::vector<std::shared_ptr<EventLoopThread>> threads_{};
    std::atomic<size_t> loopIndex_{0};
};

}  // namespace drogon

#endif  // DROGON_EVENTLOOPTHREADPOOL_IPP
