//
// Created by neo on 5/26/24.
//

#include <drogon/portable/portable.hpp>

#include <boost/asio.hpp>
#include "drogon/portable/MsgBuffer.hpp"

#include "MsgBuffer.ipp"

namespace asio = boost::asio;

namespace drogon
{

MsgBuffer::~MsgBuffer()
{
    delete impl_;
}

MsgBuffer::MsgBuffer(size_t len)
{
    impl_ = new MsgBufferImpl(len);
}

MsgBuffer::MsgBuffer(const MsgBuffer& msg)
{
    impl_ = new MsgBufferImpl(msg);
}

MsgBuffer& MsgBuffer::operator=(MsgBuffer&& other) noexcept
{
    if (this != &other)
    {
        delete impl_;
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }
    return *this;
}

void MsgBuffer::append(const void* data, std::size_t len)
{
    impl_->append(data, len);
}
void MsgBuffer::append(const MsgBuffer& buffer)
{
    impl_->append(buffer);
}
void MsgBuffer::append(const std::string& str)
{
    impl_->append(str);
}
void MsgBuffer::ensureWritableBytes(std::size_t len)
{
    impl_->ensureWritableBytes(len);
}
char* MsgBuffer::beginWrite()
{
    return impl_->beginWrite();
}
const char* MsgBuffer::peek() const
{
    return impl_->peek();
}
void MsgBuffer::hasWritten(std::size_t len)
{
    impl_->hasWritten(len);
}
void MsgBuffer::unwrite(int len)
{
    impl_->unwrite(len);
}
std::size_t MsgBuffer::readableBytes() const
{
    return impl_->readableBytes();
}
std::size_t MsgBuffer::writableBytes() const
{
    return impl_->writableBytes();
}
const char* MsgBuffer::findCRLF()
{
    return impl_->findCRLF();
}
void MsgBuffer::retrieveUntil(const char* end)
{
    impl_->retrieveUntil(end);
}
void MsgBuffer::retrieve(size_t len)
{
    impl_->retrieve(len);
}
void MsgBuffer::retrieveAll()
{
    impl_->retrieveAll();
}
const char& MsgBuffer::operator[](size_t offset) const
{
    return impl_->operator[](offset);
}
char& MsgBuffer::operator[](size_t offset)
{
    return impl_->operator[](offset);
}

}  // namespace drogon
