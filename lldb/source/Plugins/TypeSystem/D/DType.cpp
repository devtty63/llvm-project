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

lldb::Format DType::GetFormat() const {
  switch (m_kind) {
  case eDTypeKindBool:
    return lldb::eFormatBoolean;
  case eDTypeKindChar:
    return lldb::eFormatUnicode8;
  case eDTypeKindWChar:
    return lldb::eFormatUnicode16;
  case eDTypeKindDChar:
    return lldb::eFormatUnicode32;
  case eDTypeKindUShort:
  case eDTypeKindUInt:
  case eDTypeKindULong:
  case eDTypeKindUCent:
    return lldb::eFormatUnsigned;
  case eDTypeKindShort:
  case eDTypeKindInt:
  case eDTypeKindLong:
  case eDTypeKindCent:
    return lldb::eFormatDecimal;
  case eDTypeKindFloat:
  case eDTypeKindDouble:
  case eDTypeKindReal:
    return lldb::eFormatFloat;
  case eDTypeKindVoid:
    return lldb::eFormatVoid;
  default:
    return lldb::eFormatBytes;
  }
}
