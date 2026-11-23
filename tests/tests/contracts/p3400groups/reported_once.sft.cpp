//remark:contracts: P3400 a label whose group_names cannot be read (a name that is not a constant, more than 4096 names) is diagnosed once, also when its assertion is constant-evaluated more than once, for an assertion statement, a precondition and a prvalue label
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=safety:ignore
//type:fn
//match_regex:line 36: error: the group_names facet of assertion-control object "lab" must be usable in a constant expression$
//match_regex:line 46: error: the group_names facet of assertion-control object "lab" must be usable in a constant expression$
//match_regex:line 56: error: the group_names facet of assertion-control object "lab" must be usable in a constant expression$
//match_regex:line 65: error: the group_names facet of assertion-control object "many_t" holds more than 4096 names$
//match_regex:^4 errors detected
// notadragon_wg21 DECISIONS.md E120, E189 (a), E190.  Each label is on a
// line of its own; the expected errors are the whole list.
// (No system headers: also runs under the C-generating back end.)
// Mirror: gcc/testsuite/g++.dg/contracts/cpp26/
// p3400-group-names-reported-once.C (gnu_gcc).

extern const char *const rn[1];
struct rr {
  constexpr const char *const *begin() const { return rn; }
  constexpr const char *const *end() const { return rn + 1; }
};
struct lab {
  using assertion_control_object = lab;
  static constexpr rr group_names{};
};
constexpr lab l{};

// 4097 names: one more than a label may hold.
struct many_t {
  using assertion_control_object = many_t;
  static constexpr const char *group_names[4097] = { "a" };
};
constexpr many_t many{};

// An assertion statement, constant-evaluated twice.
constexpr int f(int x) {
  contract_assert<
    l
  >(x > 0);
  return 1;
}
static_assert(f(1));
static_assert(f(2));

// A precondition.
constexpr int g(int x)
  pre<
    l
  >(x > 0)
{
  return 1;
}
static_assert(g(1));

// A prvalue label.
constexpr int h(int x) {
  contract_assert<
    lab{}
  >(x > 0);
  return 1;
}
static_assert(h(1));

// Too many names.
constexpr int k(int x) {
  contract_assert<
    many
  >(x > 0);
  return 1;
}
static_assert(k(1));
