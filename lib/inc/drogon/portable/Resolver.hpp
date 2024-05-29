//
// Created by neo on 5/26/24.
//

#ifndef DROGON_RESOLVER_HPP
#define DROGON_RESOLVER_HPP

#include <drogon/portable/pch.hpp>
#include <drogon/portable/InetAddress.hpp>
#include <drogon/portable/EventLoop.hpp>

namespace drogon
{

class ResolverImpl;

class Resolver
{
  public:
    using Callback = std::function<void(const InetAddress &)>;
    using ResolverResultsCallback =
        std::function<void(const std::vector<InetAddress> &)>;

    explicit Resolver(EventLoop *loop, std::size_t size);

    explicit Resolver(EventLoop *loop);

    void resolve(const std::string &domain, const Callback &callback);

    [[nodiscard]] static bool isCAresUsed() {
        return false;
    }

    static std::shared_ptr<Resolver> newResolver(EventLoop *loop,
                                                 std::size_t size)
    {
        return std::make_shared<Resolver>(loop, size);
    }

    static std::shared_ptr<Resolver> newResolver(EventLoop *loop)
    {
        return std::make_shared<Resolver>(loop);
    }

    static std::shared_ptr<Resolver> newResolver()
    {
        return std::make_shared<Resolver>(EventLoop::getEventLoopOfCurrentThread());
    }

  protected:
    ResolverImpl *impl_;
};

}  // namespace drogon

#endif  // DROGON_RESOLVER_HPP
