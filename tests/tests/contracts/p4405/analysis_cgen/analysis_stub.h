// A stand-in for the parts of <contracts> that the front end's label facet
// probes use (the evaluation_semantic values as libstdc++ declares them,
// prove 8 and diagnose 9): the C-generating back end's eccp finds no
// system headers in the suite.  The front end probes facets through
// "empty_label | label"; this combined label combines its operands' facets
// as libstdc++'s does (the allowed sets intersected, the compute_semantics
// applied left to right, the stronger analysis_semantic), so the empty
// label, which has none, leaves the label's own.  Like <contracts>, it
// declares D4405's standard labels and makes std::contracts::labels the
// contract_control namespace.
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
template <class T> concept has_allowed = requires { T::allowed_semantics; };
template <class T> concept has_compute =
  requires (const T &t) { t.compute_semantic(evaluation_semantic::ignore); };
template <class T> concept has_analysis =
  requires (const T &t) { t.analysis_semantic(); };
template <class T> constexpr int allowed_bits() {
  if constexpr (has_allowed<T>) return T::allowed_semantics.bits;
  else return -1;
}
template <class L, class R> struct allowed_of { };
template <class L, class R> requires (has_allowed<L> || has_allowed<R>)
struct allowed_of<L, R> {
  static constexpr evaluation_semantic_set allowed_semantics = [] {
    evaluation_semantic_set s;
    s.bits = allowed_bits<L>() & allowed_bits<R>();
    return s;
  }();
};
template <class L, class R> struct combined_label : allowed_of<L, R> {
  using assertion_control_object = combined_label;
  L lhs;
  R rhs;
  constexpr evaluation_semantic compute_semantic(evaluation_semantic s) const
    requires (has_compute<L> || has_compute<R>)
  {
    if constexpr (has_compute<L>) s = lhs.compute_semantic(s);
    if constexpr (has_compute<R>) s = rhs.compute_semantic(s);
    return s;
  }
  constexpr evaluation_semantic analysis_semantic() const
    requires (has_analysis<L> || has_analysis<R>)
  {
    if constexpr (!has_analysis<R>) return lhs.analysis_semantic();
    else if constexpr (!has_analysis<L>) return rhs.analysis_semantic();
    else {
      auto rank = [](evaluation_semantic s) {
        return s == evaluation_semantic::ignore ? 0
             : s == evaluation_semantic::diagnose ? 1
             : s == evaluation_semantic::prove ? 2 : 3;
      };
      evaluation_semantic l = lhs.analysis_semantic();
      evaluation_semantic r = rhs.analysis_semantic();
      return rank(r) > rank(l) ? r : l;
    }
  }
};
#ifdef __cpp_contracts_prove
// D4405's standard labels, as <contracts> declares them: prove and diagnose
// set only the analysis semantic; always_prove and always_diagnose allow
// only that semantic.
struct prove_t {
  using assertion_control_object = prove_t;
  constexpr evaluation_semantic analysis_semantic() const
  { return evaluation_semantic::prove; }
};
inline constexpr prove_t prove{};
struct diagnose_t {
  using assertion_control_object = diagnose_t;
  constexpr evaluation_semantic analysis_semantic() const
  { return evaluation_semantic::diagnose; }
};
inline constexpr diagnose_t diagnose{};
struct always_prove_t {
  using assertion_control_object = always_prove_t;
  static constexpr evaluation_semantic_set allowed_semantics{
    evaluation_semantic::prove};
};
inline constexpr always_prove_t always_prove{};
struct always_diagnose_t {
  using assertion_control_object = always_diagnose_t;
  static constexpr evaluation_semantic_set allowed_semantics{
    evaluation_semantic::diagnose};
};
inline constexpr always_diagnose_t always_diagnose{};
#endif
template <class L, class R>
constexpr combined_label<L, R> operator|(const L &l, const R &r)
  requires requires { typename L::assertion_control_object;
                      typename R::assertion_control_object; }
{ return {{}, l, r}; }
}  // close namespace labels
}  // close namespace contracts
}  // close namespace std
#ifdef __cpp_contracts_labels
using contract_control namespace std::contracts::labels;
#endif
