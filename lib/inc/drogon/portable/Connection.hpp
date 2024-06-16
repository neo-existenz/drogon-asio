//
// Created by neo on 5/3/24.
//

#ifndef DROGON_CONNECTION_HPP
#define DROGON_CONNECTION_HPP

#include <drogon/portable/pch.hpp>
#include <drogon/portable/MsgBuffer.hpp>
#include <drogon/portable/Resolver.hpp>
#include <drogon/portable/EventLoop.hpp>
#include <drogon/portable/InetAddress.hpp>
#include <drogon/portable/Certificate.hpp>

namespace drogon
{

class TcpConnectionImpl;

class TcpConnection
{
  public:
    virtual ~TcpConnection();

    explicit TcpConnection(EventLoop *loop);

    EventLoop *getLoop();

    void shutdown();

    bool connected();

    bool disconnected();

    void forceClose();

    [[nodiscard]] InetAddress peerAddr() const;

    [[nodiscard]] InetAddress localAddr() const;

    [[nodiscard]] bool isSSLConnection() const;

    CertificatePtr peerCertificate();

    void setContext(const std::shared_ptr<void> &context);

    void setContext(std::shared_ptr<void> &&context);

    [[nodiscard]] std::shared_ptr<void> getContextVoid() const;

    /**
     * @brief Clear the custom data.
     *
     */
    void clearContext();

    /**
     * @brief Return true if the custom data is set by user.
     *
     * @return true
     * @return false
     */
    [[nodiscard]] bool hasContext() const;

    /**
     * @brief Send a file to the peer.
     *
     * @param fileName in UTF-8
     * @param offset
     * @param length
     */
    void sendFile(const char *fileName, size_t offset = 0, size_t length = 0);

    void send(const MsgBuffer &buffer);

    void send(MsgBuffer &&buffer);

    void send(std::shared_ptr<MsgBuffer> &buffer);

    void send(const std::string &msg);

    void sendStream(const std::function<std::size_t(char *, std::size_t)>& callback);

    /**
     * @brief Get the custom data from the connection.
     *
     * @tparam T
     * @return std::shared_ptr<T>
     */
    template <typename T>
    std::shared_ptr<T> getContext() const
    {
        return std::static_pointer_cast<T>(getContextVoid());
    }

    [[nodiscard]] TcpConnectionImpl *getImpl() const noexcept
    {
        return impl_;
    }

  protected:
    TcpConnectionImpl *impl_;
};

using TcpConnectionPtr = std::shared_ptr<TcpConnection>;

using TcpConnectionCallback = std::function<void(const TcpConnectionPtr &sock)>;

using ResolverPtr = std::shared_ptr<Resolver>;

constexpr std::size_t kDefaultDNSTimeout = 3000;

}  // namespace drogon

#endif  // DROGON_CONNECTION_HPP
