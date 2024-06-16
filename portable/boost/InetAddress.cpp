//
// Created by neo on 5/26/24.
//

#include <drogon/portable/portable.hpp>

#include <cstring>
#include <boost/asio.hpp>
#include "drogon/portable/InetAddress.hpp"

namespace drogon
{

namespace asio = boost::asio;

class InetAddressImpl
{
  public:
    // Constructs an InetAddress from a given IP address and port number
    explicit InetAddressImpl(uint16_t port = 0,
                             bool loopbackOnly = false,
                             bool ipv6 = false)
    {
        isUnspecified_ = true;
        ip_ = asio::ip::address_v4::any();
        port_ = port;
    }

    InetAddressImpl(const InetAddressImpl &other)
    {
        ip_ = other.ip_;
        port_ = other.port_;
        isUnspecified_ = other.isUnspecified_;
        isIntranetIp_ = other.isIntranetIp_;
    }

    /**
     * @brief Constructs an endpoint with given ip and port.
     *
     * @param ip A IPv4 or IPv6 address.
     * @param port
     * @param ipv6
     */
    InetAddressImpl(const std::string &ip, uint16_t port, bool ipv6 = false)
    {
        ip_ = asio::ip::make_address(ip);
        port_ = port;
    }

    /**
     * @brief Constructs an endpoint with given struct `sockaddr_in`. Mostly
     * used when accepting new connections
     *
     * @param addr
     */
    explicit InetAddressImpl(const struct sockaddr_in &addr)
        : isUnspecified_(false)
    {
        ip_ = asio::ip::make_address_v4(ntohl(addr.sin_addr.s_addr));
        port_ = ntohs(addr.sin_port);
    }

    // Constructs an InetAddress from a given IP address string in the form
    // "ip:port"
    explicit InetAddressImpl(const std::string &ipPort)
    {
        size_t pos = ipPort.find_last_of(':');
        if (pos != std::string::npos)
        {
            ip_ = asio::ip::make_address(ipPort.substr(0, pos));
            port_ = static_cast<uint16_t>(std::stoul(ipPort.substr(pos + 1)));
        }
        else
        {
            // Throw an exception or set a default value
            ip_ = asio::ip::make_address("0.0.0.0");
            port_ = 0;
        }
    }

    [[nodiscard]] bool isUnspecified() const
    {
        return isUnspecified_;
    }

    [[nodiscard]] bool isIntranetIp() const
    {
        return isIntranetIp_;
    }

    [[nodiscard]] bool isLoopbackIp() const
    {
        return ip_.is_loopback();
    }

    // Returns the IP address as a string
    [[nodiscard]] std::string toIpString() const
    {
        return ip_.to_string();
    }

    // Returns the IP address as an unsigned long value
    [[nodiscard]] uint32_t toIpV4() const
    {
        if (ip_.is_v4())
        {
            return asio::ip::make_address_v4(ip_.to_v4().to_bytes()).to_uint();
        }
        return 0;
    }

    // Returns the port number
    [[nodiscard]] uint16_t toPort() const
    {
        return port_;
    }

    [[nodiscard]] std::string toIp() const
    {
        return ip_.to_string();
    }

    // Returns the IP address and port number in the form "ip:port"
    [[nodiscard]] std::string toIpPort() const
    {
        return ip_.to_string() + ":" + std::to_string(port_);
    }

    [[nodiscard]] std::string toIpNetEndian() const
    {
        return ip_.to_string();
    }

    [[nodiscard]] bool isIpV6() const
    {
        return ip_.is_v6();
    }

    void setPortNetEndian(int port)
    {
        port_ = port;
    }

    [[nodiscard]] int portNetEndian() const
    {
        return port_;
    }

    [[nodiscard]] uint32_t ipNetEndian() const
    {
        return ip_.to_v4().to_ulong();
    }

    [[nodiscard]] const uint32_t *ip6NetEndian() const
    {
        memcpy((void *)addr6_.sin6_addr.s6_addr,
               ip_.to_v6().to_bytes().data(),
               sizeof(addr6_.sin6_addr.s6_addr));
        return addr6_.sin6_addr.s6_addr32;
    }

  private:
    asio::ip::address ip_;
    struct sockaddr_in6 addr6_
    {
    };
    uint16_t port_{};
    bool isUnspecified_{};
    bool isIntranetIp_{};
};

InetAddress::~InetAddress()
{
    delete impl_;
}

InetAddress::InetAddress(uint16_t port, bool loopbackOnly, bool ipv6)
{
    impl_ = new InetAddressImpl(port, loopbackOnly, ipv6);
}
InetAddress::InetAddress(const std::string &ip, uint16_t port, bool ipv6)
{
    impl_ = new InetAddressImpl(ip, port, ipv6);
}
InetAddress::InetAddress(const sockaddr_in &addr)
{
    impl_ = new InetAddressImpl(addr);
}
InetAddress::InetAddress(const std::string &ipPort)
{
    impl_ = new InetAddressImpl(ipPort);
}

InetAddress::InetAddress(const InetAddress &other)
{
    impl_ = new InetAddressImpl(*other.impl_);
}

InetAddress &InetAddress::operator=(const InetAddress &other)
{
    if (this != &other)
    {
        delete impl_;
        impl_ = new InetAddressImpl(*other.impl_);
    }
    return *this;
}

bool InetAddress::isUnspecified() const
{
    return impl_->isUnspecified();
}
bool InetAddress::isIntranetIp() const
{
    return impl_->isIntranetIp();
}
bool InetAddress::isLoopbackIp() const
{
    return impl_->isLoopbackIp();
}
std::string InetAddress::toIpString() const
{
    return impl_->toIpString();
}
uint32_t InetAddress::toIpV4() const
{
    return impl_->toIpV4();
}
uint16_t InetAddress::toPort() const
{
    return impl_->toPort();
}
std::string InetAddress::toIp() const
{
    return impl_->toIp();
}
std::string InetAddress::toIpPort() const
{
    return impl_->toIpPort();
}
std::string InetAddress::toIpNetEndian() const
{
    return impl_->toIpNetEndian();
}
bool InetAddress::isIpV6() const
{
    return impl_->isIpV6();
}
void InetAddress::setPortNetEndian(int port)
{
    impl_->setPortNetEndian(port);
}
int InetAddress::portNetEndian() const
{
    return impl_->portNetEndian();
}
uint32_t InetAddress::ipNetEndian() const
{
    return impl_->ipNetEndian();
}
const uint32_t *InetAddress::ip6NetEndian() const
{
    return impl_->ip6NetEndian();
}

}  // namespace drogon
