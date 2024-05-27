//
// Created by neo on 5/24/24.
//

#ifndef DROGON_UTILS_HPP
#define DROGON_UTILS_HPP

#include <drogon/portable/pch.hpp>

namespace drogon::utils
{

struct Hash128
{
    unsigned char bytes[16];
};

struct Hash160
{
    unsigned char bytes[20];
};

struct Hash256
{
    unsigned char bytes[32];
};

struct text
{
    static std::string toHexString(const uint8_t *data, std::size_t dataLen);
};

struct hash
{
    static Hash128 md5(const char *data, size_t dataLen);

    static Hash160 sha1(const char *data, size_t dataLen);

    static Hash160 sha1(const std::string &data);

    static std::string getMd5(const char *data, size_t dataLen);

    static std::string getSha1(const char *data, size_t dataLen);

    //    static std::string getSha256(const char *data, const size_t dataLen)
    //    {
    //    }
    //
    //    static std::string getSha3(const char *data, const size_t dataLen)
    //    {
    //        return "";
    //    }
    //
    //    static std::string getBlake2b(const char *data, const size_t dataLen)
    //    {
    //        return "";
    //    }
};

struct tls
{
    static std::string tlsBackend()
    {
        return "OpenSSL";
    }
};

struct random
{
    static bool secureRandomBytes(void *ptr, size_t size);
};

}  // namespace drogon::utils

#endif  // DROGON_UTILS_HPP
