//
// Created by neo on 5/5/24.
//

#ifndef DROGON_INETADDRESS_HPP
#define DROGON_INETADDRESS_HPP

#include <drogon/portable/pch.hpp>

#include <sys/socket.h>
#include <netinet/in.h>

namespace drogon
{

class InetAddressImpl;

class InetAddress
{
  public:
    virtual ~InetAddress();

    // Constructs an InetAddress from a given IP address and port number
    explicit InetAddress(uint16_t port = 0,
                         bool loopbackOnly = false,
                         bool ipv6 = false);

    /**
     * @brief Constructs an endpoint with given ip and port.
     *
     * @param ip A IPv4 or IPv6 address.
     * @param port
     * @param ipv6
     */
    InetAddress(const std::string &ip, uint16_t port, bool ipv6 = false);

    /**
     * @brief Constructs an endpoint with given struct `sockaddr_in`. Mostly
     * used when accepting new connections
     *
     * @param addr
     */
    explicit InetAddress(const struct sockaddr_in &addr);

    // Constructs an InetAddress from a given IP address string in the form
    // "ip:port"
    explicit InetAddress(const std::string &ipPort);

    InetAddress(const InetAddress &other);

    InetAddress &operator=(const InetAddress &other);

    InetAddress(InetAddress &&other) noexcept
    {
        impl_ = other.impl_;
        other.impl_ = nullptr;
    }

    InetAddress &operator=(InetAddress &&other) noexcept
    {
        if (this != &other)
        {
            impl_ = other.impl_;
            other.impl_ = nullptr;
        }
        return *this;
    }

    [[nodiscard]] bool isUnspecified() const;

    [[nodiscard]] bool isIntranetIp() const;

    [[nodiscard]] bool isLoopbackIp() const;

    // Returns the IP address as a string
    [[nodiscard]] std::string toIpString() const;

    // Returns the IP address as an unsigned long value
    [[nodiscard]] uint32_t toIpV4() const;

    // Returns the port number
    [[nodiscard]] uint16_t toPort() const;

    [[nodiscard]] std::string toIp() const;

    // Returns the IP address and port number in the form "ip:port"
    [[nodiscard]] std::string toIpPort() const;

    [[nodiscard]] std::string toIpNetEndian() const;

    [[nodiscard]] bool isIpV6() const;

    void setPortNetEndian(int port);

    [[nodiscard]] int portNetEndian() const;

    [[nodiscard]] uint32_t ipNetEndian() const;

    [[nodiscard]] const uint32_t *ip6NetEndian() const;

    InetAddressImpl *impl_;
};

}  // namespace drogon

#endif  // DROGON_INETADDRESS_HPP
