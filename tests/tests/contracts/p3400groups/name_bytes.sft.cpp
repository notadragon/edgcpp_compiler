//remark:contracts: P3400 a label group name is matched on all its bytes, whichever they are: with the groups "n\nx", "s\001x" and "z0" configured, the label groups "n\nx", "s\001x" and "z0" are selected and "n\001x", "s\nx" and "z\0" are not, under both back ends
//options_sep:!
//options:--contracts_p3400 '--contract_configuration=[{"match":{"group":"n\nx"},"output":{"semantic":"ignore"}},{"match":{"group":"s\u0001x"},"output":{"semantic":"ignore"}},{"match":{"group":"z0"},"output":{"semantic":"ignore"}},{"output":{"semantic":"quick_enforce"}}]'
//type:fn
//match_regex:line 46: error: contract predicate is false in constant expression$
//match_regex:line 47: error: contract predicate is false in constant expression$
//match_regex:line 48: error: contract predicate is false in constant expression$
//match_regex:^3 errors detected
// notadragon_wg21 DECISIONS.md E192 (2): the bytes 0, newline and 1 in a
// label group name are kept apart from each other and from the other bytes
// through matching, as in GCC and Clang (a JSON group may hold a newline or
// the byte 1, not a NUL).  The configured groups are ignored and everything
// else is checked (quick_enforce, which both back ends have), so an
// assertion that selects the wrong group is an error in constant
// evaluation.  (No headers, and a minimal
// std::string_view, so that a name may hold a NUL: both back ends.)

namespace std {
struct string_view {
  const char *p;
  unsigned long n;
  constexpr string_view(const char *s, unsigned long len) : p(s), n(len) {}
  constexpr unsigned long size() const { return n; }
  constexpr char operator[](unsigned long i) const { return p[i]; }
};
}

template <unsigned long L> struct one_t {
  using assertion_control_object = one_t;
  const std::string_view group_names[1];
  constexpr one_t(const char (&a)[L]) : group_names{ { a, L - 1 } } {}
};

// Selected: ignored.
constexpr one_t nl_label{ "n\nx" };
constexpr one_t one_label{ "s\001x" };
constexpr one_t zero_label{ "z0" };
// Not selected: checked.
constexpr one_t nl_one_label{ "n\001x" };
constexpr one_t one_nl_label{ "s\nx" };
constexpr one_t nul_label{ "z\0" };

constexpr bool f_nl() { contract_assert<nl_label>(false); return true; }
constexpr bool f_one() { contract_assert<one_label>(false); return true; }
constexpr bool f_zero() { contract_assert<zero_label>(false); return true; }
constexpr bool f_nl_one() { contract_assert<nl_one_label>(false); return true; }
constexpr bool f_one_nl() { contract_assert<one_nl_label>(false); return true; }
constexpr bool f_nul() { contract_assert<nul_label>(false); return true; }

static_assert(f_nl());
static_assert(f_one());
static_assert(f_zero());
static_assert(f_nl_one());
static_assert(f_one_nl());
static_assert(f_nul());
