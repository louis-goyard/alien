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

#include "BaseParameterManager.h"

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

namespace Alien {

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, int> const&
BaseParameterMng::getParams() const
{
  return m_int_params;
}

/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, int>&
BaseParameterMng::getParams()
{
  return m_int_params;
}

/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, double> const&
BaseParameterMng::getParams() const
{
  return m_double_params;
}

/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, double>&
BaseParameterMng::getParams()
{
  return m_double_params;
}

/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, std::string> const&
BaseParameterMng::getParams() const
{
  return m_string_params;
}

/*---------------------------------------------------------------------------*/

template <>
std::map<std::string, std::string>&
BaseParameterMng::getParams()
{
  return m_string_params;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

} // namespace Alien

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
