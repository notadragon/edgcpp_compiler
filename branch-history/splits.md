# Segment cuts

Declared interior boundaries for segments that anchors.py cannot divide (see
notadragon_wg21's src/tools/branchhistory/splits.py).  A heading names a
segment key; each `+/regex/` matches the first added line of a new piece.
N cut specs make N+1 pieces, numbered from 0.

## src/statements.c#11.1
contract_assert_statement (1040-p2900-base-parse), with the four lines that
add its check (1100-p2900-base-lowering) in the middle.
The skip of a misplaced capture list (4000-p3098) comes before the predicate.
The note on the assertion-control specifier at the end of the header comment,
its caching after "contract_assert" and its scan before the predicate
(4200-p3400-core).  The facets' message and semantics after the predicate
(4210-p3400-facets).
The requires-clause after the label's caching, and the discard before its scan
(4600-p4283).

- +/^  \(P3400: an assertion-control specifier/
- +/^\*\/$/
- +/^  if \(curr_token == tok_lt\) \{$/
- +/^  if \(curr_token == tok_requires &&$/
- +/^  scan_contract_assertion_attributes\(\);$/
- +/^  if \(curr_token == tok_lbracket\) \{$/
- +/^  if \(required_token\(tok_lparen, ec_exp_lparen\)\) \{$/
- +/^    if \(csp->discarded \|\|$/
- +/^    if \(csp->label_token_cache != NULL\) \{$/
- +/^    csp->predicate = scan_contract_predicate\(&csp->comment, &csp->message\);$/
- +/^    if \(csp->label != NULL\) \{$/
- +/^    \(void\)required_token\(tok_rparen, ec_exp_rparen\);$/
- +/^  if \(contract_checks_wanted\(\) && contract_check_wanted\(csp\)\) \{$/
- +/^  \/\* Check for and ignore the final semicolon\. \*\/$/

## util/edg_gxx.sh#0.0
The header comment of edg_gxx.sh (0500-gxx-driver), with the paragraphs on
the contract options (1140-p2900-base-driver), the configuration options
(2400-p3595), -fcontracts-p3850 (2900-umbrella-option) and -fcontracts-p3290
(3300-p3290) in the middle.
P3098's paragraph (4000-p3098) follows P3099's, and P3400's (4200-p3400-core)
P3290's.
P4283 (4600-p4283) after P3400.
P4298 (4800-p4298) after P4283.
P4299 (5000-p4299) after P4298.
P4301 (5200-p4301) after P4299.
P3100 (6000-p3100-core) after P4301.

- +/^# Contracts\.  The contract options/
- +/^# The configuration options of P3595,/
- +/^# -f\[no-\]contracts-p3850 /
- +/^# -f\[no-\]contracts-p3097 /
- +/^# -f\[no-\]contracts-p3099 /
- +/^# -f\[no-\]contracts-p3098 /
- +/^# -f\[no-\]contracts-p3290 /
- +/^# -f\[no-\]contracts-p3400 /
- +/^# -f\[no-\]contracts-p4283 /
- +/^# -f\[no-\]contracts-p4298 /
- +/^# -f\[no-\]contracts-p4299 /
- +/^# -f\[no-\]contracts-p4301 /
- +/^# -f\[no-\]contracts-p3100 and /
- +/^#$/#5

## util/edg_gxx.sh#0.14
The option loop of edg_gxx.sh (0500-gxx-driver), with the cases of the
contract options (1140-p2900-base-driver), the configuration options
(2400-p3595), -fcontracts-p3850 (2900-umbrella-option) and -fcontracts-p3290
(3300-p3290) in the middle.
P3098's cases (4000-p3098) follow P3099's, and P3400's (4200-p3400-core)
P3290's.
P4283 (4600-p4283) after P3400.
P4298 (4800-p4298) after P4283.
P4299 (5000-p4299) after P4298.
P4301 (5200-p4301) after P4299.
P3100 (6000-p3100-core) after P4301.

- +/^    -fcontracts\)$/
- +/^    -fcontract-configuration=\*\)$/
- +/^    -fcontracts-p3850\)$/
- +/^    -fcontracts-p3097\)$/
- +/^    -fcontracts-p3099\)$/
- +/^    -fcontracts-p3098\)$/
- +/^    -fcontracts-p3290\)$/
- +/^    -fcontracts-p3400\)$/
- +/^    -fcontracts-p4283\)$/
- +/^    -fcontracts-p4298\)$/
- +/^    -fcontracts-p4299\)$/
- +/^    -fcontracts-p4301\)$/
- +/^    -fcontracts-p3100\)$/
- +/^    -E\)$/

## util/edg_gxx.sh#0.24
The end of edg_gxx.sh's front-end-only branch (0500-gxx-driver), then the
probe that passes g++ -fcontract-comment-from-expression
(1140-p2900-base-driver).

- +/^# The generated C\+\+ names the original sources/

## dev_tools/bin/eccp-gxx#0.0
The header comment of eccp-gxx (0520-gxx-test-config), with the lines of its
table for the contract options (1140-p2900-base-driver), the configuration
options (2400-p3595), the umbrella option (2900-umbrella-option) and P3290's
option (3300-p3290) in the middle.
P3098's line (4000-p3098) follows P3099's, and P3400's (4200-p3400-core)
P3290's.
P4283 (4600-p4283) after P3400.
P4298 (4800-p4298) after P4283.
P4299 (5000-p4299) after P4298.
P4301 (5200-p4301) after P4299.
P3100 (6000-p3100-core) after P4301.

