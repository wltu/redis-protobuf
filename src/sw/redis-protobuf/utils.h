/**************************************************************************
   Copyright (c) 2019 sewenew

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
 *************************************************************************/

#ifndef SEWENEW_REDISPROTOBUF_UTILS_H
#define SEWENEW_REDISPROTOBUF_UTILS_H

#include <google/protobuf/message.h>

#include <cstring>
#include <memory>
#include <string>
#include <vector>

#include "module_api.h"

namespace sw {

namespace redis {

namespace pb {

namespace gp = google::protobuf;

using MsgUPtr = std::unique_ptr<gp::Message>;

namespace util {

std::string msg_to_json(const gp::Message& msg);

int32_t sv_to_int32(std::string_view sv);

int64_t sv_to_int64(std::string_view sv);

uint32_t sv_to_uint32(std::string_view sv);

uint64_t sv_to_uint64(std::string_view sv);

double sv_to_double(std::string_view sv);

float sv_to_float(std::string_view sv);

bool sv_to_bool(std::string_view sv);

std::string sv_to_string(std::string_view sv);

bool str_case_equal(std::string_view s1, std::string_view s2);
}  // namespace util

namespace io {

bool is_regular(const std::string& file);

bool is_directory(const std::string& file);

std::vector<std::string> list_dir(const std::string& path);

std::string extension(const std::string& file);

void remove_file(const std::string& path);

}  // namespace io

}  // namespace pb

}  // namespace redis

}  // namespace sw

#endif  // end SEWENEW_REDISPROTOBUF_UTILS_H
