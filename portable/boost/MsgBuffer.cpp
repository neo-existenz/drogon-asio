//
// Created by neo on 5/26/24.
//

#include <drogon/portable/Portable.hpp>

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

void MsgBuffer::swap(MsgBuffer& buf) noexcept
{
    impl_->swap_impl(*buf.impl_);
}

void MsgBuffer::addInFront(const char* buf, size_t len)
{
    MsgBuffer newBuf;
    newBuf.append(buf, len);
    newBuf.append(*this);
    swap(newBuf);
}

void MsgBuffer::addInFrontInt16(const uint16_t s)
{
    uint16_t ss = htons(s);
    addInFront(static_cast<const char*>((void*)&ss), 2);
}

void MsgBuffer::addInFrontInt32(const uint32_t i)
{
    uint32_t ii = htonl(i);
    addInFront(static_cast<const char*>((void*)&ii), 4);
}

void MsgBuffer::addInFrontInt64(const uint64_t l)
{
    uint64_t ll = hton64(l);
    addInFront(static_cast<const char*>((void*)&ll), 8);
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
