/*
 * Copyright 2020-2021 IFPEN-CEA
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

#include "hypre_matrix.h"
#include "hypre_instance.h"

#include <alien/hypre/backend.h>
#include <alien/core/impl/MultiMatrixImpl.h>
#include <alien/data/ISpace.h>

#include <arccore/message_passing_mpi/MpiMessagePassingMng.h>

#include <HYPRE.h>
// For hypre_*Alloc
#include <_hypre_utilities.h>

namespace Alien::Hypre
{
Matrix::Matrix(const MultiMatrixImpl* multi_impl)
: IMatrixImpl(multi_impl, AlgebraTraits<BackEnd::tag::hypre>::name())
, m_hypre(nullptr)
{
  hypre_init_if_needed();
  const auto& row_space = multi_impl->rowSpace();
  const auto& col_space = multi_impl->colSpace();
  if (row_space.size() != col_space.size())
    throw Arccore::FatalErrorException("Hypre matrix must be square");

  auto* pm = dynamic_cast<Arccore::MessagePassing::Mpi::MpiMessagePassingMng*>(distribution().parallelMng());
  m_comm = pm ? (*pm->getMPIComm()) : MPI_COMM_WORLD;
}

Matrix::~Matrix()
{
  if (m_hypre)
    HYPRE_IJMatrixDestroy(m_hypre);
}

void Matrix::setProfile(int ilower, int iupper,
                        int jlower, int jupper,
                        Arccore::ConstArrayView<int> row_sizes)
{
  if (m_hypre)
    HYPRE_IJMatrixDestroy(m_hypre);

  auto ierr = HYPRE_IJMatrixCreate(m_comm, ilower, iupper, jlower, jupper, &m_hypre);
  ierr |= HYPRE_IJMatrixSetObjectType(m_hypre, HYPRE_PARCSR);
  ierr |= HYPRE_IJMatrixInitialize(m_hypre);
  ierr |= HYPRE_IJMatrixSetRowSizes(m_hypre, row_sizes.data());

  if (ierr) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "Hypre Initialisation failed");
  }
}

void Matrix::assemble()
{
  auto ierr = HYPRE_IJMatrixAssemble(m_hypre);

  if (ierr) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "Hypre assembling failed");
  }
}

void Matrix::setRowValues(int rows, Arccore::ConstArrayView<int> cols, Arccore::ConstArrayView<double> values)
{
  HYPRE_Int col_size = cols.size();
  HYPRE_BigInt h_rows = rows;

  if (col_size != values.size()) {
    throw Arccore::FatalErrorException(A_FUNCINFO, "sizes are not equal");
  }

  const HYPRE_BigInt* ids = nullptr;
  const HYPRE_Real* data = nullptr;
  HYPRE_Int* ncols;
  HYPRE_BigInt* p_rows;

#ifdef ALIEN_HYPRE_CUDA
  HYPRE_MemoryLocation memory_location;
  HYPRE_GetMemoryLocation(&memory_location);
  if (memory_location != HYPRE_MEMORY_HOST) {
    HYPRE_BigInt* d_ids = hypre_CTAlloc(HYPRE_BigInt, cols.size(), memory_location);
    HYPRE_Real* d_values = hypre_CTAlloc(HYPRE_Real, values.size(), memory_location);
    HYPRE_Int* d_ncols = hypre_CTAlloc(HYPRE_Int, 1, memory_location);
    HYPRE_BigInt* d_rows = hypre_CTAlloc(HYPRE_BigInt, 1, memory_location);

    hypre_TMemcpy(d_ids, cols.data(), HYPRE_BigInt, cols.size(), memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_values, values.data(), HYPRE_Real, values.size(), memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_ncols, &col_size, HYPRE_Int, 1, memory_location, HYPRE_MEMORY_HOST);
    hypre_TMemcpy(d_rows, &h_rows, HYPRE_BigInt, 1, memory_location, HYPRE_MEMORY_HOST);

    ids = d_ids;
    data = d_values;
    ncols = d_ncols;
    p_rows = d_rows;
  }
  else
#endif // ALIEN_HYPRE_CUDA
  {
    ids = cols.data();
    data = values.data();
    ncols = &col_size;
    p_rows = &h_rows;
  }

  auto ierr = HYPRE_IJMatrixSetValues(m_hypre, 1, ncols, p_rows, ids, data);

  if (ierr) {
    auto msg = Arccore::String::format("Cannot set Hypre Matrix Values for row {0}", rows);
    throw Arccore::FatalErrorException(A_FUNCINFO, msg);
  }
#ifdef ALIEN_HYPRE_CUDA
  if (memory_location != HYPRE_MEMORY_HOST) {
    hypre_TFree(p_rows, memory_location);
    hypre_TFree(ncols, memory_location);
    hypre_TFree(data, memory_location);
    hypre_TFree(ids, memory_location);
  }
#endif // ALIEN_HYPRE_CUDA
}

} // namespace Alien::Hypre