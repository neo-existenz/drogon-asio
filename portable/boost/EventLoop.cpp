//
// Created by neo on 5/26/24.
//

#include <drogon/portable/portable.hpp>

#include <utility>

#include "EventLoop.ipp"
#include "EventLoopThread.ipp"
#include "EventLoopThreadPool.ipp"

namespace drogon
{

namespace asio = boost::asio;

thread_local EventLoop *EventLoop::LocalThreadEventLoop::LocalEventLoop =
    nullptr;

EventLoop::TimerId EventLoop::InvalidTimerId = nullptr;

EventLoop::~EventLoop()
{
    delete impl_;
}

EventLoop::EventLoop()
{
    impl_ = new EventLoopImpl();
}

void EventLoop::setIndex(size_t index)
{
    impl_->setIndex(index);
}

std::size_t EventLoop::index() const
{
    return impl_->index();
}

void EventLoop::resetTimerQueue()
{
    impl_->resetTimerQueue();
}

void EventLoop::resetAfterFork()
{
    impl_->resetAfterFork();
}

bool EventLoop::isInLoopThread()
{
    return impl_->isInLoopThread();
}

void EventLoop::moveToCurrentThread()
{
    if (EventLoop::LocalThreadEventLoop::get())
    {
        throw std::runtime_error(
            "There is already an EventLoop in this thread, you cannot "
            "move another in");
    }
    EventLoop::LocalThreadEventLoop::set(this);
}

void EventLoop::loop()
{
    impl_->loop();
}

bool EventLoop::isRunning()
{
    return impl_->isRunning();
}

void EventLoop::runAfter(std::chrono::duration<double> delay,
                         const EventLoop::Func &cb)
{
    impl_->runAfter(delay, cb);
}

void EventLoop::runAfter(double delay, const EventLoop::Func &cb)
{
    impl_->runAfter(delay, cb);
}

void EventLoop::runAt(const Date &date, const EventLoop::Func &cb)
{
    impl_->runAt(date, cb);
}

void EventLoop::dispatch(const EventLoop::Func &cb)
{
    impl_->dispatch(cb);
}

void EventLoop::post(const EventLoop::Func &cb)
{
    impl_->post(cb);
}

EventLoop::TimerId EventLoop::runEvery(double interval,
                                       const EventLoop::Func &cb)
{
    return impl_->runEvery(interval, cb);
}

void EventLoop::invalidateTimer(const EventLoop::TimerId &timerId)
{
    impl_->invalidateTimer(timerId);
}

void EventLoop::runOnQuit(const EventLoop::Func &cb)
{
    impl_->runOnQuit(cb);
}

void EventLoop::quit()
{
    impl_->quit();
}

void EventLoop::assertInLoopThread()
{
    impl_->assertInLoopThread();
}

/**
 * EventLoopThread
 */
EventLoopThread::~EventLoopThread()
{
    delete impl_;
}

EventLoopThread::EventLoopThread(const std::string &threadName)
{
    impl_ = new EventLoopThreadImpl(threadName);
}

EventLoop *EventLoopThread::getLoop()
{
    return impl_->getLoop();
}

void EventLoopThread::start()
{
    impl_->start();
}

void EventLoopThread::stop()
{
    impl_->stop();
}

void EventLoopThread::wait()
{
    impl_->wait();
}

void EventLoopThread::run()
{
    impl_->run();
}

/**
 * EventLoopThreadPool
 */
EventLoopThreadPool::~EventLoopThreadPool()
{
    delete impl_;
}

EventLoopThreadPool::EventLoopThreadPool(size_t threadNum,
                                         const std::string &name)
{
    impl_ = new EventLoopThreadPoolImpl(threadNum, name);
}

size_t EventLoopThreadPool::size()
{
    return impl_->size();
}

EventLoop *EventLoopThreadPool::getLoop(size_t id)
{
    return impl_->getLoop(id);
}

EventLoop *EventLoopThreadPool::getNextLoop()
{
    return impl_->getNextLoop();
}

std::vector<EventLoop *> EventLoopThreadPool::getLoops()
{
    return impl_->getLoops();
}

void EventLoopThreadPool::start()
{
    impl_->start();
}

void EventLoopThreadPool::stop()
{
    impl_->stop();
}

void EventLoopThreadPool::wait()
{
    impl_->wait();
}

}  // namespace drogon
