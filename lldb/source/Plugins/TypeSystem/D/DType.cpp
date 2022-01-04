//===-- DType.cpp ---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DType.h"
#include "lldb/lldb-enumerations.h"
#include "llvm/ADT/Triple.h"

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
  case eDTypeKindReal64:
  case eDTypeKindReal80:
  case eDTypeKindReal128:
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
  case eDTypeKindReal64:
  case eDTypeKindReal80:
  case eDTypeKindReal128:
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
  case eDTypeKindReal64:
  case eDTypeKindReal80:
  case eDTypeKindReal128:
    return lldb::eBasicTypeLongDouble;
  case eDTypeKindCFloat:
    return lldb::eBasicTypeFloatComplex;
  case eDTypeKindCDouble:
    return lldb::eBasicTypeDoubleComplex;
  case eDTypeKindCReal:
  case eDTypeKindCReal64:
  case eDTypeKindCReal80:
  case eDTypeKindCReal128:
    return lldb::eBasicTypeLongDoubleComplex;
  default:
    return lldb::eBasicTypeOther;
  }
}

static uint64_t GetRealBitSize(llvm::Triple &target_triple) {
  if (target_triple.isX86() && !target_triple.isWindowsMSVCEnvironment() && !target_triple.isAndroid())
    // Assume x87 support: x87 FPU register size
    return 80;

  if ((target_triple.isAArch64() && !target_triple.isOSDarwin()) ||
      (target_triple.isAndroid() && target_triple.getArch() == llvm::Triple::x86_64))
    // Assume 128-bit quadruple precision
    return 128;

  // at least size of a double type kind
  return 64;
}

llvm::Optional<uint64_t> DType::GetBitSize(llvm::Triple &target_triple) const {
  return DType::GetBitSize(m_kind, target_triple);
}

llvm::Optional<uint64_t> DType::GetBitSize(DTypeKind kind, llvm::Triple &target_triple) {
  switch(kind)
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
    case eDTypeKindReal64:
      return 64;
    case eDTypeKindReal80:
      return 80;
    case eDTypeKindCent:
    case eDTypeKindUCent:
    case eDTypeKindCDouble:
    case eDTypeKindReal128:
    case eDTypeKindCReal64:
      return 128;
    case eDTypeKindCReal80:
      return 160;
    case eDTypeKindCReal128:
      return 256;
    case eDTypeKindReal:
      return GetRealBitSize(target_triple);
    case eDTypeKindCReal:
      return GetRealBitSize(target_triple) * 2;
    default:
      return llvm::None;
  }
}

ConstString DType::GetName(DTypeKind kind)
{
  switch(kind)
  {
  case eDTypeKindPtr:
    return ConstString("void*");
  case eDTypeKindVoid:
    return ConstString("void");
  case eDTypeKindBool:
    return ConstString("bool");
  case eDTypeKindByte:
    return ConstString("byte");
  case eDTypeKindUByte:
    return ConstString("ubyte");
  case eDTypeKindShort:
    return ConstString("short");
  case eDTypeKindUShort:
    return ConstString("ushort");
  case eDTypeKindInt:
    return ConstString("int");
  case eDTypeKindUInt:
    return ConstString("uint");
  case eDTypeKindLong:
    return ConstString("long");
  case eDTypeKindULong:
    return ConstString("ulong");
  case eDTypeKindCent:
    return ConstString("cent");
  case eDTypeKindUCent:
    return ConstString("ucent");
  case eDTypeKindChar:
    return ConstString("char");
  case eDTypeKindWChar:
    return ConstString("wchar");
  case eDTypeKindDChar:
    return ConstString("dchar");
  case eDTypeKindFloat:
    return ConstString("float");
  case eDTypeKindDouble:
    return ConstString("double");
  case eDTypeKindReal:
  case eDTypeKindReal64:
  case eDTypeKindReal80:
  case eDTypeKindReal128:
    return ConstString("real");
  case eDTypeKindIFloat:
    return ConstString("ifloat");
  case eDTypeKindIDouble:
    return ConstString("idouble");
  case eDTypeKindIReal:
    return ConstString("ireal");
  case eDTypeKindCFloat:
    return ConstString("cfloat");
  case eDTypeKindCDouble:
    return ConstString("cdouble");
  case eDTypeKindCReal:
  case eDTypeKindCReal64:
  case eDTypeKindCReal80:
  case eDTypeKindCReal128:
    return ConstString("creal");
    default:
      return ConstString();
  }
}
