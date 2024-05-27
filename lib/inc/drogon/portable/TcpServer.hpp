//
// Created by neo on 5/4/24.
//

#ifndef DROGON_TCPSERVER_HPP
#define DROGON_TCPSERVER_HPP

#include <drogon/portable/pch.hpp>
#include <drogon/portable/Connection.hpp>
#include <drogon/portable/EventLoop.hpp>
#include <drogon/portable/InetAddress.hpp>
#include <drogon/portable/MsgBuffer.hpp>
#include <drogon/portable/TLSPolicy.hpp>

namespace drogon
{

class TcpServerImpl;

class TcpServer
{
    using RecvMessageCallback =
        std::function<void(const TcpConnectionPtr&, MsgBuffer*)>;

    using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

  public:
    /**
     * @brief Construct a new TCP server instance.
     *
     * @param loop The event loop in which the acceptor of the server is
     * handled.
     * @param address The address of the server.
     * @param name The name of the server.
     * @param reUseAddr The SO_REUSEADDR option.
     * @param reUsePort The SO_REUSEPORT option.
     */
    TcpServer(EventLoop* loop,
              const InetAddress& address,
              const std::string& name,
              bool reUseAddr = true,
              bool reUsePort = true);

    virtual ~TcpServer();

    std::string name();

    void start();

    void stop();

    [[nodiscard]] const InetAddress& address() const;

    void enableSSL(const std::string& certPath, const std::string& keyPath);

    /**
     * @brief Enable SSL encryption.
     */
    void enableSSL(TLSPolicyPtr policy);

    void setIoLoopThreadPool(const std::shared_ptr<EventLoopThreadPool>& pool);

    void setIoLoopNum(int numThreads);

    void setIoLoops(const std::vector<EventLoop*>& ioLoops);

    void kickoffIdleConnections(size_t timeout);

    [[nodiscard]] EventLoop* getLoop() const;

    [[nodiscard]] std::vector<EventLoop*> getIoLoops() const;

    void setConnectionCallback(const ConnectionCallback& cb);
    void setConnectionCallback(ConnectionCallback&& cb);
    void setRecvMessageCallback(const RecvMessageCallback& cb);

    void setRecvMessageCallback(RecvMessageCallback&& cb);

  protected:
    TcpServerImpl* impl_;
};

}  // namespace drogon

#endif  // DROGON_TCPSERVER_HPP
