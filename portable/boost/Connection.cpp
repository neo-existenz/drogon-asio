//
// Created by neo on 5/26/24.
//
#include <drogon/portable/portable.hpp>

#include "Connection.ipp"

drogon::TcpConnection::~TcpConnection()
{
    delete impl_;
}

drogon::TcpConnection::TcpConnection(EventLoop *loop)
{
    impl_ = new TcpConnectionImpl(loop->getImpl());
}

bool drogon::TcpConnection::isSSLConnection() const
{
    return impl_->isSSLConnection();
}

drogon::EventLoop *drogon::TcpConnection::getLoop()
{
    return impl_->getLoop();
}

void drogon::TcpConnection::shutdown()
{
    impl_->shutdown();
}

bool drogon::TcpConnection::connected()
{
    return impl_->connected();
}

bool drogon::TcpConnection::disconnected()
{
    return impl_->disconnected();
}

void drogon::TcpConnection::forceClose()
{
    impl_->forceClose();
}

drogon::InetAddress drogon::TcpConnection::peerAddr() const
{
    return impl_->peerAddr();
}

drogon::InetAddress drogon::TcpConnection::localAddr() const
{
    return impl_->localAddr();
}

drogon::CertificatePtr drogon::TcpConnection::peerCertificate()
{
    return impl_->peerCertificate();
}

void drogon::TcpConnection::setContext(const std::shared_ptr<void> &context)
{
}

void drogon::TcpConnection::setContext(std::shared_ptr<void> &&context)
{
}

std::shared_ptr<void> drogon::TcpConnection::getContextVoid() const
{
    return std::shared_ptr<void>();
}

void drogon::TcpConnection::clearContext()
{
}

bool drogon::TcpConnection::hasContext() const
{
    return false;
}

void drogon::TcpConnection::sendFile(const char *fileName,
                                     size_t offset,
                                     size_t length)
{
}

void drogon::TcpConnection::send(const drogon::MsgBuffer &buffer)
{
}

void drogon::TcpConnection::send(drogon::MsgBuffer &&buffer)
{
}

void drogon::TcpConnection::send(std::shared_ptr<MsgBuffer> &buffer)
{
}

void drogon::TcpConnection::send(const std::string &msg)
{
}

void drogon::TcpConnection::sendStream(
    std::function<std::size_t(char *, std::size_t)> callback)
{
}
