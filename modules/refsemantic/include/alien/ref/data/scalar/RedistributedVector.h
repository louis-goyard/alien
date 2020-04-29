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

#include <alien/data/IVector.h>
#include <alien/kernels/redistributor/Redistributor.h>
#include <alien/ref/AlienRefSemanticPrecomp.h>
#include <alien/utils/ICopyOnWriteObject.h>

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

class ALIEN_REFSEMANTIC_EXPORT RedistributedVector final : public IVector
{
 public:
  RedistributedVector() = delete;
  RedistributedVector(RedistributedVector&& vector) = delete;
  RedistributedVector(IVector& vector, Redistributor& redist);

  ~RedistributedVector();

  RedistributedVector& operator=(RedistributedVector&& vector) = delete;

  RedistributedVector(const RedistributedVector& vector) = delete;
  RedistributedVector& operator=(const RedistributedVector& vector) = delete;

 public:
  // Pour les expressions
  void visit(ICopyOnWriteVector&) const;

  const VectorDistribution& distribution() const;

  const ISpace& space() const;

  void setUserFeature(String feature);

  bool hasUserFeature(String feature) const;

 public:
  MultiVectorImpl* impl();

  const MultiVectorImpl* impl() const;

 private:
  std::shared_ptr<MultiVectorImpl> m_impl;
};

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
