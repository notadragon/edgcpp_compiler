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

extern a_const_char *add_contract_config_source(
                                       a_contract_config_source_kind  kind,
                                       a_const_char                   *arg);
extern void init_contract_config(void);
extern a_contract_evaluation_semantic contract_semantic_for(
                                        a_contract_specifier_ptr  csp,
                                        a_routine_ptr             routine,
                                        a_boolean                 in_ce);
extern a_boolean contract_semantic_possible(
                                    a_contract_evaluation_semantic  semantic);
/* In expr.c, with the probing of the facets of P3400 labels. */
extern a_contract_evaluation_semantic apply_contract_label_facets(
                              a_contract_specifier_ptr        csp,
                              a_contract_evaluation_semantic  semantic,
                              a_boolean                       in_ce);
/* In statements.c, with the walk for P4405's analysis evaluations. */
extern a_contract_evaluation_semantic contract_analysis_semantic_for(
                                        a_contract_specifier_ptr  csp,
                                        a_routine_ptr             routine);
extern a_contract_evaluation_semantic contract_analysis_stronger(
                                         a_contract_evaluation_semantic  a,
                                         a_contract_evaluation_semantic  b);
extern void diagnose_contract_analyses_at_call(a_routine_ptr      rp,
                                               a_source_position  *pos);
extern void diagnose_contract_address_taken(a_routine_ptr      rp,
                                            a_source_position  *pos);
extern void note_contract_static_destruction(a_dynamic_init_ptr  dip);
extern a_contract_evaluation_semantic apply_contract_label_facets_analysis(
                              a_contract_specifier_ptr        csp,
                              a_contract_evaluation_semantic  semantic,
                              a_contract_evaluation_semantic  *p_analysis,
                              a_boolean                       in_ce);

/* Conditionally close the "edg" namespace. */
END_EDG_NAMESPACE

#endif /* ifndef CONTRACT_CONFIG_H */