- +/^#   --\[no_\]contracts, --contract_evaluation_semantic=X$/
- +/^#   --contract_configuration=X,/
- +/^#   --\[no_\]contracts_p3850 /
- +/^#   --\[no_\]contracts_p3097 /
- +/^#   --\[no_\]contracts_p3099 /
- +/^#   --\[no_\]contracts_p3098 /
- +/^#   --\[no_\]contracts_p3290 /
- +/^#   --\[no_\]contracts_p3400 /
- +/^#   --\[no_\]contracts_p4283 /
- +/^#   --\[no_\]contracts_p4298 /
- +/^#   --\[no_\]contracts_p4299 /
- +/^#   --\[no_\]contracts_p4301 /
- +/^#   --\[no_\]contracts_p3100 /
- +/^#   -D, -U, -I, -l, -L, -Wl,/

## dev_tools/bin/eccp-gxx#0.4
The option loop of eccp-gxx (0520-gxx-test-config), with the cases of the
contract options (1140-p2900-base-driver), the configuration options
(2400-p3595), the umbrella option (2900-umbrella-option) and P3290's option
(3300-p3290) in the middle.
P3098's cases (4000-p3098) follow P3099's, and P3400's (4200-p3400-core)
P3290's.
P4283 (4600-p4283) after P3400.
P4298 (4800-p4298) after P4283.
P4299 (5000-p4299) after P4298.
P4301 (5200-p4301) after P4299.
P3100 (6000-p3100-core) after P4301.

- +/^    --contracts\)$/
- +/^    --contract_configuration=\*\)$/
- +/^    --contracts_p3850\)$/
- +/^    --contracts_p3097\)$/
- +/^    --contracts_p3099\)$/
- +/^    --contracts_p3098\)$/
- +/^    --contracts_p3290\)$/
- +/^    --contracts_p3400\)$/
- +/^    --contracts_p4283\)$/
- +/^    --contracts_p4298\)$/
- +/^    --contracts_p4299\)$/
- +/^    --contracts_p4301\)$/
- +/^    --contracts_p3100\)$/
- +/^    -D\*\|-U\*\|-I\*\|-l\*\|-L\*\|-Wl,\*\)$/

## util/Changes#0.7
The end of the Changes entry of 2400-p3595, and the date line of the entry of
1140-p2900-base-driver that follows it.

- +/^9\/30\/26  \[\]$/

## util/Changes#0.11
The end of the Changes entry of 1140-p2900-base-driver, and the date line of
the entry of 0500-gxx-driver that follows it.

- +/^9\/30\/26  \[\]$/

## src/cmd_line.c#1
The option descriptions of the base options (1000-p2900-base-basic), the
configuration options (2400-p3595), the umbrella option
(2900-umbrella-option) and P3290's option (3300-p3290).
P3098's (4000-p3098) follow P3099's.
P3400's (4200-p3400-core) follow P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100 (6000-p3100-core) last.

