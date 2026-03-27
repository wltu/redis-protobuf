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

#ifndef SEWENEW_REDISPROTOBUF_PROTO_FACTORY_H
#define SEWENEW_REDISPROTOBUF_PROTO_FACTORY_H

#include <google/protobuf/compiler/importer.h>
#include <google/protobuf/dynamic_message.h>
#include <google/protobuf/message.h>

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <shared_mutex>
#include <string>
#include <string_view>
#include <thread>
#include <unordered_map>
#include <unordered_set>

#include "utils.h"

namespace sw {

namespace redis {

namespace pb {

class FactoryErrorCollector : public gp::compiler::MultiFileErrorCollector {
 public:
  virtual void RecordError(std::string_view filename, int line, int column,
                           std::string_view message) override {
    _add_error("error", filename, line, column, message);
  }

  virtual void RecordWarning(std::string_view filename, int line, int column,
                             std::string_view message) override {
    _add_error("warning", filename, line, column, message);
  }

  std::string last_errors() const;

  bool has_error() const { return !_errors.empty(); }

  void clear() { _errors.clear(); }

 private:
  void _add_error(const std::string& type, std::string_view filename, int line,
                  int column, std::string_view message);

  std::vector<std::string> _errors;
};

class ProtoFactory {
 public:
  explicit ProtoFactory(const std::string& proto_dir);

  ProtoFactory(const ProtoFactory&) = delete;
  ProtoFactory& operator=(const ProtoFactory&) = delete;

  ProtoFactory(ProtoFactory&&) = delete;
  ProtoFactory& operator=(ProtoFactory&&) = delete;

  ~ProtoFactory();

  MsgUPtr create(const std::string& type);

  MsgUPtr create(const std::string& type, std::string_view sv);

  const gp::Descriptor* descriptor(const std::string& type);

  void load(const std::string& file, const std::string& content);

  std::unordered_map<std::string, std::string> last_loaded();

 private:
  void _load_protos(const std::string& proto_dir);

  void _load(const std::string& file);

  void _load(const std::string& filename, const std::string& content);

  std::string _canonicalize_path(std::string proto_dir) const;

  void _async_load();

  void _dump_to_disk(const std::string& filename,
                     const std::string& content) const;

  std::string _absolute_path(const std::string& path) const;

  // Dir where .proto file are saved.
  std::string _proto_dir;

  gp::compiler::DiskSourceTree _source_tree;

  FactoryErrorCollector _error_collector;

  gp::compiler::Importer _importer;

  gp::DynamicMessageFactory _factory;

  std::unordered_map<std::string, const gp::Descriptor*> _descriptor_cache;

  std::unordered_set<std::string> _loaded_files;

  std::mutex _mtx;

  std::condition_variable _cv;

  // map<file, content>
  std::unordered_map<std::string, std::string> _tasks;

  // map<file, load status>
  std::unordered_map<std::string, std::string> _last_loaded_files;

  std::atomic<bool> _stop_loader{false};

  std::thread _async_loader;
};

}  // namespace pb

}  // namespace redis

}  // namespace sw

#endif  // end SEWENEW_REDISPROTOBUF_PROTO_FACTORY_H
