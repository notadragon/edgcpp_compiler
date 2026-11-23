//remark:contracts: P3400 group names read as std::string_view are the whole view, NUL bytes included, for the semantic of a run-time check: "safety\0x", "safety\0" and "safety\0.x" are checked, "safety.x\0y", a name after one holding a NUL, and a const char * name "safety\0x" are in the ignored safety group; with lib.part configured, "lib.part\0x", "lib.part\0", "lib.part\0.x", "lib\0.part" and "lib.partial\0" are checked and "lib.part.x\0y" is ignored; the combined P3100 group ub:conv.lval.valid.representation.bool.enum ignores "ub:conv.lval.valid.representation.enum.x\0y" but not "ub:conv.lval.valid.representation.bool\0x"
//require:BACK_END_IS_CP_GEN_BE 1
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=safety:ignore,lib.part:ignore,ub:conv.lval.valid.representation.bool.enum:ignore
//type:rp
// notadragon_wg21 DECISIONS.md E191 (a), at run time (g++ applies the
// groups, from the labels put out as written).  Each checked assertion
// prints its name; an ignored one does not evaluate its predicate.
// (<string_view>: runs under the gxx back end only; run_string_view_cgen
// is the C-generating back end's.)
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/
// p3400-group-names-string-view-nul-run.C (gnu_gcc).

#include <cstdio>
#include <string_view>

using namespace std::string_view_literals;

static bool checked(const char *name) {
  std::printf("%s checked\n", name);
  return true;
}

template <const std::string_view &N> struct one_t {
  using assertion_control_object = one_t;
  static constexpr std::string_view group_names[1] = { N };
};

constexpr std::string_view inner = "safety\0x"sv;
constexpr one_t<inner> inner_label{};
constexpr std::string_view ending = "safety\0"sv;
constexpr one_t<ending> end_label{};
constexpr std::string_view within = "safety.x\0y"sv;
constexpr one_t<within> within_label{};
constexpr std::string_view before_dot = "safety\0.x"sv;
constexpr one_t<before_dot> before_dot_label{};
constexpr std::string_view part_inner = "lib.part\0x"sv;
constexpr one_t<part_inner> part_inner_label{};
constexpr std::string_view part_end = "lib.part\0"sv;
constexpr one_t<part_end> part_end_label{};
constexpr std::string_view part_before_dot = "lib.part\0.x"sv;
constexpr one_t<part_before_dot> part_before_dot_label{};
constexpr std::string_view part_within = "lib.part.x\0y"sv;
constexpr one_t<part_within> part_within_label{};
constexpr std::string_view part_split = "lib\0.part"sv;
constexpr one_t<part_split> part_split_label{};
constexpr std::string_view partial = "lib.partial\0"sv;
constexpr one_t<partial> partial_label{};
constexpr std::string_view leaf_inner =
  "ub:conv.lval.valid.representation.bool\0x"sv;
constexpr one_t<leaf_inner> leaf_inner_label{};
constexpr std::string_view leaf_within =
  "ub:conv.lval.valid.representation.enum.x\0y"sv;
constexpr one_t<leaf_within> leaf_within_label{};

struct then_t {
  using assertion_control_object = then_t;
  static constexpr std::string_view group_names[2] = { "safety\0x"sv,
                                                       "safety"sv };
};
constexpr then_t then_label{};

struct pointer_t {
  using assertion_control_object = pointer_t;
  static constexpr const char *group_names[1] = { "safety\0x" };
};
constexpr pointer_t pointer_label{};

int main() {
  contract_assert<inner_label>(checked("inner"));
  contract_assert<end_label>(checked("end"));
  contract_assert<within_label>(checked("within"));
  contract_assert<before_dot_label>(checked("before_dot"));
  contract_assert<then_label>(checked("then"));
  contract_assert<pointer_label>(checked("pointer"));
  contract_assert<part_inner_label>(checked("part_inner"));
  contract_assert<part_end_label>(checked("part_end"));
  contract_assert<part_before_dot_label>(checked("part_before_dot"));
  contract_assert<part_within_label>(checked("part_within"));
  contract_assert<part_split_label>(checked("part_split"));
  contract_assert<partial_label>(checked("partial"));
  contract_assert<leaf_inner_label>(checked("leaf_inner"));
  contract_assert<leaf_within_label>(checked("leaf_within"));
  std::puts("done");
  return 0;
}
