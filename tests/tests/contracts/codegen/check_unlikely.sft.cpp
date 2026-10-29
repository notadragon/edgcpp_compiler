//remark:contracts: the C-generating back end marks the violation arm of each contract check unlikely (__builtin_expect), as GCC and Clang mark their own
//require:DO_IL_LOWERING 1
//type:cp
//options:--contract_evaluation_semantic=quick_enforce --keep_gen_c;cp:--contracts_p4298 --contract_evaluation_semantic=noexcept_observe --keep_gen_c;cp:--contracts_p4298 --contract_evaluation_semantic=noexcept_enforce --keep_gen_c;cp
//filter:tr '\n' ' ' < Test_name.int.c | sed -E 's/ # [0-9]+ / /g' | grep -oE 'if +\(__builtin_expect\( *\( *!' | wc -l
//match_regex:^3$
// notadragon_wg21 DECISIONS.md E63 (a): GCC marks the violation arm of every
// explicit check unlikely (a cold label, 90% on the success arm) and Clang
// gives the branch likely weights.  The front end's checks put out for the
// C compiler become "if (__builtin_expect((!(predicate)), 0)) violation;",
// which gives GCC the same 90%; through edg-gxx the checks are g++'s own.
// --keep_gen_c keeps Test_name.int.c for the filter, which joins its lines
// (the back end breaks long lines with line directives) and counts the
// marked checks: a precondition, a contract_assert, and a precondition whose
// predicate might throw (a check in a try block).
bool chk(int x) { return x > 0; }
int f(int x) pre(x > 0) { return x; }
void g(int x) { contract_assert(x != 4); }
int h(int x) pre(chk(x)) { return x; }
int main() { return f(1) - h(1); }
