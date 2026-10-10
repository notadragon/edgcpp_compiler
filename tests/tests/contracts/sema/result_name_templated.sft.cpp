//remark:contracts: a result name on a declaration of a templated function with a deduced return type
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//match_regex:line 16: error: a result name in a postcondition of a function with a deduced return type can appear only on a function definition
// [dcl.contract.res]: a postcondition of a non-defining declaration can
// introduce a result name when the declared return type contains a
// placeholder only if the function is templated (formerly EDG-38).
auto g(auto &p) post(r: r >= 0);                      // OK, g is a template
template <class T> auto g2(T &p) post(r: r >= 0);     // OK
template <class T> decltype(auto) g3(T &p) post(r: r >= 0);  // OK
template <class T> struct S {
  auto h(T p) post(r: r >= 0);                        // OK, templated
  template <class U> auto k(U p) post(r: r >= 0);     // OK
};
auto g(auto &p) post(r: r >= 0);                      // OK, the same
auto ng() post(r: r >= 0);                            // Error
