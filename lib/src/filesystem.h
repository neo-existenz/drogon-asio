/*  @author An Tao
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

#include <drogon/config.h>

#if HAS_STD_FILESYSTEM_PATH
#include <filesystem>
#include <system_error>
#else
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#endif

namespace drogon
{
#if HAS_STD_FILESYSTEM_PATH
namespace filesystem = std::filesystem;
using std::error_code;
#else
namespace filesystem = boost::filesystem;
using boost::system::error_code;
#endif
}  // namespace drogon