- +/^  add_option_description\(optk_contract_configuration,$/
- +/^  add_option_description\(optk_contracts_p3850, "contracts_p3850"/
- +/^  add_option_description\(optk_contracts_p3097, "contracts_p3097"/
- +/^  add_option_description\(optk_contracts_p3099, "contracts_p3099"/
- +/^  add_option_description\(optk_contracts_p3098, "contracts_p3098"/
- +/^  add_option_description\(optk_contracts_p3290, "contracts_p3290"/
- +/^  add_option_description\(optk_contracts_p3400, "contracts_p3400"/
- +/^  add_option_description\(optk_contracts_p4283, "contracts_p4283"/
- +/^  add_option_description\(optk_contracts_p4298, "contracts_p4298"/
- +/^  add_option_description\(optk_contracts_p4299, "contracts_p4299"/
- +/^  add_option_description\(optk_contracts_p4301, "contracts_p4301"/
- +/^  add_option_description\(optk_contracts_p3100, "contracts_p3100"/

## src/cmd_line.c#2
The C-mode errors for the umbrella option (2900-umbrella-option), the base
option (1000-p2900-base-basic) and P3290's option (3300-p3290).
P3098's (4000-p3098) follows P3099's.
P3400's (4200-p3400-core) follows P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100 (6000-p3100-core) last.

- +/^  if \(option_kind_used\[\(int\)optk_contracts\] && contracts_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3097\] && contracts_p3097_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3099\] && contracts_p3099_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3098\] && contracts_p3098_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3290\] && contracts_p3290_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3400\] && contracts_p3400_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p4283\] && contracts_p4283_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p4298\] && contracts_p4298_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p4299\] && contracts_p4299_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p4301\] && contracts_p4301_enabled\) \{$/
- +/^  if \(option_kind_used\[\(int\)optk_contracts_p3100\] && contracts_p3100_enabled\) \{$/

## src/cmd_line.c#4
The option cases of 1000-p2900-base-basic, 2400-p3595, 2900-umbrella-option
and 3300-p3290, in that order.
P3098's (4000-p3098) follow P3099's.
P3400's (4200-p3400-core) follow P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
The noexcept semantics' names (4800-p4298) in the semantic option's parsing.
The noexcept semantics allowed with the C-generating back end (4810-p4298-c-gen)
before its restriction.
P3100 (6000-p3100-core): its semantic's name after the noexcept ones, allowed with the C-generating back end first, and its cases last.

- +/^          \} else if \(strcmp\(semantic_string, "noexcept_observe"\) == 0\) \{$/
- +/^          \} else if \(strcmp\(semantic_string, "assume"\) == 0\) \{$/
- +/^          \} else \{$/
- +/^          if \(contract_evaluation_semantic == ces_assume\) \{$/
- +/^          if \(contract_evaluation_semantic == ces_noexcept_observe \|\|$/
- +/^          if \(contract_evaluation_semantic != ces_ignore &&$/
- +/^      case optk_contract_configuration:$/
- +/^      case optk_contracts_p3850:$/
- +/^      case optk_contracts_p3097:$/
- +/^      case optk_contracts_p3099:$/
- +/^      case optk_contracts_p3098:$/
- +/^      case optk_contracts_p3290:$/
- +/^      case optk_contracts_p3400:$/
- +/^      case optk_contracts_p4283:$/
- +/^      case optk_contracts_p4298:$/
- +/^      case optk_contracts_p4299:$/
- +/^      case optk_contracts_p4301:$/
- +/^      case optk_contracts_p3100:$/

## src/cmd_line.c#8
The default settings of 1000-p2900-base-basic, 2900-umbrella-option and
3300-p3290.
P3098's (4000-p3098) follows P3099's.
P3400's (4200-p3400-core) follows P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100 (6000-p3100-core) last.

- +/^  contracts_p3850_enabled = FALSE;$/
- +/^  contracts_p3097_enabled = FALSE;$/
- +/^  contracts_p3099_enabled = FALSE;$/
- +/^  contracts_p3098_enabled = FALSE;$/
- +/^  contracts_p3290_enabled = FALSE;$/
- +/^  contracts_p3400_enabled = FALSE;$/
- +/^  contracts_p4283_enabled = FALSE;$/
- +/^  contracts_p4298_enabled = FALSE;$/
- +/^  contracts_p4299_enabled = FALSE;$/
- +/^  contracts_p4301_enabled = FALSE;$/
- +/^  contracts_p3100_enabled = FALSE;$/

## src/cmd_line.h#0
The option kinds of 1000-p2900-base-basic, 2400-p3595, 2900-umbrella-option
and 3300-p3290.
P3098's (4000-p3098) follows P3099's.
P3400's (4200-p3400-core) follows P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100 (6000-p3100-core) last.

- +/^  optk_contract_configuration,$/
- +/^  optk_contracts_p3850,$/
- +/^  optk_contracts_p3097,$/
- +/^  optk_contracts_p3099,$/
- +/^  optk_contracts_p3098,$/
- +/^  optk_contracts_p3290,$/
- +/^  optk_contracts_p3400,$/
- +/^  optk_contracts_p4283,$/
- +/^  optk_contracts_p4298,$/
- +/^  optk_contracts_p4299,$/
- +/^  optk_contracts_p4301,$/
- +/^  optk_contracts_p3100,$/

## util/eccp.sh#0
eccp.sh's sorted list of options: those of 2400-p3595 and
1000-p2900-base-basic interleaved, then P3290's (3300-p3290) and the
umbrella's (2900-umbrella-option).
P3098's (4000-p3098) sorts between P3097's and P3099's, P3400's
(4200-p3400-core) between P3290's and the umbrella's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100's (6000-p3100-core) in their sorted places.

- +/^--contract_evaluation_semantic$/
- +/^--contract_group_evaluation_semantic$/
- +/^--contracts$/
- +/^--contracts_allow_assume$/
- +/^--contracts_p3097$/
- +/^--contracts_p3098$/
- +/^--contracts_p3099$/
- +/^--contracts_p3100$/
- +/^--contracts_p3290$/
- +/^--contracts_p3400$/
- +/^--contracts_p3850$/
- +/^--contracts_p4283$/
- +/^--contracts_p4298$/
- +/^--contracts_p4299$/
- +/^--contracts_p4301$/

## util/eccp.sh#1
The negative options of 1000-p2900-base-basic, 3300-p3290 and
2900-umbrella-option in the sorted list.
P3098's (4000-p3098) sorts between P3097's and P3099's, P3400's
(4200-p3400-core) between P3290's and the umbrella's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100's (6000-p3100-core) in their sorted places.

- +/^--no_contracts_allow_assume$/
- +/^--no_contracts_p3097$/
- +/^--no_contracts_p3098$/
- +/^--no_contracts_p3099$/
- +/^--no_contracts_p3100$/
- +/^--no_contracts_p3290$/
- +/^--no_contracts_p3400$/
- +/^--no_contracts_p3850$/
- +/^--no_contracts_p4283$/
- +/^--no_contracts_p4298$/
- +/^--no_contracts_p4299$/
- +/^--no_contracts_p4301$/

## util/eccp.sh#2
The options without a value of 1000-p2900-base-basic, 2900-umbrella-option and
3300-p3290.
P3098's (4000-p3098) follow P3099's, and P3400's (4200-p3400-core) P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4299 (5000-p4299) last.
P4301 (5200-p4301) last.
P3100 (6000-p3100-core) last.

- +/^         --contracts_p3850 \| \\$/
- +/^         --contracts_p3097 \| \\$/
- +/^         --contracts_p3099 \| \\$/
- +/^         --contracts_p3098 \| \\$/
- +/^         --contracts_p3290 \| \\$/
- +/^         --contracts_p3400 \| \\$/
- +/^         --contracts_p4283 \| \\$/
- +/^         --contracts_p4298 \| \\$/
- +/^         --contracts_p4299 \| \\$/
- +/^         --contracts_p4301 \| \\$/
- +/^         --contracts_p3100 \| \\$/

## util/eccp.sh#3
The options with a separate value of 1000-p2900-base-basic and 2400-p3595.

- +/^         --contract_configuration \| \\$/

## util/eccp.sh#4
The options with a joined value of 1000-p2900-base-basic and 2400-p3595.

- +/^          --contract_configuration=\* \| \\$/

## src/cmd_line.c#6
The option implications of 3000-p3097, 3600-p3099 and 3300-p3290.
P3098's (4000-p3098) follows P3099's.
P3400's (4200-p3400-core) follows P3290's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
The C-generating back end's rejection of a noexcept semantic's replacement
(4810-p4298-c-gen) in its fallback.
P4301 (5200-p4301) after the P4298 fallback.
P3100 (6000-p3100-core) last, with the assume fallback.

- +/^  if \(contracts_p3850_enabled &&$/#2
- +/^  if \(contracts_p3850_enabled &&$/#3
- +/^  if \(contracts_p3850_enabled &&$/#4
- +/^  if \(contracts_p3850_enabled &&$/#5
- +/^  if \(contracts_p3850_enabled &&$/#6
- +/^  if \(contracts_p3850_enabled &&$/#7
- +/^#if BACK_END_IS_C_GEN_BE$/
- +/^  \}  \/\* if \*\/$/#15
- +/^  if \(contracts_p3850_enabled &&$/#8
- +/^  if \(contracts_p3850_enabled &&$/#9

## src/interpret.c#32
The interface preconditions of 3000-p3097, then the callee's preconditions
(1090-p2900-base-sema-constexpr).

- +/^      evaluate_function_contracts\(ips, callee, ctk_pre,$/

## src/macro.c#1
The predefinition of __cpp_contracts (1000-p2900-base-basic), with the line
by which P3097 raises its value (3000-p3097) in the middle.
Then P3098's macro (4000-p3098) after P3099's.
Then P3400's macro (4200-p3400-core) after P3098's.
P4283 (4600-p4283) last.
P4298 (4800-p4298) last.
P4301 (5200-p4301) last.

- +/^        if \(contracts_p3097_enabled\) \{$/
- +/^        \(void\)enter_predef_macro\(contracts_value, "__cpp_contracts",$/
- +/^      if \(contracts_enabled && contracts_p3099_enabled\) \{$/
- +/^      if \(contracts_enabled && contracts_p3098_enabled\) \{$/
- +/^      if \(contracts_enabled && contracts_p3400_enabled\) \{$/
- +/^      if \(contracts_enabled && contracts_p4283_enabled\) \{$/
- +/^      if \(contracts_enabled && contracts_p4298_enabled\) \{$/
- +/^      if \(contracts_enabled && contracts_p4301_enabled\) \{$/

## src/il_def.h#7.2
The fields of a contract specifier (1020-p2900-base-il), with the diagnostic
message's (3600-p3099) after the comment's.
Then the captures' fields (4000-p3098) after the message's, and the label's
(4200-p3400-core) after the captures', then the facets' (4210-p3400-facets).
The requires-clause's fields (4600-p4283) after the label's, and its bits after
the facets'.
The noexcept semantics' computed values (4800-p4298) after the label's.
The assume semantic's computed value (6000-p3100-core) after the noexcept ones.

- +/^  a_const_char\t\*message;$/
- +/^  a_variable_ptr$/#3
- +/^  an_expr_node_ptr$/#2
- +/^  an_expr_node_ptr$/#3
- +/^  a_byte\tlabel_allowed_semantics;$/
- +/^  a_byte\tlabel_computed_noexcept_semantics\[2\];$/
- +/^  a_byte\tlabel_computed_assume_semantic;$/
- +/^  a_const_char\t\*label_message;$/
- +/^  a_const_char\t\*label_groups;$/
- +/^  a_bit_field\thas_label_message:1;$/
- +/^  a_bit_field\tlocal_requires_clause:1;$/
- +/^  a_bit_field\tlocal_predicate:1;$/

## src/il_alloc.c#9.1
The initialization of a contract specifier (1020-p2900-base-il), with the
diagnostic message's (3600-p3099) after the comment's.
Then the captures' (4000-p3098) after the message's, and the label's
(4200-p3400-core) after the captures', then the facets' (4210-p3400-facets).
The requires-clause's (4600-p4283) after the label's.
The noexcept semantics' computed values (4800-p4298) after the label's.
The assume semantic's computed value (6000-p3100-core) after the noexcept ones.

- +/^  csp->message = NULL;$/
- +/^  csp->captures = NULL;$/
- +/^  csp->label = NULL;$/
- +/^  csp->requires_constraint = NULL;$/
- +/^  csp->label_allowed_semantics = 0;$/
- +/^  \(void\)memset\(csp->label_computed_noexcept_semantics, 0,$/
- +/^  csp->label_computed_assume_semantic = 0;$/
- +/^  csp->label_message = NULL;$/
- +/^  csp->label_groups = NULL;$/
- +/^  csp->has_label_message = FALSE;$/
- +/^  csp->local_predicate = FALSE;$/

## src/il_display.c#7.1
The display of a contract specifier (1020-p2900-base-il), with the diagnostic
message's (3600-p3099) after the comment's.
Then the captures' (4000-p3098) after the message's, and the label's
(4200-p3400-core) after the captures', then the facets' (4210-p3400-facets).
The requires-clause's (4600-p4283) after the label's.
The noexcept semantics' computed values (4800-p4298) after the facets'.
The assume semantic's computed value (6000-p3100-core) after the noexcept ones.

- +/^  if \(ptr->message != NULL\) \{$/
- +/^  if \(ptr->captures != NULL\) \{$/
- +/^  if \(ptr->label != NULL\) \{$/
- +/^  if \(ptr->requires_constraint != NULL\) \{$/
- +/^  if \(ptr->has_label_message\) \{$/
- +/^  if \(ptr->label_groups != NULL\) \{$/
- +/^  if \(ptr->label_allowed_semantics != 0\) \{$/
- +/^  if \(ptr->label_computed_noexcept_semantics\[0\] != 0\) \{$/
- +/^  if \(ptr->label_computed_assume_semantic != 0\) \{$/
- +/^  if \(ptr->operand_cached\) disp_boolean/

## src/walk_entry.h#3
The walk of a contract specifier (1020-p2900-base-il), with the diagnostic
message's (3600-p3099) after the comment's.
Then the captures' (4000-p3098) after the message's, and the label's
(4200-p3400-core) after the captures', then the facets' (4210-p3400-facets).
The requires-clause's (4600-p4283) after the groups'.

- +/^      walk_string_ptr\(eptr->message, iek_other_text, 0\);$/
- +/^      \/\* The capture variables are on no scope's variables list either;$/
- +/^      \/\* The label \(P3400\); its token cache is for front end use only\. \*\/$/
- +/^      walk_string_ptr\(eptr->label_message, iek_other_text, 0\);$/
- +/^      walk_string_ptr\(eptr->label_groups, iek_other_text, 0\);$/
- +/^      \/\* The requires-clause \(P4283\), possibly through a local expression$/
- +/^      \/\* The token cache pointer is for front end use only\. \*\/$/

## src/declarator.c#8.18
place_contract_predicate (1040-p2900-base-parse), with the copy of the
diagnostic message (3600-p3099) after that of the comment.
The copy of the label (4200-p3400-core) comes first, that of the message
its facet computes (4210-p3400-facets) after that of the message.
The requires-clause's constraint (4600-p4283) after the label.

- +/^  if \(csp->label != NULL && in_file_scope\(csp\) && !in_file_scope\(csp->label\)\) \{$/
- +/^  if \(csp->requires_constraint != NULL && in_file_scope\(csp\) &&$/
- +/^  if \(pred == NULL \|\| !in_file_scope\(csp\) \|\| in_file_scope\(pred\)\) return;$/
- +/^  if \(csp->message != NULL && !in_file_scope\(csp->message\)\) \{$/
- +/^  if \(csp->label_message != NULL && !in_file_scope\(csp->label_message\)\) \{$/
- +/^  if \(csp->label_groups != NULL && !in_file_scope\(csp->label_groups\)\) \{$/
- +/^  if \(!expr_has_reference_to_local_entity\(pred\) &&$/

## src/declarator.c#13.10
match_contract_specifiers (1060-p2900-base-sema-core), with the comparison of
the diagnostic messages (3600-p3099) at the end of its loop.
The comparison of the captures (4000-p3098) comes before that of the
predicates, and that of the labels (4200-p3400-core) between them.
The requires-clause (4600-p4283) after the label.

- +/^    if \(!equiv_postcondition_captures\(prev->captures, curr->captures\)\) \{$/
- +/^    if \(\(prev->label == NULL\) != \(curr->label == NULL\) \|\|$/
- +/^    an_expr_node_ptr  prev_req = prev->requires_constraint;$/
- +/^    an_expr_node_ptr  prev_pred = contract_specifier_predicate\(prev\);$/
- +/^    if \(\(prev->message == NULL\) != \(curr->message == NULL\) \|\|$/
- +/^  \}  \/\* for \*\/$/#2

## src/declarator.c#8.50
scan_function_contract_specifiers (1040-p2900-base-parse), with the scan of a
postcondition's capture list (4000-p3098) after the attributes.
The caching of an assertion-control specifier (4200-p3400-core) comes before
the attributes.
The requires-clause (4600-p4283) after the label.

- +/^    if \(curr_token == tok_lt\) \{$/
- +/^    if \(curr_token == tok_requires &&$/
- +/^    scan_contract_assertion_attributes\(\);$/
- +/^    if \(curr_token == tok_lbracket\) \{$/
- +/^    if \(!required_token\(tok_lparen, ec_exp_lparen\)\) break;$/

## src/il_def.h#6
The variable flags of the result variable of the C-generating checks
(1100-p2900-base-lowering), then of the variables a contract specifier owns
(1070-p2900-base-sema-predicate-scope).

- +/^  a_bit_field$/#2

## src/il_alloc.c#0
The initialization of the variable flags of the result variable of the
C-generating checks (1100-p2900-base-lowering), then of the variables a
contract specifier owns (1070-p2900-base-sema-predicate-scope).

- +/^  vp->is_contract_specifier_var/

## src/il_display.c#0
The display of the variable flags of the result variable of the C-generating
checks (1100-p2900-base-lowering), then of the variables a contract
specifier owns (1070-p2900-base-sema-predicate-scope).

- +/^  if \(ptr->is_contract_specifier_var\) \{$/

## src/declarator.c#8.22
scan_contract_specifier_operand (1040-p2900-base-parse), with the scan of the
label (4200-p3400-core) before that of the captures, and the message the
label's facet computes (4210-p3400-facets) after the predicate.
The requires-clause and the discard (4600-p4283) first.

- +/^  if \(csp->requires_token_cache != NULL &&$/
- +/^  \/\* A lambda in a capture's initializer or in the predicate may name the$/
- +/^  if \(csp->label_token_cache != NULL\) \{$/
- +/^  if \(capture_cache != NULL\) \{$/
- +/^  if \(csp->label != NULL\) apply_contract_label_message_facet\(csp\);$/
- +/^  \(void\)set_postcondition_captures_in_scope\(saved_captures\);$/

## src/declarator.c#8.81
instantiate_contract_specifiers (1040-p2900-base-parse), with the sharing of
the template's label tokens (4200-p3400-core) after the position.
The discard (4600-p4283) before the specifier is made.

- +/^    if \(templ_csp->requires_constraint != NULL \?$/
- +/^    csp = alloc_contract_specifier\(templ_csp->kind\);$/
- +/^    \/\* The label \(P3400\) is scanned with the operand, from the template's$/
- +/^    if \(templ_csp->result_name != NULL && dps\.has_deducible_return_type\) \{$/

## src/declarator.h#1
The declarations of 1040-p2900-base-parse, with those of 4200-p3400-core after
scan_contract_assertion_attributes.

- +/^extern a_boolean curr_token_starts_labeled_contract_specifier\(void\);$/
- +/^extern a_boolean contract_specifiers_are_cached\($/

## src/expr.h#3
The declarations of 1070-p2900-base-sema-predicate-scope, then
scan_contract_label (4200-p3400-core), then those of 4210-p3400-facets.

- +/^extern an_expr_node_ptr scan_contract_label\(void\);$/
- +/^extern void resolve_contract_label_facets\(a_contract_specifier_ptr  csp\);$/
- +/^extern void resolve_contract_label_groups\(a_contract_specifier_ptr  csp,$/

## src/fe_init.c#1
The contract_assert keyword (1000-p2900-base-basic), then the
contract_control keyword (4200-p3400-core), then P4299's
_ContractAssert (5000-p4299).

- +/^    if \(contracts_enabled && contracts_p3400_enabled\) \{$/
- +/^    if \(contracts_enabled && contracts_p4299_enabled\) \{$/

## src/ifc_modules_write.c#0
The IFC token class of contract_assert (1000-p2900-base-basic), then of
contract_control (4200-p3400-core).
Then of P4299's _Pre and _Post (5000-p4299).

- +/^    case tok_contract_control:$/
- +/^    case tok_p4299_pre:$/

## src/lexical.h#0
The operator-name kind of contract_assert (1000-p2900-base-basic), then of
contract_control (4200-p3400-core).
Then of P4299's _Pre and _Post (5000-p4299).

- +/^   onk_none,           \/\* tok_contract_control \*\/$/
- +/^   onk_none,           \/\* tok_p4299_pre \*\/$/

## src/il_def.h#3
The token kind of contract_assert (1000-p2900-base-basic), then of
contract_control (4200-p3400-core).
Then of P4299's _Pre and _Post (5000-p4299).

- +/^  tok_contract_control,$/
- +/^  tok_p4299_pre,$/

## src/il_def.h#4
The token name of contract_assert (1000-p2900-base-basic), then of
contract_control (4200-p3400-core).
Then of P4299's _Pre and _Post (5000-p4299).

- +/^   "contract_control",$/
- +/^   "_Pre",$/

## src/declarator.c#4.8
scan_cached_contract_label (4200-p3400-core), then the resolution of the
label's facets (4210-p3400-facets).

- +/^  \/\* The facets that bear on the semantic the front end chooses\. \*\/$/

## src/statements.c#13
The checks of a definition's contract specifiers
(1070-p2900-base-sema-predicate-scope), with the choice of their semantics
(4210-p3400-facets) at the end.

- +/^    resolve_contract_semantics\(current_routine_entry\(\)->contract_specifiers\);$/
- +/^  \}  \/\* if \*\/$/

## src/interpret.c#13.63
The report of the problems with contract assertions in constant evaluation
(1090-p2900-base-sema-constexpr), with the message a label's facet computes
(4210-p3400-facets) before the diagnostic.

- +/^      \/\* P3400: the message a label computes, if it does\. \*\/$/
- +/^      if \(!cpp->not_constant && message != NULL\) \{$/

## src/contract_config.c#0.109
contract_semantic_for (2400-p3595), with the application of a label's facets
(4210-p3400-facets) before it returns.
The noexcept semantics in constant evaluation (4800-p4298) before it returns.
Then assume as ignore (6000-p3100-core).

- +/^  if \(csp->label != NULL &&$/
- +/^  if \(in_ce\) \{$/
- +/^  if \(semantic == ces_assume\) \{$/
- +/^  return semantic;$/

## src/contract_config.c#0.105
config_entry_matches (2400-p3595), with the match of a label's groups
(4220-p3400-groups) in place of the rejection of every group entry.

- +/^  if \(entry->group != NULL\) \{$/
- +/^  if \(entry->ns != NULL &&$/

## src/expr.c#50.21
resolve_contract_label_facets (4210-p3400-facets), with the reading of the
label's groups (4220-p3400-groups) at the end.
The noexcept semantics' reset and probes (4800-p4298) after the facets'.
The assume semantic's reset and probe (6000-p3100-core) after the noexcept ones.

- +/^  \(void\)memset\(csp->label_computed_noexcept_semantics, 0,$/
- +/^  csp->label_computed_assume_semantic = 0;$/
- +/^  if \(contract_label_constant\(csp, label_con\)\) \{$/
- +/^    if \(contracts_p4298_enabled && csp->label_computed_semantics\[1\] != 0\) \{$/
- +/^    if \(contracts_allow_assume_enabled\) \{$/
- +/^    \/\* The group names \(P3400\)\. \*\/$/
- +/^  \}  \/\* if \*\/$/

## src/expr.c#62
The one-time initialization of the facet probes' caches (4210-p3400-facets),
then of the group names' (4220-p3400-groups).

- +/^  contract_label_group_count_cache = NULL;$/

## src/declarator.c#8.51
The end of the loop of scan_function_contract_specifiers (1040-p2900-base-parse),
with the discard of a specifier (4600-p4283) before it is linked.

- +/^    if \(csp->discarded\) continue;$/
- +/^    \*p_next = csp;$/

## src/declarator.c#8.56
scan_cached_contract_specifiers (1040-p2900-base-parse), with the removal of the
specifiers an instance discards (4600-p4283) after its loop.

- +/^  if \(csps == rp->contract_specifiers\) \{$/
- +/^  ssep->decl_parse_state = NULL;$/

## src/declarator.c#12.7
scan_contract_operands_of_declaration (1070-p2900-base-sema-predicate-scope), with
the removal of the specifiers an instance discards (4600-p4283).

- +/^  \/\* Those an instance discards \(P4283\)\. \*\/$/
- +/^  pop_scope\(\);$/#1

## src/cp_gen_be.c#7.9
gen_contract_specifiers (1020-p2900-base-il), with the requires-clause (4600-p4283)
after the label.

- +/^    gen_contract_requires_clause\(csp\);$/
- +/^    if \(csp->captures == NULL\) \{$/

## src/cp_gen_be.c#13
The contract_assert statement (1020-p2900-base-il), with the requires-clause
(4600-p4283) after the label.

- +/^          gen_contract_requires_clause\(csp\);$/
- +/^          write_tok_ch\('\('\);$/

## src/il.h#1
contract_specifier_predicate (1020-p2900-base-il), then
contract_specifier_requires_constraint (4600-p4283).

- +/^extern an_expr_node_ptr contract_specifier_requires_constraint\($/

## src/expr.c#50.40
apply_contract_label_facets (4210-p3400-facets), with the diversion to the
noexcept semantics' version (4800-p4298) first.
The diversion to the assume semantic's version (6000-p3100-core) before it.

- +/^  if \(contracts_allow_assume_enabled\) \{$/
- +/^  if \(contracts_p4298_enabled\) \{$/
- +/^  if \(csp->label_allowed_semantics != 0\) \{$/

## src/contract_config.c#0.64
supported_semantic (2400-p3595), with the noexcept semantics (4800-p4298) first.
Then assume (6000-p3100-core).

- +/^  if \(contracts_p4298_enabled\) \{$/
- +/^  if \(contracts_allow_assume_enabled && semantic == cs_assume\) \{$/
- +/^  switch \(semantic\) \{$/

## src/contract_config.c#0.67
check_entry_semantic (2400-p3595), without the warning for the noexcept
semantics with P4298 (4800-p4298).
The noexcept semantics allowed with the C-generating back end
(4810-p4298-c-gen).
assume allowed with the C-generating back end (6000-p3100-core) first.
The error's text naming the semantics (6900-cross-paper-options) in the middle
of the restriction.

- +/^    if \(contracts_p4298_enabled\) \{$/
- +/^    if \(p_warned != NULL && !\*p_warned\) \{$/
- +/^      if \(semantic == ces_assume\) \{$/
- +/^      if \(semantic == ces_noexcept_observe \|\|$/
- +/^      if \(semantic != ces_ignore && semantic != ces_quick_enforce\) \{$/
- +/^        config_error\(where, "this configuration supports only the ignore, "$/
- +/^      \}  \/\* if \*\/$/

## src/contract_config.c#0.95
init_contract_config (2400-p3595), with the default entry's noexcept semantics
(4800-p4298).
Then assume (6000-p3100-core).

- +/^  if \(contract_evaluation_semantic == ces_noexcept_observe\) \{$/
- +/^  if \(contract_evaluation_semantic == ces_assume\) \{$/
- +/^  append_config_entry\(entry\);$/

## src/cmd_line.h#1.1
The evaluation semantics (1000-p2900-base-basic), with the noexcept ones
(4800-p4298) last.
Then assume (6000-p3100-core).

- +/^  ces_noexcept_observe,/
- +/^  ces_assume\t\t\/\* P3100/
- +/^\};$/

## src/error_msg.txt#0.76
The requires-clause's messages (4600-p4283), then the noexcept semantics'
(4800-p4298), then the result name of a lambda (1060-p2900-base-sema-core,
EDG-89).

- +/^ec_cl_noexcept_observe_needs_p4298;/
- +/^ec_contract_result_name_shadows_init_capture;/

## src/expr.c#60.15
apply_contract_label_facets_p4298 (4800-p4298), with the noexcept semantics
allowed with the C-generating back end (4810-p4298-c-gen).

- +/^  if \(!in_ce\) \{$/#2
- +/^#endif \/\* BACK_END_IS_C_GEN_BE \*\/$/#1

## src/decls.c#1
The attachment of a declaration's contract specifiers (1040-p2900-base-parse),
with the marking of the violation handler (4810-p4298-c-gen).

- +/^    \/\* For the alias by which the contracts runtime calls it \(P4298, the$/
- +/^  \}  \/\* if \*\/$/#2

## src/statements.c#10.8
contract_violation_call (1100-p2900-base-lowering), with the call of the
noexcept semantics (4810-p4298-c-gen) first.

- +/^  \{ a_contract_evaluation_semantic  semantic =$/
- +/^  check_assertion\(contract_semantic_for\(csp, current_routine_entry\(\),$/

## src/symbol_tbl.c#2
enter_contract_check_routines (1100-p2900-base-lowering), with the noexcept
semantics' routine (4810-p4298-c-gen) last.

- +/^  if \(CONTRACT_CHECKS_IN_FRONT_END && contracts_p4298_enabled\) \{$/
- +/^\}  \/\* enter_contract_check_routines \*\/$/

## util/Changes#0.3
The end of the Changes entry of 4810-p4298-c-gen, and the date line of the
entry of 2400-p3595 that follows it.

- +/^10\/3\/26  \[\]$/

## src/statements.c#14
The checks of a definition's preconditions (1100-p2900-base-lowering), then
the same processing for the compound statement of a function-try-block
(EDG-83): the checks of its contract specifiers
(1070-p2900-base-sema-predicate-scope), the choice of their semantics
(4210-p3400-facets) and the checks of its preconditions
(1100-p2900-base-lowering).

- +/^  if \(is_function_try_block &&$/
- +/^    resolve_contract_semantics\(current_routine_entry\(\)->contract_specifiers\);$/
- +/^    add_precondition_checks\(\);$/#2

## tests/expectations/edg_x86_64_gxx/contracts.log#0.46
The last lines of the gxx expectations, one per commit: a parse test's
(1040-p2900-base-parse) and the sema tests'
(1080-p2900-base-sema-templates', 0160-float-literal-exponent-range's and
0110-typeid-evaluated-operand's).

- +/^sema\/explicit_specialization_in_class/
- +/^sema\/float_literal_exponent_range/
- +/^sema\/typeid_evaluated_operand/

## src/interpret.c#17
The constant evaluation of a contract_assert (1090-p2900-base-sema-constexpr)
around the lookup of its check through the try block of a predicate that
might throw (1100-p2900-base-lowering).

- +/^          if \(check->kind == \(a_statement_kind\)stmk_try_block\) \{$/
- +/^          evaluate_contract_assertion\(ips, csp, check->expr,$/

## src/il_def.h#9
The routine fields of contract assertions: the specifiers
(1020-p2900-base-il) and the P3097 interface wrapper's (3000-p3097).

- +/^  a_routine_ptr\tcontract_interface_wrapper;$/

## src/il_alloc.c#2
The initialization of the routine fields of contract assertions
(1020-p2900-base-il, 3000-p3097).

- +/^  rp->contract_interface_wrapper  = NULL;$/

## src/il_display.c#2
The display of the routine fields of contract assertions
(1020-p2900-base-il, 3000-p3097).

- +/^  if \(ptr->contract_interface_wrapper != NULL\) \{$/

## src/walk_entry.h#0
The walk of the routine fields of contract assertions (1020-p2900-base-il,
3000-p3097).

- +/^        \/\* The interface wrapper of a virtual function, and the function of$/

## src/Changes#0.6
The end of the Changes entry of 6000-p3100-core, and the date line of the entry of
5200-p4301 that follows it.

- +/^10\/5\/26  \[\]$/

## src/Changes#0.9
The end of the Changes entry of 5200-p4301, and the date line of the entry of
5000-p4299 that follows it.

- +/^10\/5\/26  \[\]$/

## src/Changes#0.14
The end of the Changes entry of 5000-p4299, and the date line of the entry of
4810-p4298-c-gen that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.23
The end of the Changes entry of 4810-p4298-c-gen, and the date line of the
entry of 4800-p4298 that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.29
The end of the Changes entry of 4800-p4298, and the date line of the entry of
4600-p4283 that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.39
The end of the Changes entry of 4600-p4283, and the date line of the entry of
4220-p3400-groups that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.45
The end of the Changes entry of 4220-p3400-groups, and the date line of the
entry of 4210-p3400-facets that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.53
The end of the Changes entry of 4210-p3400-facets, and the date line of the
entry of 4200-p3400-core that follows it.

- +/^10\/4\/26  \[\]$/

## src/Changes#0.61
The end of the Changes entry of 4200-p3400-core, and the date line of the entry
of 4000-p3098 that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.75
The end of the Changes entry of 4000-p3098, and the date line of the entry of
3600-p3099 that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.79
The end of the Changes entry of 3600-p3099, and the date line of the entry of
3000-p3097 that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.83
The end of the Changes entry of 3000-p3097, and the date line of the entry of
3300-p3290 that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.90
The end of the Changes entry of 3300-p3290, and the date line of the entry of
2900-umbrella-option that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.93
The end of the Changes entry of 2900-umbrella-option, and the date line of the
entry of 2400-p3595 that follows it.

- +/^10\/3\/26  \[\]$/

## src/Changes#0.101
The end of the Changes entry of 2400-p3595, and the date line of the entry of
1070-p2900-base-sema-predicate-scope that follows it.

- +/^10\/1\/26  \[\]$/

## src/Changes#0.110
The end of the Changes entry of 1070-p2900-base-sema-predicate-scope, and the
date line of the entry of 1060-p2900-base-sema-core that follows it.

- +/^10\/1\/26  \[\]$/

## src/Changes#0.121
The end of the Changes entry of 1060-p2900-base-sema-core, and the date line
of the entry of 1090-p2900-base-sema-constexpr that follows it.

- +/^10\/1\/26  \[\]$/

## src/Changes#0.130
The end of the Changes entry of 1090-p2900-base-sema-constexpr, and the date
line of the entry of 1080-p2900-base-sema-templates that follows it.

- +/^10\/1\/26  \[\]$/

## src/Changes#0.136
The end of the Changes entry of 1080-p2900-base-sema-templates, and the date
line of the member-function entry of 1040-p2900-base-parse that follows it.

- +/^10\/1\/26  \[\]$/

## src/Changes#0.139
The end of the member-function Changes entry of 1040-p2900-base-parse, and
the date line of the entry of 1100-p2900-base-lowering that follows it.

- +/^9\/30\/26  \[\]$/

## src/Changes#0.150
The end of the Changes entry of 1100-p2900-base-lowering, and the date line
of the first entry of 1040-p2900-base-parse that follows it.

- +/^9\/30\/26  \[\]$/

## src/declarator.c#4.1
curr_token_starts_labeled_contract_specifier (4200-p3400-core), with the
demotion of P4299's _Pre and _Post (5000-p4299) first.

- +/^  demote_p4299_contract_keyword\(\);$/
- +/^  return contracts_enabled && contracts_p3400_enabled &&$/

## src/declarator.c#8.2
curr_token_starts_function_contract_specifier (1040-p2900-base-parse), with
the demotion of P4299's _Pre and _Post (5000-p4299) first.

- +/^  demote_p4299_contract_keyword\(\);$/
- +/^  return contracts_enabled &&$/

## src/declarator.c#9.2
cached_token_is_contract_intro (4600-p4283), with P4299's _Pre and _Post
(5000-p4299) first.

- +/^  if \(tok->is\(tok_p4299_pre\) \|\| tok->is\(tok_p4299_post\)\) return TRUE;$/
- +/^  if \(!tok->is\(tok_identifier\) \|\| !tok->is_identifier\(\)\) return FALSE;$/
