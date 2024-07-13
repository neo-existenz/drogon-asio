//
// Created by neo on 5/26/24.
//

#ifndef DROGON_CONNECTION_IPP
#define DROGON_CONNECTION_IPP

#include <drogon/portable/Portable.hpp>

#include <fstream>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include "EventLoop.ipp"
#include "Connection.ipp"
#include "MsgBuffer.ipp"

namespace drogon
{

namespace asio = boost::asio;
using error_code = boost::system::error_code;

class TcpConnectionImpl : public std::enable_shared_from_this<TcpConnectionImpl>
{
  public:
    explicit TcpConnectionImpl(EventLoop *loop)
        : loop_(loop),
          io_context_(loop->getImpl()->getIoContext()),
          socket_(io_context_)
    {
    }

    [[nodiscard]] EventLoop *getLoop() const
    {
        return loop_;
    }

    void shutdown()
    {
        io_context_.post([this]() {
            error_code ec =
                socket_.shutdown(asio::ip::tcp::socket::shutdown_both, ec);
            if (ec)
            {
                LOG_ERROR << "Error: " << ec.message();
            }
        });
    }

    bool connected()
    {
        return socket_.is_open();
    }

    bool disconnected()
    {
        return !socket_.is_open();
    }

    void forceClose()
    {
        socket_.close();
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

    void sendFile1(const std::shared_ptr<std::ifstream> &file,
                   const std::shared_ptr<asio::streambuf> &buf_ptr,
                   std::size_t frameSize,
                   std::size_t remaining)
    {
        auto mu = buf_ptr->prepare(frameSize);
        file->read(static_cast<char *>(mu.data()), mu.size());
        buf_ptr->commit(frameSize);
        // Send the buffer
        asio::async_write(
            socket_,
            *buf_ptr,
            [file, buf_ptr, frameSize, remaining, this](const error_code &ec,
                                                        size_t len) {
                if (!ec)
                {
                    assert(len == frameSize);
                    LOG_DEBUG << "Sent: " << frameSize << " bytes";
                    buf_ptr->consume(frameSize);
                    size_t remaining_next = remaining - frameSize;
                    size_t frameSize_next;
                    if (remaining_next > 0)
                    {
                        frameSize_next = std::min(remaining_next, frameSize);
                        sendFile1(file,
                                  buf_ptr,
                                  frameSize_next,
                                  remaining_next);
                    }
                }
                else
                {
                    LOG_ERROR << "Error: " << ec.message();
                }
            });
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
        static size_t maxFileSize = 1024 * 1024 * 1024;  // 1GB
        auto file =
            std::make_shared<std::ifstream>(fileName,
                                            std::ios::in | std::ios::binary);
        if (!file->good())
        {
            LOG_ERROR << "Failed to open file: " << fileName;
            return;
        }

        struct stat filestat
        {
        };
        if (stat(fileName, &filestat) < 0)
        {
            LOG_SYSERR << fileName << " stat error";
            return;
        }

        size_t remaining;

        if (length > 0)
        {
            remaining = length;
        }
        else
        {
            remaining = filestat.st_size - offset;
        }

        size_t frameSize = std::min(remaining, maxFileSize);

        // Skip to the offset
        file->seekg(offset, std::ios::beg);

        // Create a streambuf for the file
        auto buf_ptr = std::make_shared<asio::streambuf>();
        sendFile1(file, buf_ptr, frameSize, remaining);
    }

    void send(const MsgBuffer &buffer)
    {
        auto buf = std::make_shared<MsgBuffer>(buffer);
        asio::async_write(socket_,
                          static_cast<asio::streambuf &>(*buf->impl_),
                          [buf, this](const error_code &ec, size_t len) {
                              if (!ec)
                              {
                                  LOG_DEBUG << "Sent: " << len << " bytes";
                              }
                              else
                              {
                                  LOG_ERROR << "Error: " << ec.message();
                              }
                          });
    }

    void send(MsgBuffer &&buffer)
    {
        auto buf = std::make_shared<MsgBuffer>(buffer);
        asio::async_write(socket_,
                          static_cast<asio::streambuf &>(*buf->impl_),
                          [buf, this](const error_code &ec, size_t len) {
                              if (!ec)
                              {
                                  LOG_DEBUG << "Sent: " << len << " bytes";
                              }
                              else
                              {
                                  LOG_ERROR << "Error: " << ec.message();
                              }
                          });
    }

    void send(std::shared_ptr<MsgBuffer> &buffer)
    {
        asio::async_write(socket_,
                          static_cast<asio::streambuf &>(*buffer->impl_),
                          [buffer, this](const error_code &ec, size_t len) {
                              if (!ec)
                              {
                                  LOG_DEBUG << "Sent: " << len << " bytes";
                              }
                              else
                              {
                                  LOG_ERROR << "Error: " << ec.message();
                              }
                          });
    }

    void send(const std::string &msg)
    {
        auto msg_ptr = std::make_shared<std::string>(msg);
        asio::async_write(socket_,
                          asio::buffer(*msg_ptr),
                          [msg_ptr, this](const error_code &ec, size_t len) {
                              if (!ec)
                              {
                                  LOG_DEBUG << "Sent: " << len << " bytes";
                              }
                              else
                              {
                                  LOG_ERROR << "Error: " << ec.message();
                              }
                          });
    }

    void sendStream(
        const std::function<std::size_t(char *, std::size_t)> &callback)
    {
        auto buf = std::make_shared<std::array<char, 1024>>();
        size_t len = callback(buf->data(), buf->size());
        if (len == 0)
        {
            return;
        }
        asio::async_write(socket_,
                          asio::buffer(buf->data(), len),
                          [buf, callback, this](const error_code &ec,
                                                size_t len) {
                              if (!ec)
                              {
                                  LOG_DEBUG << "Sent: " << len << " bytes";
                                  sendStream(callback);
                              }
                              else
                              {
                                  LOG_ERROR << "Error: " << ec.message();
                              }
                          });
    }

    void do_receive(const TcpConnectionPtr &conn)
    {
        asio::async_read_until(socket_,
                               static_cast<asio::streambuf &>(*buffer_.impl_),
                               std::string_view{"\n"},
                               [conn, this](const error_code &ec, size_t len) {
                                   if (!ec)
                                   {
                                       LOG_DEBUG << "Received: " << len
                                                 << ", Buffered: "
                                                 << buffer_.readableBytes()
                                                 << " bytes";
//                                       char *tmp =
//                                           const_cast<char *>();
                                       std::cout << buffer_.peek();
                                       recv_callback_(conn, &buffer_);
                                       buffer_.retrieve(len);
                                       do_receive(conn);
                                   }
                                   else  // if (ec == asio::error::eof)
                                   {
                                       // The remote host closed the connection
                                       conn->forceClose();
                                       conn_callback_(conn);
                                       LOG_ERROR << "Error: " << ec.message();
                                   }
                               });
    }

    void receive(const TcpConnectionPtr &conn,
                 const TcpServer::RecvMessageCallback &callback,
                 const TcpServer::ConnectionCallback &connCallback)
    {
        recv_callback_ = callback;
        conn_callback_ = connCallback;
        do_receive(conn);
    }

    EventLoop *loop_{};
    asio::io_context &io_context_;
    asio::ip::tcp::socket socket_;
    std::shared_ptr<void> contextPtr_;
    MsgBuffer buffer_;
    TcpServer::RecvMessageCallback recv_callback_;
    TcpServer::ConnectionCallback conn_callback_;
};

}  // namespace drogon

#endif  // DROGON_CONNECTION_IPP
