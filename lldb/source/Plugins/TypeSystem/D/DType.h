//===-- DType.h -------------------------------------------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLDB_SOURCE_PLUGINS_TYPESYSTEM_D_DTYPE_H
#define LLDB_SOURCE_PLUGINS_TYPESYSTEM_D_DTYPE_H

#include "lldb/Utility/ConstString.h"
#include "lldb/lldb-enumerations.h"

enum DTypeKind : uint8_t {
  eDTypeKindInvalid = 0,
  eDTypeKindVoid,
  eDTypeKindPtr,
  eDTypeKindSlice,

  eDTypeKindBuiltin = 1 << 7, // 128
  eDTypeKindBool    = eDTypeKindBuiltin,
  eDTypeKindByte,
  eDTypeKindUByte,
  eDTypeKindShort,
  eDTypeKindUShort,
  eDTypeKindInt,
  eDTypeKindUInt,
  eDTypeKindLong,
  eDTypeKindULong,
  eDTypeKindCent,
  eDTypeKindUCent,
  eDTypeKindChar,
  eDTypeKindWChar,
  eDTypeKindDChar,
  eDTypeKindFloat,
  eDTypeKindDouble,
  eDTypeKindReal,
  eDTypeKindIFloat,
  eDTypeKindIDouble,
  eDTypeKindIReal,
  eDTypeKindCFloat,
  eDTypeKindCDouble,
  eDTypeKindCReal,
  eDTypeKindMax,
};

using namespace lldb_private;

class DType {
public:
  DType(DTypeKind kind, const ConstString &name) : m_kind(kind), m_name(name) {}
  virtual ~DType() = default;

  DTypeKind GetKind() { return m_kind; }
  const ConstString &GetName() const { return m_name; }

  bool IsBuiltIn() const;

  lldb::Format GetFormat() const;
  lldb::Encoding GetEncoding(uint64_t &count) const;

private:
  DTypeKind m_kind;
  ConstString m_name;
};

#endif // LLDB_SOURCE_PLUGINS_TYPESYSTEM_D_DTYPE_H
