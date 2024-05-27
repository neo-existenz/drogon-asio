/**
 *
 *  @file HttpServer.h
 *  @author An Tao
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

#include <functional>
#include <string>
#include <vector>
#include <drogon/portable/TcpServer.hpp>
#include <drogon/portable/MsgBuffer.hpp>
#include <drogon/portable/TLSPolicy.hpp>
#include <boost/noncopyable.hpp>
#include "ListenerManager.h"
#include "impl_forwards.h"

struct CallbackParamPack;
namespace drogon
{
class HttpServer : boost::noncopyable
{
  public:
    HttpServer(EventLoop *loop,
               const InetAddress &listenAddr,
               std::string name,
               const std::vector<
                   std::function<HttpResponsePtr(const HttpRequestPtr &)>>
                   &syncAdvices,
               const std::vector<std::function<void(const HttpRequestPtr &,
                                                    const HttpResponsePtr &)>>
                   &preSendingAdvices);

    ~HttpServer();

    [[nodiscard]] EventLoop *getLoop() const
    {
        return server_.getLoop();
    }

    void setHttpAsyncCallback(const HttpAsyncCallback &cb)
    {
        httpAsyncCallback_ = cb;
    }
    void setNewWebsocketCallback(const WebSocketNewAsyncCallback &cb)
    {
        newWebsocketCallback_ = cb;
    }
    void setConnectionCallback(const TcpConnectionCallback &cb)
    {
        connectionCallback_ = cb;
    }
    void setIoLoopThreadPool(
        const std::shared_ptr<EventLoopThreadPool> &pool)
    {
        server_.setIoLoopThreadPool(pool);
    }
    void setIoLoopNum(int numThreads)
    {
        server_.setIoLoopNum(numThreads);
    }
    void setIoLoops(const std::vector<EventLoop *> &ioLoops)
    {
        server_.setIoLoops(ioLoops);
    }
    void kickoffIdleConnections(size_t timeout)
    {
        server_.kickoffIdleConnections(timeout);
    }
    EventLoop *getLoop()
    {
        return server_.getLoop();
    }
    std::vector<EventLoop *> getIoLoops()
    {
        return server_.getIoLoops();
    }
    void start();
    void stop();

    [[deprecated("Use enableSSL(SSLPolicy) instead")]] void enableSSL(
        const std::string &certPath,
        const std::string &keyPath,
        bool useOldTLS,
        const std::vector<std::pair<std::string, std::string>> &sslConfCmds)
    {
        auto policy = TLSPolicy::defaultServerPolicy(certPath, keyPath);
        policy->setConfCmds(sslConfCmds).setUseOldTLS(useOldTLS);
        server_.enableSSL(std::move(policy));
    }

    void enableSSL(TLSPolicyPtr policy)
    {
        server_.enableSSL(std::move(policy));
    }

    [[nodiscard]] const InetAddress &address() const
    {
        return server_.address();
    }

  private:
    void onConnection(const TcpConnectionPtr &conn);
    void onMessage(const TcpConnectionPtr &, MsgBuffer *);
    void onRequests(const TcpConnectionPtr &,
                    const std::vector<HttpRequestImplPtr> &,
                    const std::shared_ptr<HttpRequestParser> &);
    void handleResponse(const HttpResponsePtr &response,
                        const std::shared_ptr<CallbackParamPack> &paramPack,
                        bool *respReadyPtr);
    void sendResponse(const TcpConnectionPtr &,
                      const HttpResponsePtr &,
                      bool isHeadMethod);
    void sendResponses(
        const TcpConnectionPtr &conn,
        const std::vector<std::pair<HttpResponsePtr, bool>> &responses,
        MsgBuffer &buffer);
    TcpServer server_;
    HttpAsyncCallback httpAsyncCallback_;
    WebSocketNewAsyncCallback newWebsocketCallback_;
    TcpConnectionCallback connectionCallback_;
    const std::vector<std::function<HttpResponsePtr(const HttpRequestPtr &)>>
        &syncAdvices_;
    const std::vector<
        std::function<void(const HttpRequestPtr &, const HttpResponsePtr &)>>
        &preSendingAdvices_;
};

}  // namespace drogon
