//
// Created by neo on 5/26/24.
//

#include <drogon/portable/Portable.hpp>

#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>

namespace asio = boost::asio;

namespace drogon
{

class TLSPolicyImpl
{
  public:
    TLSPolicyImpl() : context_(asio::ssl::context::sslv23)
    {
        context_.set_options(asio::ssl::context::default_workarounds |
                             asio::ssl::context::no_sslv2 |
                             asio::ssl::context::single_dh_use);
    }

    asio::ssl::context context_;
    TLSPolicy::ConfCmds sslConfCmds_ = {};
    bool useOldTLS_ = false;  // turn into specific version
    bool validate_ = false;
};

TLSPolicy::TLSPolicy()
{
    impl_ = new TLSPolicyImpl();
}

TLSPolicy::~TLSPolicy()
{
    delete impl_;
}

std::shared_ptr<TLSPolicy> TLSPolicy::defaultServerPolicy(
    const std::string& certPath,
    const std::string& keyPath)
{
    auto policy = std::make_shared<TLSPolicy>();
    policy->setValidate(false)
        .setUseOldTLS(false)
        .setUseSystemCertStore(false)
        .setCertPath(certPath)
        .setKeyPath(keyPath);
    return policy;
}

std::shared_ptr<TLSPolicy> TLSPolicy::defaultClientPolicy()
{
    auto policy = std::make_shared<TLSPolicy>();
    policy->setValidate(false).setUseOldTLS(false).setUseSystemCertStore(false);
    return policy;
}

TLSPolicy& TLSPolicy::setValidate(bool enabled)
{
    impl_->validate_ = enabled;
    return *this;
}

TLSPolicy& TLSPolicy::setConfCmds(const TLSPolicy::ConfCmds& sslConfCmds)
{
    impl_->sslConfCmds_ = sslConfCmds;
    return *this;
}

TLSPolicy::ConfCmds TLSPolicy::sslConfCmds() const
{
    return impl_->sslConfCmds_;
}

TLSPolicy& TLSPolicy::setUseOldTLS(bool useOldTLS)
{
    impl_->useOldTLS_ = useOldTLS;
    return *this;
}

TLSPolicy& TLSPolicy::setHostname(const std::string& hostName)
{
    return *this;
}

TLSPolicy& TLSPolicy::setKeyPath(const std::string& path)
{
    return *this;
}

TLSPolicy& TLSPolicy::setUseSystemCertStore(bool enabled)
{
    return *this;
}

TLSPolicy& TLSPolicy::setCertPath(const std::string& path)
{
    return *this;
}

}  // namespace drogon
