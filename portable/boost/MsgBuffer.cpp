//
// Created by neo on 5/26/24.
//

#include <drogon/portable/portable.hpp>

#include <boost/asio.hpp>
#include "drogon/portable/MsgBuffer.hpp"

namespace asio = boost::asio;

namespace drogon
{

class MsgBufferImpl : protected asio::streambuf
{
  public:
    explicit MsgBufferImpl(size_t len = kBufferDefaultLength)
    {
        reserve(len);
    }

    explicit MsgBufferImpl(MsgBuffer& msg) : asio::streambuf()
    {
        retrieveAll();
        append(msg.peek(), msg.readableBytes());
    }

    void append(const void* data, std::size_t len)
    {
        ensureWritableBytes(len);
        std::copy(static_cast<const char*>(data),
                  static_cast<const char*>(data) + len,
                  beginWrite());
        hasWritten(len);
    }

    void append(const MsgBuffer& buffer)
    {
        append(buffer.peek(), buffer.readableBytes());
    }

    void append(const std::string& str)
    {
        append(str.data(), str.size());
    }

    void ensureWritableBytes(std::size_t len)
    {
        mu_ = prepare(len);
    }

    char* beginWrite()
    {
        return static_cast<char*>(mu_.data());
    }

    [[nodiscard]] const char* peek() const
    {
        return gptr();
    }

    void hasWritten(std::size_t len)
    {
        commit(len);
    }

    void unwrite(int len)
    {
        pbump(-len);
    }

    [[nodiscard]] std::size_t readableBytes() const
    {
        return size();
    }

    [[nodiscard]] std::size_t writableBytes() const
    {
        return mu_.size();
    }

    const char* findCRLF()
    {
        asio::const_buffer buf = data();
        const char* peek = static_cast<const char*>(buf.data());
        auto crlf = std::search(peek, peek + buf.size(), CRLF, CRLF + 2);
        return crlf == peek + buf.size() ? nullptr : crlf;
    }

    void retrieveUntil(const char* end)
    {
        assert(peek() <= end);
        assert(end <= size() + peek());
        retrieve(end - peek());
    }

    void retrieve(size_t len)
    {
        if (len >= readableBytes())
        {
            retrieveAll();
            return;
        }
        consume(len);
    }

    void retrieveAll()
    {
        pubsetbuf(nullptr, 0);
    }

    /**
     * @brief Access a byte in the buffer.
     *
     * @param offset
     * @return const char&
     */
    const char& operator[](size_t offset) const
    {
        assert(readableBytes() >= offset);
        return gptr()[offset];
    }

    char& operator[](size_t offset)
    {
        assert(readableBytes() >= offset);
        return gptr()[offset];
    }

    asio::mutable_buffer mu_;
};

MsgBuffer::MsgBuffer(size_t len)
{
    impl_ = new MsgBufferImpl(len);
}

MsgBuffer::MsgBuffer(MsgBuffer& msg)
{
    impl_ = new MsgBufferImpl(msg);
}

MsgBuffer::~MsgBuffer()
{
    delete impl_;
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
}
void MsgBuffer::retrieve(size_t len)
{
}
void MsgBuffer::retrieveAll()
{
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
