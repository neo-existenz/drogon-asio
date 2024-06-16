
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

#include <drogon/portable/portable.hpp>

#include "Connection.ipp"
#include "EventLoop.ipp"

namespace drogon
{

class TcpServerImpl : protected EventLoop
{
  public:
    typedef boost::asio::detail::socket_option::boolean<SOL_SOCKET, SO_REUSEPORT> reuse_port;
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
    TcpServerImpl(EventLoop* loop,
                  const InetAddress& address,
                  std::string name,
                  bool reUseAddr = true,
                  bool reUsePort = true)
        : loop_(loop),
          acceptor_(loop->getImpl()->getIoContext()),
          address_(address),
          endpoint_(asio::ip::make_address(address.toIpNetEndian()),
                    address.portNetEndian()),
          name_(std::move(name))
    {
        acceptor_.open(endpoint_.protocol());
        acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(reUseAddr));
        acceptor_.set_option(reuse_port(reUsePort));
        acceptor_.bind(endpoint_);
        acceptor_.listen();
    }

    void start()
    {
        auto conn = std::make_shared<TcpConnection>(loop_);
        acceptor_.async_accept(
            conn->getImpl()->socket_, [conn, this](std::error_code ec) {
                if (!ec)
                {
                    LOG_INFO << conn->getImpl()->socket_.remote_endpoint();
                    if (connectionCallback_)
                    {
                        connectionCallback_(conn);
                    }
                    if (recvMessageCallback_)
                    {
                        conn->getImpl()->receive(conn, recvMessageCallback_, connectionCallback_);
                    }
                    start();
                } else {
                    std::cout << ec.message() << '\n';
                }
            });
    }

    void stop()
    {
        acceptor_.close();
    }

    [[nodiscard]] const InetAddress& address() const
    {
        return address_;
    }

    void enableSSL(const std::string& certPath, const std::string& keyPath)
    {
        sslContext_->use_certificate_file(certPath, asio::ssl::context::pem);
        sslContext_->use_private_key_file(keyPath, asio::ssl::context::pem);
        useSSL_ = true;
        //        asio::ssl::context sslContext(asio::ssl::context::sslv23);
        //        sslContext.set_options(
        //            asio::ssl::context::default_workarounds
        //            | asio::ssl::context::no_sslv2
        //            | asio::ssl::context::single_dh_use);
        //        sslContext.set_password_callback(...);//如果需要,设置密码回调函数
        //        sslContext.use_certificate_chain_file("server.crt");//设置服务器证书
        //        sslContext.use_private_key_file("server.key",
        //        asio::ssl::context::pem);//设置私钥
        //        sslContext.use_tmp_dh_file("dh512.pem");//设置临时DH参数(可选)
    }
    /**
     * @brief Enable SSL encryption.
     */
    void enableSSL(TLSPolicyPtr policy)
    {
        policyPtr_ = std::move(policy);

        sslContext_ = new asio::ssl::context(asio::ssl::context::sslv23);
    }

    void setIoLoopThreadPool(const std::shared_ptr<EventLoopThreadPool>& pool)
    {
        // Set the I/O loop thread pool
        ioLoopThreadPool_ = pool;
    }

    void setIoLoops(const std::vector<EventLoop*>& ioLoops)
    {
        // Set the I/O loops
        ioLoops_ = ioLoops;
    }

    void kickoffIdleConnections(size_t timeout)
    {
        // Kick off idle connections after timeout seconds
        // This implementation depends on your specific requirements
    }

    [[nodiscard]] EventLoop* getLoop() const
    {
        // Get the event loop of the server
        return loop_;
    }

    [[nodiscard]] std::vector<EventLoop*> getIoLoops() const
    {
        // Get the I/O event loops of the server
        return ioLoops_;
    }

    std::string name_;
    asio::ip::tcp::acceptor acceptor_;
    asio::ip::tcp::endpoint endpoint_;
    asio::ssl::context* sslContext_{};
    bool useSSL_ = false;
    TLSPolicyPtr policyPtr_;
    std::shared_ptr<EventLoopThreadPool> ioLoopThreadPool_;
    std::vector<EventLoop*> ioLoops_;
    InetAddress address_;
    EventLoop* loop_{};
    TcpServer::ConnectionCallback connectionCallback_;
    TcpServer::RecvMessageCallback recvMessageCallback_;
};

TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& address,
                     const std::string& name,
                     bool reUseAddr,
                     bool reUsePort)
{
    impl_ = new TcpServerImpl(loop, address, name, reUseAddr, reUsePort);
}

TcpServer::~TcpServer()
{
    delete impl_;
}

std::string TcpServer::name()
{
    return impl_->name_;
}

void TcpServer::start()
{
    impl_->start();
}

void TcpServer::stop()
{
    impl_->stop();
}

const InetAddress& TcpServer::address() const
{
    return impl_->address_;
}

void TcpServer::enableSSL(const std::string& certPath,
                          const std::string& keyPath)
{
    impl_->enableSSL(certPath, keyPath);
}

void TcpServer::enableSSL(TLSPolicyPtr policy)
{
    impl_->enableSSL(std::move(policy));
}

void TcpServer::setIoLoopThreadPool(
    const std::shared_ptr<EventLoopThreadPool>& pool)
{
    impl_->setIoLoopThreadPool(pool);
}

void TcpServer::setIoLoops(const std::vector<EventLoop*>& ioLoops)
{
    impl_->setIoLoops(ioLoops);
}

void TcpServer::kickoffIdleConnections(size_t timeout)
{
    impl_->kickoffIdleConnections(timeout);
}

EventLoop* TcpServer::getLoop() const
{
    return impl_->getLoop();
}

std::vector<EventLoop*> TcpServer::getIoLoops() const
{
    return impl_->getIoLoops();
}

void TcpServer::setConnectionCallback(const TcpServer::ConnectionCallback& cb)
{
    impl_->connectionCallback_ = cb;
}

void TcpServer::setConnectionCallback(TcpServer::ConnectionCallback&& cb)
{
    impl_->connectionCallback_ = cb;
}

void TcpServer::setRecvMessageCallback(const TcpServer::RecvMessageCallback& cb)
{
    impl_->recvMessageCallback_ = cb;
}

void TcpServer::setRecvMessageCallback(TcpServer::RecvMessageCallback&& cb)
{
    impl_->recvMessageCallback_ = cb;
}

}  // namespace drogon
