//
// Created by neo on 5/24/24.
//

#ifndef DROGON_ENCRYPT_HPP
#define DROGON_ENCRYPT_HPP

#include <drogon/portable/Common.hpp>

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

    static std::string toUtf8(const std::wstring &wstr);

    static std::wstring fromUtf8(const std::string &str);

    static std::string fromWidePath(const std::wstring &path);

    static std::wstring toWidePath(const std::string &path);

    static const std::string &toNativePath(const std::string &path);

    static std::string toNativePath(const std::wstring &path);
};

struct hash
{
    static Hash128 md5(const char *data, size_t dataLen);

    static Hash160 sha1(const char *data, size_t dataLen);

    static Hash160 sha1(const std::string &data);

    static std::string getMd5(const char *data, size_t dataLen);

    static std::string getSha1(const char *data, size_t dataLen);

    static std::string getSha256(const char *data, size_t dataLen);

    static std::string getSha3(const char *data, size_t dataLen);
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
