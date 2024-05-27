//
// Created by neo on 5/26/24.
//

#ifndef DROGON_CONNECTION_IPP
#define DROGON_CONNECTION_IPP

#include <drogon/portable/portable.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "EventLoop.ipp"

namespace drogon
{

namespace asio = boost::asio;

class TcpConnectionImpl
{
  public:
    explicit TcpConnectionImpl(EventLoopImpl *loop)
        : socket_(loop->getIoContext())
    {
    }

    EventLoop *getLoop()
    {
        return loop_;
    }

    void shutdown()
    {
    }

    bool connected()
    {
        return false;
    }

    bool disconnected()
    {
        return false;
    }

    void forceClose()
    {
    }

    [[nodiscard]] InetAddress peerAddr() const
    {
        auto endPoint = socket_.remote_endpoint();
        auto addr = endPoint.address();
        return {addr.to_string(), endPoint.port(), addr.is_v6()};
    }

    [[nodiscard]] InetAddress localAddr() const
    {
        auto endPoint = socket_.local_endpoint();
        auto addr = endPoint.address();
        return {addr.to_string(), endPoint.port(), addr.is_v6()};
    }

    [[nodiscard]] bool isSSLConnection() const
    {
        // 你需要根据你的实现来决定如何判断一个连接是否是SSL连接
        // 这里只是一个示例，实际上asio::ip::tcp::socket并没有is_open_ssl()方法
        // return socket_.is_open_ssl();
        return false;
    }

    CertificatePtr peerCertificate()
    {
        // 你需要根据你的实现来获取对等方的证书
        // 这里只是一个示例，实际上asio::ip::tcp::socket并没有get_peer_certificate()方法
        // return socket_.get_peer_certificate();
        return std::make_shared<Certificate>();
    }

    /**
     * @brief Set the custom data on the connection.
     *
     * @param context
     */
    void setContext(const std::shared_ptr<void> &context)
    {
        contextPtr_ = context;
    }

    void setContext(std::shared_ptr<void> &&context)
    {
        contextPtr_ = std::move(context);
    }

    /**
     * @brief Get the custom data from the connection.
     *
     * @tparam T
     * @return std::shared_ptr<T>
     */
    template <typename T>
    std::shared_ptr<T> getContext() const
    {
        return std::static_pointer_cast<T>(contextPtr_);
    }

    /**
     * @brief Clear the custom data.
     *
     */
    void clearContext()
    {
        contextPtr_.reset();
    }

    /**
     * @brief Return true if the custom data is set by user.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool hasContext() const
    {
        return (bool)contextPtr_;
    }

    /**
     * @brief Send a file to the peer.
     *
     * @param fileName in UTF-8
     * @param offset
     * @param length
     */
    void sendFile(const char *fileName, size_t offset = 0, size_t length = 0)
    {
    }

    void send(const MsgBuffer &buffer)
    {
    }

    void send(MsgBuffer &&buffer)
    {
    }

    void send(std::shared_ptr<MsgBuffer> &buffer)
    {
    }

    void send(const std::string &msg)
    {
    }

    void sendStream(std::function<std::size_t(char *, std::size_t)> callback)
    {
    }

  private:
    EventLoop *loop_{};
    asio::ip::tcp::socket socket_;
    std::shared_ptr<void> contextPtr_;
};

}  // namespace drogon
#endif  // DROGON_CONNECTION_IPP
