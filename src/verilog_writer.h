/*!
@file verilog-ast-walk.h
@brief Contains declarations of functions and structures used to walk the
verilog source tree.
*/

#include "verilog_parser.h"

#ifndef VERILOG_AST_WALK_H
#define VERILOG_AST_WALK_H

NAMESPACE_VERILOG_BEGIN

// Verilog Writer Function
void PrintVerilog( FILE* fout, verilog_source_tree * tree );
 
void PrintExpression( FILE* fout, ast_expression* expression, size_t& strSize );
void PrintPrimary( FILE* fout, ast_primary* primary, size_t& strSize );

NAMESPACE_VERILOG_END

#endif
