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

#ifndef SEWENEW_REDISPROTOBUF_APPEND_COMMANDS_H
#define SEWENEW_REDISPROTOBUF_APPEND_COMMANDS_H

#include <string>
#include <string_view>
#include <vector>

#include "field_ref.h"
#include "module_api.h"
#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

// command: PB.APPEND key type path element [element, element...]
// return:  Integer reply: return the length of the array after the append
// operations.
//          Or return the length of the string after the append operations.
// error:   If the path doesn't exist, or the corresponding field is not an
// array, or
//          a string, return an error reply.
class AppendCommand {
 public:
  int run(RedisModuleCtx* ctx, RedisModuleString** argv, int argc) const;

 private:
  struct Args {
    RedisModuleString* key_name;
    Path path;
    std::vector<std::string_view> elements;
  };

  Args _parse_args(RedisModuleString** argv, int argc) const;

  long long _append(MutableFieldRef& field,
                    const std::vector<std::string_view>& elements) const;

  void _append_arr(MutableFieldRef& field, std::string_view val) const;

  long long _append_str(MutableFieldRef& field,
                        const std::vector<std::string_view>& elements) const;

  void _add_msg(MutableFieldRef& field, std::string_view val) const;
};

}  // namespace pb

}  // namespace redis

}  // namespace sw

#endif  // end SEWENEW_REDISPROTOBUF_APPEND_COMMANDS_H
