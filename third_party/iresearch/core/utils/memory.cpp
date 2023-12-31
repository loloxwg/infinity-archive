////////////////////////////////////////////////////////////////////////////////
/// DISCLAIMER
///
/// Copyright 2016 by EMC Corporation, All Rights Reserved
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
/// Copyright holder is EMC Corporation
///
/// @author Andrey Abramov
/// @author Vasiliy Nabatchikov
////////////////////////////////////////////////////////////////////////////////

#ifndef _MSC_VER
#ifndef DISABLE_EXECINFO
#include <execinfo.h>  // for backtrace(...)
#endif

#include <cstddef>  // needed for __GLIBC__

#if !defined(__APPLE__) && defined(__GLIBC__)
#include <malloc.h>
#endif
#endif

#include "memory.hpp"

namespace irs::memory {

}  // namespace irs::memory
