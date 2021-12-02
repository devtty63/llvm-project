//===-- DType.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DType.h"

bool DType::IsBasicType() const {
  switch (m_kind) {
  case eDTypeKindBool:
  case eDTypeKindByte:
  case eDTypeKindUByte:
  case eDTypeKindShort:
  case eDTypeKindUShort:
  case eDTypeKindInt:
  case eDTypeKindUInt:
  case eDTypeKindLong:
  case eDTypeKindULong:
  case eDTypeKindCent:
  case eDTypeKindUCent:
  case eDTypeKindChar:
  case eDTypeKindWChar:
  case eDTypeKindDChar:
  case eDTypeKindFloat:
  case eDTypeKindDouble:
  case eDTypeKindReal:
  case eDTypeKindIFloat:
  case eDTypeKindIDouble:
  case eDTypeKindIReal:
  case eDTypeKindCFloat:
  case eDTypeKindCDouble:
  case eDTypeKindCReal:
    return true;
  default: break;
  }
  return false;
}
