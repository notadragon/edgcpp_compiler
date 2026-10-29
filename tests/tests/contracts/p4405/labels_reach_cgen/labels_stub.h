// A stand-in for the parts of <contracts> that the front end's label facet
// probes use (the evaluation_semantic values as libstdc++ declares them,
// prove 8 and diagnose 9): the C-generating back end's eccp finds no
// system headers in the suite.  The front end probes facets through
// "empty_label | label"; this combined label forwards its right operand's
// allowed_semantics (every label here has one) and compute_semantic.
namespace std {
namespace contracts {
enum class evaluation_semantic : int {
  ignore = 1, observe, enforce, quick_enforce, assume, noexcept_observe,
  noexcept_enforce, prove, diagnose
};
struct evaluation_semantic_set {
  int bits;
  template <class... S>
  constexpr evaluation_semantic_set(S... s) : bits((0 | ... | (1 << int(s))))
  { }
  constexpr bool contains(evaluation_semantic s) const {
    return bits & (1 << int(s));
  }
};
namespace labels {
struct empty_label_t { using assertion_control_object = empty_label_t; };
inline constexpr empty_label_t empty_label{};
template <class L, class R> struct combined_label {
  using assertion_control_object = combined_label;
  static constexpr evaluation_semantic_set allowed_semantics =
    R::allowed_semantics;
  L lhs;
  R rhs;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic s) const
    requires requires (const R &r) { r.compute_semantic(s); }
  { return rhs.compute_semantic(s); }
};
template <class L, class R>
constexpr combined_label<L, R> operator|(const L &l, const R &r)
  requires requires { typename L::assertion_control_object;
                      typename R::assertion_control_object; }
{ return {l, r}; }
}  // close namespace labels
}  // close namespace contracts
}  // close namespace std
