// A stand-in for the parts of <contracts> that the front end's label facet
// probes and the test use (contract_violation's members as libstdc++
// declares them): the test runs only under the C-generating back end, whose
// eccp finds no system headers in the suite.  The front end probes facets
// through "empty_label | label"; this combined label forwards its right
// operand's (<contracts>' combines both operands').
namespace std {
using size_t = decltype(sizeof 0);
namespace contracts {
enum class evaluation_semantic : int {
  ignore = 1, observe, enforce, quick_enforce, assume, noexcept_observe,
  noexcept_enforce
};
struct evaluation_semantic_set {
  int bits;
  constexpr bool contains(evaluation_semantic s) const {
    return bits & (1 << int(s));
  }
};
class contract_violation {
  const void *chain;
public:
  contract_violation(const contract_violation &) = delete;
  const char *comment() const noexcept;
  const char *message() const noexcept;
  void *query_control_object(const void *key, size_t index = 0) const;
};
namespace labels {
struct empty_label_t { using assertion_control_object = empty_label_t; };
inline constexpr empty_label_t empty_label{};
template <class L, class R> struct combined_label {
  using assertion_control_object = combined_label;
  L lhs;
  R rhs;
  constexpr const char *compute_comment(const char *c) const
    requires requires (const R &r) { r.compute_comment(c); }
  { return rhs.compute_comment(c); }
  constexpr const char *compute_message(const char *m) const
    requires requires (const R &r) { r.compute_message(m); }
  { return rhs.compute_message(m); }
  void handle_contract_violation(const contract_violation &v) const
    requires requires (const R &r) { r.handle_contract_violation(v); }
  { rhs.handle_contract_violation(v); }
  void *query(const void *key, size_t index = 0) const
    requires requires (const R &r) { r.query(key, index); }
  { return rhs.query(key, index); }
};
template <class L, class R>
constexpr combined_label<L, R> operator|(const L &l, const R &r)
  requires requires { typename L::assertion_control_object;
                      typename R::assertion_control_object; }
{ return {l, r}; }
}  // close namespace labels
}  // close namespace contracts
}  // close namespace std
