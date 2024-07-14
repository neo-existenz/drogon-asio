//
// Created by neo on 6/16/24.
//

#ifndef DROGON_UTILS_HPP
#define DROGON_UTILS_HPP

#include <drogon/portable/utils/Date.hpp>
#include <drogon/portable/utils/Encrypt.hpp>
#include <drogon/portable/utils/Logger.hpp>

namespace drogon {
inline uint64_t hton64(uint64_t n)
{
    static const int one = 1;
    static const char sig = *(char *)&one;
    if (sig == 0)
        return n;  // for big endian machine just return the input
    char *ptr = reinterpret_cast<char *>(&n);
    std::reverse(ptr, ptr + sizeof(uint64_t));
    return n;
}
inline uint64_t ntoh64(uint64_t n)
{
    return hton64(n);
}
}

#endif  // DROGON_UTILS_HPP
