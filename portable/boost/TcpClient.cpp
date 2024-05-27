//
// Created by neo on 5/26/24.
//

#include <drogon/portable/portable.hpp>
#include "drogon/portable/TcpClient.hpp"

namespace drogon
{

class TcpClientImpl : NonCopyable
{
  public:
    TcpClientImpl(EventLoop *loop,
                  const InetAddress &addr,
                  const std::string &name)
    {
    }

    void connect()
    {
    }

    [[nodiscard]] std::shared_ptr<TcpConnection> connection() const
    {
        return connPtr_;
    }

    void enableSSL(TLSPolicyPtr policy)
    {
        tlsPolicyPtr_ = std::move(policy);
        sslContextPtr_ = SSLContext::newSSLContext(*tlsPolicyPtr_, false);
    }

    void setSockOptCallback(const std::function<void(int)> &cb)
    {
    }

    void setConnectionCallback(
        const std::function<void(const TcpConnectionPtr &)> &cb)
    {
    }

    void setConnectionErrorCallback(const std::function<void()> &cb)
    {
    }

    void setMessageCallback(
        const std::function<void(const TcpConnectionPtr &, MsgBuffer *)> &cb)
    {
    }

    void setSSLErrorCallback(const std::function<void(SSLError)> &cb)
    {
    }

    TcpConnectionPtr connPtr_;
    TLSPolicyPtr tlsPolicyPtr_;
    SSLContextPtr sslContextPtr_;
    bool validateCert_{false};
};

TcpClient::TcpClient(EventLoop *loop,
                     const InetAddress &addr,
                     const std::string &name)
{
    impl_ = new TcpClientImpl(loop, addr, name);
}

TcpClient::~TcpClient()
{
    delete impl_;
}

std::shared_ptr<TcpConnection> TcpClient::connection() const
{
    return impl_->connPtr_;
}

void TcpClient::enableSSL(TLSPolicyPtr policy)
{
    impl_->enableSSL(std::move(policy));
}

void TcpClient::setSockOptCallback(const std::function<void(int)> &cb)
{
    impl_->setSockOptCallback(cb);
}

void TcpClient::setConnectionCallback(
    const std::function<void(const TcpConnectionPtr &)> &cb)
{
    impl_->setConnectionCallback(cb);
}

void TcpClient::setConnectionErrorCallback(const std::function<void()> &cb)
{
    impl_->setConnectionErrorCallback(cb);
}

void TcpClient::setMessageCallback(
    const std::function<void(const TcpConnectionPtr &, MsgBuffer *)> &cb)
{
    impl_->setMessageCallback(cb);
}

void TcpClient::setSSLErrorCallback(const std::function<void(SSLError)> &cb)
{
    impl_->setSSLErrorCallback(cb);
}

}  // namespace drogon
