//
// Created by neo on 5/8/24.
//

#ifndef DROGON_TCPCLIENT_HPP
#define DROGON_TCPCLIENT_HPP

#include <drogon/portable/pch.hpp>
#include <drogon/portable/InetAddress.hpp>
#include <drogon/portable/EventLoop.hpp>
#include <drogon/portable/TLSPolicy.hpp>
#include <drogon/portable/SSLContext.hpp>
#include <drogon/portable/NonCopyable.hpp>

namespace drogon {

class TcpClientImpl;

class TcpClient : NonCopyable {

  public:
    TcpClient(EventLoop *loop, const InetAddress &addr, const std::string &name);

    virtual ~TcpClient();

    void connect() {

    }

    [[nodiscard]] std::shared_ptr<TcpConnection> connection() const;

    void enableSSL(TLSPolicyPtr policy);

    void setSockOptCallback(const std::function<void(int)> &cb);

    void setConnectionCallback(const std::function<void(const TcpConnectionPtr &)> &cb);

    void setConnectionErrorCallback(const std::function<void()> &cb);

    void setMessageCallback(const std::function<void(const TcpConnectionPtr &, MsgBuffer *)> &cb);

    void setSSLErrorCallback(const std::function<void(SSLError)> &cb);

  protected:
    TcpClientImpl *impl_;
};

}

#endif  // DROGON_TCPCLIENT_HPP
