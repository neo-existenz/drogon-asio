//
// Created by neo on 5/4/24.
//

#ifndef DROGON_TCPSERVER_HPP
#define DROGON_TCPSERVER_HPP

#include <drogon/portable/Common.hpp>
#include <drogon/portable/asio/Connection.hpp>
#include <drogon/portable/asio/EventLoop.hpp>
#include <drogon/portable/asio/InetAddress.hpp>
#include <drogon/portable/asio/MsgBuffer.hpp>
#include <drogon/portable/asio/TLSPolicy.hpp>

namespace drogon
{

class TcpServerImpl;

class TcpServer
{
  public:
    using RecvMessageCallback =
        std::function<void(const TcpConnectionPtr&, MsgBuffer*)>;

    using ConnectionCallback = std::function<void(const TcpConnectionPtr&)>;

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
