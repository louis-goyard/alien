/*
 * Copyright 2020 IFPEN-CEA
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#pragma once

#include <ginkgo/ginkgo.hpp>
#include <map>

namespace Alien::Ginkgo
{

struct ginkgo_executor
{
  static std::string target_machine;
  static std::map<std::string, std::function<std::shared_ptr<gko::Executor>()>> exec_map;
};

} // namespace Alien::Ginkgo