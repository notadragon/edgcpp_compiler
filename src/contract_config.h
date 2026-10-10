/*
Part of the EDG Compiler Project, under the Apache License v2.0 with LLVM
Exceptions.
See https://edgcpp.org/LICENSE.txt for license information.
SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
*/

/*

contract_config.h -- Declarations related to contract_config.c (the
                     configuration of contract evaluation semantics, P3595).

*/

/* Avoid including these declarations more than once: */
#ifndef CONTRACT_CONFIG_H
#define CONTRACT_CONFIG_H 1

#ifndef IL_H
#include "il.h"
#endif /* ifndef IL_H */
#ifndef CMD_LINE_H
#include "cmd_line.h"
#endif /* ifndef CMD_LINE_H */

/* Conditionally open the "edg" namespace. */
BEGIN_EDG_NAMESPACE

/*
The kinds of source of contract configuration entries, one for each option
that supplies them.
*/
typedef enum a_contract_config_source_kind {
  ccsk_group_semantic,	/* --contract_group_evaluation_semantic=g:s[,g:s] */
  ccsk_json_inline,	/* --contract_configuration=JSON */
  ccsk_json_file	/* --contract_configuration_file=PATH */
} a_contract_config_source_kind;

extern void add_contract_config_source(a_contract_config_source_kind  kind,
                                       a_const_char                   *arg);
extern void init_contract_config(void);
extern a_contract_evaluation_semantic contract_semantic_for(
                                        a_contract_specifier_ptr  csp,
                                        a_routine_ptr             routine,
                                        a_boolean                 in_ce);
extern a_boolean contract_semantic_possible(
                                    a_contract_evaluation_semantic  semantic);
/* Conditionally close the "edg" namespace. */
END_EDG_NAMESPACE

#endif /* ifndef CONTRACT_CONFIG_H */
