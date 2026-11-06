//remark:contracts: P3400 group names read as std::string_view are the whole view, NUL bytes included, for the semantic of a check the front end generates: "safety\0x", "safety\0" and "safety\0.x" are checked, "safety.x\0y", a name after one holding a NUL, and a const char * name "safety\0x" are in the ignored safety group; with lib.part configured, "lib.part\0x", "lib.part\0", "lib.part\0.x", "lib\0.part" and "lib.partial\0" are checked and "lib.part.x\0y" is ignored; the combined P3100 group ub:conv.lval.valid.representation.bool.enum ignores "ub:conv.lval.valid.representation.enum.x\0y" but not "ub:conv.lval.valid.representation.bool\0x"
//require:DO_IL_LOWERING 1
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=safety:ignore,lib.part:ignore,ub:conv.lval.valid.representation.bool.enum:ignore
//type:rp
// notadragon_wg21 DECISIONS.md E191 (a), under the C-generating back end,
// which cannot use <string_view>: the front end reads names as whatever
// std::string_view is declared, here a minimal one.  Each checked assertion
// prints its name; an ignored one does not evaluate its predicate.
// run_string_view is the gxx back end's.

extern "C" int printf(const char *, ...);

namespace std {
struct string_view {
  const char *p;
  unsigned long n;
  constexpr string_view(const char *s, unsigned long len) : p(s), n(len) {}
  constexpr string_view(const char *s) : p(s), n(0) { while (s[n] != 0) ++n; }
  constexpr unsigned long size() const { return n; }
  constexpr char operator[](unsigned long i) const { return p[i]; }
};
}

static bool checked(const char *name) {
  printf("%s checked\n", name);
  return true;
}

template <unsigned long L> struct one_t {
  using assertion_control_object = one_t;
  const std::string_view group_names[1];
  constexpr one_t(const char (&a)[L]) : group_names{ { a, L - 1 } } {}
};

constexpr one_t inner_label{ "safety\0x" };
constexpr one_t end_label{ "safety\0" };
constexpr one_t within_label{ "safety.x\0y" };
constexpr one_t before_dot_label{ "safety\0.x" };
constexpr one_t part_inner_label{ "lib.part\0x" };
constexpr one_t part_end_label{ "lib.part\0" };
constexpr one_t part_before_dot_label{ "lib.part\0.x" };
constexpr one_t part_within_label{ "lib.part.x\0y" };
constexpr one_t part_split_label{ "lib\0.part" };
constexpr one_t partial_label{ "lib.partial\0" };
constexpr one_t leaf_inner_label{
  "ub:conv.lval.valid.representation.bool\0x" };
constexpr one_t leaf_within_label{
  "ub:conv.lval.valid.representation.enum.x\0y" };

struct then_t {
  using assertion_control_object = then_t;
  static constexpr std::string_view group_names[2] = {
    { "safety\0x", 8 }, { "safety", 6 } };
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
  printf("done\n");
  return 0;
}
