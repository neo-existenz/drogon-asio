//
// Created by neo on 5/26/24.
//
#include <drogon/portable/portable.hpp>

#include <boost/algorithm/hex.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/detail/sha1.hpp>
#include <boost/uuid/detail/md5.hpp>
#include <boost/uuid/detail/random_provider.hpp>
#include "drogon/portable/Utils.hpp"

std::string drogon::utils::text::toHexString(const uint8_t *data,
                                             const size_t dataLen)
{
    std::string hexStr;
    boost::algorithm::hex(data, data + dataLen, std::back_inserter(hexStr));
    return hexStr;
}

drogon::utils::Hash128 drogon::utils::hash::md5(const char *data,
                                                const size_t dataLen)

{
    boost::uuids::detail::md5 md5;
    boost::uuids::detail::md5::digest_type digest;
    md5.process_bytes(data, dataLen);
    md5.get_digest(digest);
    Hash128 hash = reinterpret_cast<Hash128 &>(digest);
    return hash;
}

drogon::utils::Hash160 drogon::utils::hash::sha1(const char *data,
                                                 const size_t dataLen)
{
    boost::uuids::detail::sha1 sha1;
    boost::uuids::detail::sha1::digest_type digest;
    sha1.process_bytes(data, dataLen);
    sha1.get_digest(digest);
    Hash160 hash = reinterpret_cast<Hash160 &>(digest);
    return hash;
}

drogon::utils::Hash160 drogon::utils::hash::sha1(const std::string &data)
{
    return sha1(data.data(), data.size());
}

std::string drogon::utils::hash::getMd5(const char *data, size_t dataLen)

{
    auto hash = md5(data, dataLen);
    return text::toHexString(hash.bytes, sizeof(hash.bytes));
}

std::string drogon::utils::hash::getSha1(const char *data, size_t dataLen)
{
    auto hash = sha1(data, dataLen);
    return text::toHexString(hash.bytes, sizeof(hash.bytes));
}

bool drogon::utils::random::secureRandomBytes(void *ptr, size_t size)
{
    boost::uuids::detail::random_provider rp;
    rp.get_random_bytes(ptr, size);
    return true;
}
