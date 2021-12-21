//===-- DType.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DType.h"
#include "lldb/lldb-enumerations.h"

bool DType::IsBuiltIn() const {
  return m_kind & eDTypeKindBuiltin;
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

lldb::Encoding DType::GetEncoding(uint64_t &count) const {
  count = 1;
  switch (m_kind) {
  case eDTypeKindByte:
  case eDTypeKindShort:
  case eDTypeKindInt:
  case eDTypeKindLong:
  case eDTypeKindCent:
    return lldb::eEncodingSint;
  case eDTypeKindBool:
  case eDTypeKindUByte:
  case eDTypeKindUShort:
  case eDTypeKindUInt:
  case eDTypeKindULong:
  case eDTypeKindUCent:
  case eDTypeKindChar:
  case eDTypeKindWChar:
  case eDTypeKindDChar:
    return lldb::eEncodingUint;
  case eDTypeKindFloat:
  case eDTypeKindDouble:
  case eDTypeKindReal:
    return lldb::eEncodingIEEE754;
  default:
    count = 0;
    return lldb::eEncodingInvalid;
  }
}
