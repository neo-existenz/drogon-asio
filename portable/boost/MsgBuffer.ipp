//
// Created by neo on 6/4/24.
//

#ifndef DROGON_MSGBUFFER_IPP
#define DROGON_MSGBUFFER_IPP

#include <boost/asio.hpp>

namespace drogon
{

namespace asio = boost::asio;

class MsgBufferImpl : public asio::streambuf
{
  public:
    explicit MsgBufferImpl(size_t len = kBufferDefaultLength)
    {
        reserve(len);
    }

    explicit MsgBufferImpl(const MsgBuffer& msg)
    {
        retrieveAll();
        append(msg.peek(), msg.readableBytes());
    }

    MsgBufferImpl(const MsgBufferImpl& msg)
    {
        retrieveAll();
        append(msg.peek(), msg.readableBytes());
    }

    MsgBufferImpl(MsgBufferImpl&& buf) noexcept
    {
        swap(buf);
    }

    MsgBufferImpl& operator=(MsgBufferImpl&& buf) noexcept
    {
        swap(buf);
        return *this;
    }

    MsgBufferImpl& operator=(asio::streambuf && buf) noexcept
    {
        swap(buf);
        return *this;
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

    [[nodiscard]] char* beginWrite() const
    {
        return pptr();
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
        consume(size());
        //        pubsetbuf(nullptr, 0);
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

}  // namespace drogon

#endif  // DROGON_MSGBUFFER_IPP
