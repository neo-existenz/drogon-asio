/**
 *
 *  @file SessionManager.h
 *  An Tao
 *
 *  Copyright 2018, An Tao.  All rights reserved.
 *  https://github.com/an-tao/drogon
 *  Use of this source code is governed by a MIT license
 *  that can be found in the License file.
 *
 *  Drogon
 *
 */

#pragma once

#include <drogon/Session.h>
#include <drogon/drogon_callbacks.h>
#include <drogon/CacheMap.h>
#include <memory>
#include <string>
#include <mutex>
#include <vector>

namespace drogon
{
class SessionManager : public NonCopyable
{
  public:
    SessionManager(
        EventLoop *loop,
        size_t timeout,
        const std::vector<AdviceStartSessionCallback> &startAdvices,
        const std::vector<AdviceDestroySessionCallback> &destroyAdvices);
    ~SessionManager()
    {
        sessionMapPtr_.reset();
    }
    SessionPtr getSession(const std::string &sessionID, bool needToSet);
    void changeSessionId(const SessionPtr &sessionPtr);

  private:
    std::unique_ptr<CacheMap<std::string, SessionPtr>> sessionMapPtr_;
    EventLoop *loop_;
    size_t timeout_;
    const std::vector<AdviceStartSessionCallback> &sessionStartAdvices_;
    const std::vector<AdviceDestroySessionCallback> &sessionDestroyAdvices_;
};
}  // namespace drogon
