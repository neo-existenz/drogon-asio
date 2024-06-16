//
// Created by neo on 5/9/24.
//

#ifndef DROGON_SSLCONTEXT_HPP
#define DROGON_SSLCONTEXT_HPP

#include <drogon/portable/Common.hpp>

namespace drogon
{

class SSLContext;

using SSLContextPtr = std::shared_ptr<SSLContext>;

class SSLContextImpl;

class SSLContext
{
  public:
    SSLContext(
        bool useOldTLS,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds,
        bool server);

    static SSLContextPtr newSSLContext(const TLSPolicy &policy,
                                                bool isServer)
    {
        return std::make_shared<SSLContext>(
            true,
            policy.sslConfCmds(),
            isServer);
    }

  protected:
    SSLContextImpl *impl_;
};

}  // namespace drogon

#endif  // DROGON_SSLCONTEXT_HPP
