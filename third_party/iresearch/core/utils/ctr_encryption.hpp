////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2019 ArangoDB GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is ArangoDB GmbH, Cologne, Germany
///
/// @author Andrey Abramov
////////////////////////////////////////////////////////////////////////////////

#pragma once

#include "encryption.hpp"

namespace irs {

////////////////////////////////////////////////////////////////////////////////
///// @class cipher
////////////////////////////////////////////////////////////////////////////////
struct cipher {
  virtual ~cipher() = default;

  virtual size_t block_size() const = 0;

  virtual bool encrypt(byte_type* data) const = 0;

  virtual bool decrypt(byte_type* data) const = 0;
};

////////////////////////////////////////////////////////////////////////////////
///// @class ctr_encryption
////////////////////////////////////////////////////////////////////////////////
class ctr_encryption : public encryption {
 public:
  static constexpr size_t DEFAULT_HEADER_LENGTH = 4096;
  static constexpr size_t MIN_HEADER_LENGTH = sizeof(uint64_t);

  explicit ctr_encryption(const cipher& cipher) noexcept : cipher_(&cipher) {}

  size_t header_length() noexcept override { return DEFAULT_HEADER_LENGTH; }

  bool create_header(std::string_view filename, byte_type* header) final;

  stream::ptr create_stream(std::string_view filename, byte_type* header) final;

 private:
  const cipher* cipher_;
};

}  // namespace irs
