//remark:contracts: P4283 in constant evaluation: an instance that does not satisfy a contract assertion's requires-clause does not have it, and its predicate is never scanned; on every route (members, member templates, lambdas, generic lambdas, local classes)
//type:fn
//options_all:--contracts_p4283 --contracts_p3400 --contract_evaluation_semantic=quick_enforce
//match_regex:line 25: error: contract predicate is false in constant expression
//match_regex:line 31: error: contract predicate is false in constant expression
//match_regex:line 43: error: contract predicate is false in constant expression
//match_regex:line 51: error: contract predicate is false in constant expression
//match_regex:line 62: error: contract predicate is false in constant expression
//match_regex:line 64: error: contract predicate is false in constant expression
//match_regex:line 66: error: contract predicate is false in constant expression
//match_regex:line 78: error: contract predicate is false in constant expression
//match_regex:line 83: error: contract predicate is false in constant expression
//match_regex:line 92: error: contract predicate is false in constant expression
// (No system headers: also runs under the C-generating back end.)
template <class T> struct is_int { static constexpr bool value = false; };
template <> struct is_int<int> { static constexpr bool value = true; };
template <class T> concept Int = is_int<T>::value;
template <class T> concept Big = sizeof(T) > 100;
struct Label { using assertion_control_object = Label; };
constexpr Label lbl{};

// Function templates: the bare and parenthesized forms, a conjunction of a
// concept and a requires-expression naming a parameter.
template <class T>
constexpr T f1(T x) pre requires Int<T> (x > 0) { return x; }
static_assert(f1(1.5) == 1.5);   // OK, discarded
constexpr int e1 = f1(-1);       // Error

template <class T>
constexpr T f2(T x)
  pre requires (Int<T> && requires { x + 1; }) (x > 0) { return x; }
static_assert(f2(-1.5) == -1.5); // OK, discarded
constexpr int e2 = f2(-1);       // Error

// A substitution failure makes the constraint not satisfied, and the
// predicate, invalid for that instance, is not scanned; a settled
// conjunction or disjunction does not substitute its second operand.
template <class T>
constexpr int s1(T t) pre requires (T::value > 0) (t < T::value) { return 1; }
template <class T>
constexpr int s2(T t) pre requires (Big<T> && T::value > 0) (t > 0) { return 1; }
template <class T>
constexpr int s3(T t) pre requires (!Big<T> || T::value > 0) (t < 0) { return 1; }
static_assert(s1(1) + s2(1) == 2);   // OK, both discarded
constexpr int e10 = s3(1);           // Error, kept: !Big<int> holds

// contract_assert; a discarded one is an empty statement.
template <class T>
constexpr int m1(T t) {
  contract_assert requires (T::value > 0) (t.missing());
  if (t) contract_assert requires Int<T> (t > 0);
  return 1;
}
static_assert(m1(1.5) == 1);     // OK, both discarded
constexpr int e3 = m1(-1);       // Error

// Members of a class template (cached), naming parameters, and a member
// template; a postcondition with a result name and a label.
template <class T>
struct S {
  constexpr int pre1(T x) const
    pre requires (sizeof(x) <= 4 && requires { x + 1; }) (x > 0) { return 1; }
  template <class U>
  constexpr int pre2(U y) const pre requires Int<U> (y > 0) { return 1; }
  constexpr T post1(T x) const
    post <lbl> requires Int<T> [[maybe_unused]] (r: r > 0) { return x; }
};
static_assert(S<double>().pre1(-1.0) == 1);  // OK, discarded
constexpr int e4 = S<int>().pre1(-1);        // Error
static_assert(S<int>().pre2(-1.0) == 1);     // OK, discarded
constexpr int e5 = S<int>().pre2(-1);        // Error
static_assert(S<double>().post1(-1.0) == -1.0);  // OK, discarded
constexpr int e6 = S<int>().post1(-1);       // Error

// A lambda in a template, and a generic lambda.
template <class T>
constexpr int l1(T t) {
  auto a = [](T x) pre requires Int<T> (x > 0) { return 1; };
  return a(t);
}
static_assert(l1(-1.5) == 1);    // OK, discarded
constexpr int e7 = l1(-1);       // Error
constexpr auto gl = [](auto y) pre requires Int<decltype(y)> (y > 0) {
  return 1;
};
static_assert(gl(-1.5) == 1);    // OK, discarded
constexpr int e8 = gl(-1);       // Error

// A member of a local class in a function template.
template <class T>
constexpr int c1(T t) {
  struct L { constexpr int m(T x) pre requires Int<T> (x > 0) { return 1; } };
  return L().m(t);
}
static_assert(c1(-1.5) == 1);    // OK, discarded
constexpr int e9 = c1(-1);       // Error
