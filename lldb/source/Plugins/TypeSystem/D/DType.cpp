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

lldb::BasicType DType::GetBasicType() const {
  if(!IsBuiltIn())
    return lldb::eBasicTypeOther;

  switch (m_kind) {
  case eDTypeKindVoid:
    return lldb::eBasicTypeVoid;
  case eDTypeKindBool:
    return lldb::eBasicTypeBool;
  case eDTypeKindByte:
    return lldb::eBasicTypeSignedChar;
  case eDTypeKindUByte:
    return lldb::eBasicTypeUnsignedChar;
  case eDTypeKindShort:
    return lldb::eBasicTypeShort;
  case eDTypeKindUShort:
    return lldb::eBasicTypeUnsignedShort;
  case eDTypeKindInt:
    return lldb::eBasicTypeInt;
  case eDTypeKindUInt:
    return lldb::eBasicTypeUnsignedInt;
  case eDTypeKindLong:
    return lldb::eBasicTypeLong;
  case eDTypeKindULong:
    return lldb::eBasicTypeUnsignedLong;
  case eDTypeKindCent:
    return lldb::eBasicTypeInt128;
  case eDTypeKindUCent:
    return lldb::eBasicTypeUnsignedInt128;
  case eDTypeKindChar:
    return lldb::eBasicTypeChar8;
  case eDTypeKindWChar:
    return lldb::eBasicTypeChar16;
  case eDTypeKindDChar:
    return lldb::eBasicTypeChar32;
  case eDTypeKindFloat:
    return lldb::eBasicTypeFloat;
  case eDTypeKindDouble:
    return lldb::eBasicTypeDouble;
  case eDTypeKindReal:
    return lldb::eBasicTypeLongDouble;
  case eDTypeKindCFloat:
    return lldb::eBasicTypeFloatComplex;
  case eDTypeKindCDouble:
    return lldb::eBasicTypeDoubleComplex;
  case eDTypeKindCReal:
    return lldb::eBasicTypeLongDoubleComplex;
  default:
    return lldb::eBasicTypeOther;
  }
}

static uint64_t GetRealBitSize(llvm::Triple &target_triple) {
  if (target_triple.isX86() && target_triple.isArch64Bit())
    // Assume x87 support: x87 FPU register size
    return 80;

  // at least size of a double type kind
  return 64;
}

llvm::Optional<uint64_t> DType::GetBitSize(llvm::Triple &target_triple) const {
  switch(m_kind)
  {
    case eDTypeKindPtr:
      if (target_triple.isArch64Bit())
        return 64;
      if (target_triple.isArch32Bit())
        return 32;
      if (target_triple.isArch16Bit())
        return 16;

      // unknown arch bit size
      return llvm::None;
    case eDTypeKindBool:
    case eDTypeKindByte:
    case eDTypeKindUByte:
    case eDTypeKindChar:
      return 8;
    case eDTypeKindShort:
    case eDTypeKindUShort:
    case eDTypeKindWChar:
      return 16;
    case eDTypeKindInt:
    case eDTypeKindUInt:
    case eDTypeKindDChar:
    case eDTypeKindFloat:
      return 32;
    case eDTypeKindLong:
    case eDTypeKindULong:
    case eDTypeKindDouble:
    case eDTypeKindCFloat:
      return 64;
    case eDTypeKindCent:
    case eDTypeKindUCent:
    case eDTypeKindCDouble:
      return 128;
    case eDTypeKindReal:
      return GetRealBitSize(target_triple);
    case eDTypeKindCReal:
      return GetRealBitSize(target_triple) * 2;
    default:
      return llvm::None;
  }
}
