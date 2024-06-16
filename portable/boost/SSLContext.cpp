//
// Created by neo on 5/26/24.
//
#include <drogon/portable/Portable.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace drogon
{

class SSLContextImpl
{
  public:
    SSLContextImpl(
        bool useOldTLS,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds,
        bool server) : isServer(server)
    {
    }

    static SSLContextPtr newSSLContext(const TLSPolicy &policy, bool isServer)
    {
        return std::make_shared<SSLContext>(true,
                                            policy.sslConfCmds(),
                                            isServer);
    }

    bool isServer;
};

SSLContext::SSLContext(
    bool useOldTLS,
    const std::vector<std::pair<std::string, std::string>> &sslConfCmds,
    bool server)
{
    impl_ = new SSLContextImpl(useOldTLS, sslConfCmds, server);
}

}  // namespace drogon
