//remark:contracts: through edg-gxx each warning about the contract options that g++ also gives is given once (EDG-162, notadragon_wg21 DECISIONS.md E120): when g++ runs, by g++ alone (cases 1-9); when the compilation stops after the front end, by the front end (cases 10-18, the same options); and when the front end stops with an error, so that g++ does not compile, by g++ run on an empty file with the same options, once (EDG-163: cases 19-27, and 29-31 for -Wno-contract-configuration, -w and -Werror), unless the front end has already stopped on a command-line error that g++ shares (case 28); a catastrophic error in the source is treated alike (case 32)
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:--c++23 '--contract_configuration=[]';cp!--c++23 --contract_evaluation_semantic=enforce;cp!--c++26 --contracts --contract_evaluation_semantic=noexcept_observe;cp!--c++26 --contracts --contract_evaluation_semantic=noexcept_enforce;cp!--c++26 --contracts --contract_evaluation_semantic=prove;cp!--c++26 --contracts --contract_evaluation_semantic=diagnose;cp!--c++26 --contracts --contract_analysis_semantic=prove;cp!--c++26 --contracts --contract_analysis_semantic=diagnose;cp!--c++26 --contracts '--contract_configuration=[{"match":{"module":"m"},"output":{"semantic":"enforce"}},{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"noexcept_enforce"}}]';cp!--c++23 '--contract_configuration=[]';fp!--c++23 --contract_evaluation_semantic=enforce;fp!--c++26 --contracts --contract_evaluation_semantic=noexcept_observe;fp!--c++26 --contracts --contract_evaluation_semantic=noexcept_enforce;fp!--c++26 --contracts --contract_evaluation_semantic=prove;fp!--c++26 --contracts --contract_evaluation_semantic=diagnose;fp!--c++26 --contracts --contract_analysis_semantic=prove;fp!--c++26 --contracts --contract_analysis_semantic=diagnose;fp!--c++26 --contracts '--contract_configuration=[{"match":{"module":"m"},"output":{"semantic":"enforce"}},{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"noexcept_enforce"}}]';fp!--c++23 '--contract_configuration=[]';cn!--c++23 --contract_evaluation_semantic=enforce;cn!--c++26 --contracts --contract_evaluation_semantic=noexcept_observe;cn!--c++26 --contracts --contract_evaluation_semantic=noexcept_enforce;cn!--c++26 --contracts --contract_evaluation_semantic=prove;cn!--c++26 --contracts --contract_evaluation_semantic=diagnose;cn!--c++26 --contracts --contract_analysis_semantic=prove;cn!--c++26 --contracts --contract_analysis_semantic=diagnose;cn!--c++26 --contracts '--contract_configuration=[{"match":{"module":"m"},"output":{"semantic":"enforce"}},{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"noexcept_enforce"}}]';cn!--c++26 --contracts_p3400 --no_contracts_p4451 --contract_evaluation_semantic=prove;cc!--c++26 --contracts '--contract_configuration=[{"match":{"module":"m"},"output":{"semantic":"enforce"}},{"match":{"kind":"pre"},"output":{"semantic":"prove"}},{"output":{"semantic":"noexcept_enforce"}}]' -Wno-contract-configuration;cn!--c++26 --contracts --contract_evaluation_semantic=prove -w;cn!--c++26 --contracts --contract_evaluation_semantic=prove -Werror;cn!--c++23 --contract_evaluation_semantic=enforce;cc
//filter:sed -E 's#[^ ]*/edg-gxx\.[^/ ]*/[0-9]+/#EDG_GXX_TMP/#g'
// Cases 1 and 10: the configuration options have no effect without
// contracts.  Cases 2 and 11: the "no effect" warning of the option rule.
// Cases 3-6 and 12-15: an evaluation semantic needs --contracts_p4298 or
// --contracts_p4405; cases 7, 8, 16 and 17: an analysis semantic needs
// --contracts_p4405.  Cases 9 and 18: the configuration warnings
// (-Wcontract-configuration): an unknown key, prove without P4405 and a
// noexcept semantic without P4298.  g++ names the generated file, which the
// filter renames.  Cases 19-27 are cases 1-9 with an error in the source
// (an undeclared name): g++ gives each warning once, on an empty file of the
// generated file's name.  Case 28: a command-line error that g++ also gives;
// the front end stops before any warning and g++ adds nothing.  Cases 29-31:
// g++ is also given -Wno-contract-configuration, -w and -Werror, so cases 29
// and 30 give no warning and case 31 makes it an error.  Case 32: a
// catastrophic error (a missing header) instead of the undeclared name.
#if TEST_NUMBER == 32
#include "report_once_missing_header.h"
#endif
#ifdef __cpp_contracts
int f(int x) pre (x > 0) { return x; }
#else
int f(int x) { return x; }
#endif
int main() { return f(1) - 1; }
#if TEST_NUMBER >= 19
int g() { return undeclared_name; }
#endif
