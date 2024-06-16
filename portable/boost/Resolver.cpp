//
// Created by neo on 5/26/24.
//
#include <drogon/portable/Portable.hpp>

#include "Resolver.ipp"

namespace drogon
{

Resolver::Resolver(EventLoop *loop, std::size_t size)
{
    impl_ = new ResolverImpl(loop, size);
}

Resolver::Resolver(EventLoop *loop)
{
    impl_ = new ResolverImpl(loop);
}

void Resolver::resolve(const std::string &domain,
                       const Resolver::Callback &callback)
{
    impl_->resolve(domain, callback);
}

}  // namespace drogon
