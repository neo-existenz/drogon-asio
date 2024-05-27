//
// Created by neo on 5/26/24.
//

#ifndef DROGON_RESOLVER_IPP
#define DROGON_RESOLVER_IPP

#include <drogon/portable/portable.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "EventLoop.ipp"

namespace drogon
{

namespace asio = boost::asio;

class ResolverImpl
{
  public:
    using Callback = std::function<void(const InetAddress &)>;
    using ResolverResultsCallback =
        std::function<void(const std::vector<InetAddress> &)>;

    explicit ResolverImpl(EventLoop *loop, std::size_t size)
        : M_(loop->getImpl()->getIoContext())
    {
    }

    explicit ResolverImpl(EventLoop *loop) : M_(loop->getImpl()->getIoContext())
    {
    }

    void resolve(const std::string &domain, const Callback &callback)
    {
        M_.async_resolve(
            domain,
            [callback](const boost::system::error_code &error,
                       const asio::ip::tcp::resolver::results_type &result) {
                if (error)
                {
                    return;
                }
                for (const auto &endpoint : result)
                {
                    callback({endpoint.endpoint().address().to_string(),
                              endpoint.endpoint().port(),
                              endpoint.endpoint().address().is_v6()});
                }
            });
    }

    [[nodiscard]] static bool isCAresUsed()
    {
        return false;
    }

  private:
    asio::ip::tcp::resolver M_;
};

}  // namespace drogon

#endif  // DROGON_RESOLVER_IPP
