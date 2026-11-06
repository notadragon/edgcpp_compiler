//remark:contracts: P3400 the combined P3100 group ub:conv.lval.valid.representation.bool.enum, configured by option or JSON, selects a label group that is or is within it or either of its leaves .bool and .enum, as in GCC and Clang, under both back ends
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=ub:conv.lval.valid.representation.bool.enum:ignore!--contracts_p3400 '--contract_configuration=[{"match":{"group":"ub:conv.lval.valid.representation.bool.enum"},"output":{"semantic":"ignore"}},{"output":{"semantic":"quick_enforce"}}]'
//type:fn
//match_regex:line 46: error: contract predicate is false in constant expression$
//match_regex:line 47: error: contract predicate is false in constant expression$
//match_regex:^2 errors detected
// notadragon_wg21 DECISIONS.md E125 (a): the group the invalid-value check
// reported before its .bool and .enum leaves still selects both, though
// they are not within its name.  The combined group is ignored and
// everything else is checked (quick_enforce, which both back ends have), so
// an assertion that selects the wrong group is an error in constant
// evaluation.  (No headers: both back ends.)
// Mirror: the combined-leaf cases of gcc/testsuite/g++.dg/contracts/cpp26/
// p3400-group-names-string-view-nul.C (gnu_gcc) and
// clang/test/Contracts/p3400-group-names-string-view-nul.cpp
// (llvm_llvm-project).

template <unsigned long L> struct one_t {
  using assertion_control_object = one_t;
  const char *const group_names[1];
  constexpr one_t(const char (&a)[L]) : group_names{ a } {}
};

// The leaves, a group within a leaf, the combined group itself and a group
// within it: ignored.
constexpr one_t bool_label{ "ub:conv.lval.valid.representation.bool" };
constexpr one_t enum_label{ "ub:conv.lval.valid.representation.enum" };
constexpr one_t enum_x_label{ "ub:conv.lval.valid.representation.enum.x" };
constexpr one_t combined_label{
  "ub:conv.lval.valid.representation.bool.enum" };
constexpr one_t combined_x_label{
  "ub:conv.lval.valid.representation.bool.enum.x" };

// The leaves' parent and a name that starts with a leaf but not followed by
// a dot: checked.
constexpr one_t parent_label{ "ub:conv.lval.valid.representation" };
constexpr one_t boolean_label{
  "ub:conv.lval.valid.representation.boolean" };

constexpr bool f_bool() { contract_assert<bool_label>(false); return true; }
constexpr bool f_enum() { contract_assert<enum_label>(false); return true; }
constexpr bool f_enum_x() { contract_assert<enum_x_label>(false); return true; }
constexpr bool f_combined() { contract_assert<combined_label>(false); return true; }
constexpr bool f_combined_x() { contract_assert<combined_x_label>(false); return true; }
constexpr bool f_parent() { contract_assert<parent_label>(false); return true; }
constexpr bool f_boolean() { contract_assert<boolean_label>(false); return true; }

static_assert(f_bool());
static_assert(f_enum());
static_assert(f_enum_x());
static_assert(f_combined());
static_assert(f_combined_x());
static_assert(f_parent());
static_assert(f_boolean());
