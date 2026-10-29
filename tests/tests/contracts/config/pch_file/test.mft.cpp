//remark:contracts: a precompiled header made with a configuration file is used with the same file (EDG-69)
//type:fn
//source_files:header.h config.json
//options_all:--pch --pch_test_mode --contract_evaluation_semantic=quick_enforce --contract_configuration_file=config.json
//match_regex:using precompiled header file
//match_regex:"header.h", line 2: warning: contract predicate is false in constant expression
//match_regex:"header.h", line 4: error: contract predicate is false in constant expression
// The second compilation uses the PCH file the first one created: the
// file's contents are part of what the PCH is checked against (EDG-69), so
// a PCH made with other contents is not used (checked by hand: a test
// cannot change the file between the two compilations).  Nor is one made
// with a file of other text but the same entries (checked by hand, M38-EDG,
// cpfe and cpfe-cp: "creating precompiled header file", then "using" on the
// next compilation); GCC uses its PCH there, as it compares the parsed
// configuration (EDG-129).
#include "header.h"

static_assert(obs::f(0) == 0);    // OK, observed (line 2)
static_assert(g(0) == 0);         // Error, quick_enforce (line 4)
