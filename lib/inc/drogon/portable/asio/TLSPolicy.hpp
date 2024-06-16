//
// Created by neo on 5/4/24.
//

#ifndef DROGON_TLSPOLICY_HPP
#define DROGON_TLSPOLICY_HPP

#include <drogon/portable/Common.hpp>

namespace drogon
{

enum class SSLError
{
    kSSLHandshakeError,
    kSSLInvalidCertificate,
    kSSLProtocolError
};

class TLSPolicyImpl;

class TLSPolicy
{
  public:
    using ConfCmds = std::vector<std::pair<std::string, std::string>>;

  public:
    TLSPolicy();
    virtual ~TLSPolicy();

    static std::shared_ptr<TLSPolicy> defaultServerPolicy(
        const std::string& certPath,
        const std::string& keyPath);

    static std::shared_ptr<TLSPolicy> defaultClientPolicy();

    TLSPolicy& setValidate(bool enabled);

    TLSPolicy& setUseSystemCertStore(bool enabled);

    TLSPolicy& setCertPath(const std::string& path);

    TLSPolicy& setKeyPath(const std::string& path);

    TLSPolicy& setHostname(const std::string& hostName);

    TLSPolicy& setConfCmds(const ConfCmds& sslConfCmds);

    [[nodiscard]] ConfCmds sslConfCmds() const;

    TLSPolicy& setUseOldTLS(bool useOldTLS);

  protected:
    TLSPolicyImpl* impl_;
};

using TLSPolicyPtr = std::shared_ptr<TLSPolicy>;

}  // namespace drogon

#endif  // DROGON_TLSPOLICY_HPP
