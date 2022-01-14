//===-- DWARFASTParserD.cpp -----------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "DWARFASTParserD.h"
#include "DWARFASTParserClang.h"
#include "DWARFASTParser.h"
#include "DWARFDebugInfo.h"
#include "DWARFDeclContext.h"
#include "DWARFDefines.h"
#include "SymbolFileDWARF.h"
#include "SymbolFileDWARFDebugMap.h"
#include "SymbolFileDWARFDwo.h"
#include "UniqueDWARFASTType.h"

#include "Plugins/TypeSystem/D/TypeSystemD.h"

using namespace lldb_private;
using namespace lldb;

DWARFASTParserD::DWARFASTParserD(TypeSystemD &ast)
	: m_ast(ast) {}

TypeSP DWARFASTParserD::ParseTypeFromDWARF(const SymbolContext &sc,
                                        const DWARFDIE &die,
                                        bool *type_is_new_ptr)
{
  if (type_is_new_ptr)
    *type_is_new_ptr = false;

  if (!die)
    return nullptr;

  // TODO: Add logging

  SymbolFileDWARF *dwarf = die.GetDWARF();

  Type *type_ptr = dwarf->GetDIEToType().lookup(die.GetDIE());
  if (type_ptr == DIE_IS_BEING_PARSED)
    return nullptr;

  if (type_ptr)
    return type_ptr->shared_from_this();
  // Set a bit that lets us know that we are currently parsing this
  dwarf->GetDIEToType()[die.GetDIE()] = DIE_IS_BEING_PARSED;

  ParsedDWARFTypeAttributes attrs(die);

  if (DWARFDIE signature_die = attrs.signature.Reference()) {
    if (TypeSP type_sp =
            ParseTypeFromDWARF(sc, signature_die, type_is_new_ptr)) {
      dwarf->GetDIEToType()[die.GetDIE()] = type_sp.get();
      return type_sp;
    }
    return nullptr;
  }

  if (type_is_new_ptr)
    *type_is_new_ptr = true;

  const dw_tag_t tag = die.Tag();

  TypeSP type_sp;
  switch(tag) {
    case DW_TAG_base_type:
      type_sp = ParseSimpleType(sc, die, attrs);
      break;
    case DW_TAG_typedef:
      type_sp = ParseDerivedType(sc, die, attrs);
      break;
    case DW_TAG_pointer_type:
    case DW_TAG_reference_type:
    case DW_TAG_rvalue_reference_type:
    case DW_TAG_const_type:
    case DW_TAG_restrict_type:
    case DW_TAG_volatile_type:
    case DW_TAG_atomic_type:
    case DW_TAG_unspecified_type:
      return type_sp = TypeSP();
      break;

    default: break;
  }

  // TODO: Use Update Symbol function
  return UpdateSymbolContextScopeForType(sc, die, type_sp);
}

lldb::TypeSP
DWARFASTParserD::ParseSimpleType(const lldb_private::SymbolContext &sc,
                               const DWARFDIE &die,
                               ParsedDWARFTypeAttributes &attrs)
{
  SymbolFileDWARF *dwarf = die.GetDWARF();
  CompilerType cp_type;

  cp_type = m_ast.GetBuiltinTypeForDWARFEncodingAndBitSize(
      attrs.name.GetStringRef(),
      attrs.encoding, attrs.byte_size.getValueOr(0) * 8);

  return std::make_shared<Type>(
      die.GetID(), dwarf, attrs.name, attrs.byte_size, nullptr,
      dwarf->GetUID(attrs.type.Reference()), Type::eEncodingIsUID, &attrs.decl,
      cp_type, Type::ResolveState::Full);
}


TypeSP
DWARFASTParserD::ParseTypeFromModule(const lldb_private::SymbolContext &sc,
                                const DWARFDIE &die) {
  return TypeSP();
}

lldb::TypeSP
DWARFASTParserD::ParseDerivedType(const lldb_private::SymbolContext &sc,
                               const DWARFDIE &die,
                               ParsedDWARFTypeAttributes &attrs) {
  SymbolFileDWARF *dwarf = die.GetDWARF();
  const dw_tag_t tag = die.Tag();
  CompilerType cp_type;
  Type::ResolveState resolve_state = Type::ResolveState::Unresolved;

  switch(tag) {
    case DW_TAG_typedef: {
      const DWARFDIE encoding_die = attrs.type.Reference();
      if (encoding_die &&
          encoding_die.GetAttributeValueAsUnsigned(DW_AT_declaration, 0) == 1) {
        TypeSP type_sp = ParseTypeFromModule(sc, die);
        if (type_sp)
          return type_sp;
      }

      break;
    }
    default:
      break;
  }

  return std::make_shared<Type>(
      die.GetID(), dwarf, attrs.name, attrs.byte_size, nullptr,
      dwarf->GetUID(attrs.type.Reference()), GetEncodingFromDWARFTypeTag(tag), &attrs.decl,
      cp_type, resolve_state);
}

Function *
DWARFASTParserD::ParseFunctionFromDWARF(CompileUnit &comp_unit,
                       const DWARFDIE &die,
                       const AddressRange &range)
{
  return nullptr;
}

bool
DWARFASTParserD::CompleteTypeFromDWARF(const DWARFDIE &die, Type *type,
                      lldb_private::CompilerType &compiler_type)
{
  return false;
}

CompilerDecl
DWARFASTParserD::GetDeclForUIDFromDWARF(const DWARFDIE &die)
{
  return CompilerDecl();
}

CompilerDeclContext
DWARFASTParserD::GetDeclContextForUIDFromDWARF(const DWARFDIE &die)
{
  return CompilerDeclContext();
}

CompilerDeclContext
DWARFASTParserD::GetDeclContextContainingUIDFromDWARF(const DWARFDIE &die)
{
  return CompilerDeclContext();
}

void DWARFASTParserD::EnsureAllDIEsInDeclContextHaveBeenParsed(
    lldb_private::CompilerDeclContext decl_context) {}
