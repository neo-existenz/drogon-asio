/**
 *
 *  SharedLibManager.h
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

#include <drogon/portable/portable.hpp>
#include <unordered_map>
#include <vector>
#include <sys/stat.h>

namespace drogon
{
class SharedLibManager : public NonCopyable
{
  public:
    SharedLibManager(const std::vector<std::string> &libPaths,
                     std::string outputPath);
    ~SharedLibManager();

  private:
    void managerLibs();
    std::vector<std::string> libPaths_;
    std::string outputPath_;
    struct DLStat
    {
        void *handle{nullptr};
        struct timespec mTime = {0, 0};
    };
    std::unordered_map<std::string, DLStat> dlMap_;
    void *compileAndLoadLib(const std::string &sourceFile, void *oldHld);
    void *loadLib(const std::string &soFile, void *oldHld);
    bool shouldCompileLib(const std::string &soFile,
                          const struct stat &sourceStat);
    EventLoop::TimerId timeId_;
    EventLoopThread workingThread_;
};
}  // namespace drogon
