//
// Created by neo on 5/26/24.
//

#include "Connection.ipp"

namespace drogon
{

TcpConnection::~TcpConnection()
{
    delete impl_;
}

TcpConnection::TcpConnection(EventLoop *loop)
{
    impl_ = new TcpConnectionImpl(loop);
}

bool TcpConnection::isSSLConnection() const
{
    return impl_->isSSLConnection();
}

EventLoop *TcpConnection::getLoop()
{
    return impl_->getLoop();
}

void TcpConnection::shutdown()
{
    impl_->shutdown();
}

bool TcpConnection::connected()
{
    return impl_->connected();
}

bool TcpConnection::disconnected()
{
    return impl_->disconnected();
}

void TcpConnection::forceClose()
{
    impl_->forceClose();
}

InetAddress TcpConnection::peerAddr() const
{
    return impl_->peerAddr();
}

InetAddress TcpConnection::localAddr() const
{
    return impl_->localAddr();
}

CertificatePtr TcpConnection::peerCertificate()
{
    return impl_->peerCertificate();
}

void TcpConnection::setContext(const std::shared_ptr<void> &context)
{
    impl_->setContext(context);
}

void TcpConnection::setContext(std::shared_ptr<void> &&context)
{
    impl_->setContext(std::move(context));
}

std::shared_ptr<void> TcpConnection::getContextVoid() const
{
    return impl_->contextPtr_;
}

void TcpConnection::clearContext()
{
    impl_->clearContext();
}

bool TcpConnection::hasContext() const
{
    return impl_->hasContext();
}

void TcpConnection::sendFile(const char *fileName, size_t offset, size_t length)
{
    impl_->sendFile(fileName, offset, length);
}

void TcpConnection::send(const MsgBuffer &buffer)
{
    impl_->send(buffer);
}

void TcpConnection::send(MsgBuffer &&buffer)
{
    impl_->send(buffer);
}

void TcpConnection::send(std::shared_ptr<MsgBuffer> &buffer)
{
    impl_->send(buffer);
}

void TcpConnection::send(const std::string &msg)
{
    impl_->send(msg);
}

void TcpConnection::sendStream(
    const std::function<std::size_t(char *, std::size_t)>& callback)
{
    impl_->sendStream(callback);
}

}  // namespace drogon
