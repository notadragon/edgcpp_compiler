//remark: imported from gcc:p3400-group-names-too-many.C
//type: fn
//options_sep: !
//options: --c++26 --contracts --contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=safety:ignore
//match_regex: ", line 109: error: the group_names facet of assertion-control object "range_label<4097>" holds more than 4096 names$
//match_regex: ", line 110: error: the group_names facet of assertion-control object "array_label<4097>" holds more than 4096 names$
//match_regex: ", line 111: error: the group_names facet of assertion-control object "class_label<4097>" holds more than 4096 names$
//match_regex: ^3 errors detected
// EDG: adapted -- no //require: it includes no header, so it also runs
// under the C-generating back end, which needs quick_enforce for enforce
// (the same in constant evaluation), and has no __integer_pack (GNU mode
// only); the errors are the front end's (fn).
// P3400 [support.contract.control.ident] and notadragon_wg21 DECISIONS.md
// E187 (a), E190: a label's group_names may hold at most 4096 names; a
// range that has not ended by then, or an array of more names, is an error
// rather than a label silently in no group.  4096 names are read in full:
// the last of them is "safety", whose group is ignored, so a violated
// precondition is not evaluated during constant evaluation.  The range is
// walked once, so reading 4096 names through a class-type iterator is
// quick, and a 4097-name one stops at the cap before reading any name.  No
// header: the names are read as const char *;
// p3400-group-names-too-many-string-view.C reads them as string_view.
//
// Mirror: clang/test/Contracts/p3400-group-names-too-many.cpp in the
// llvm_llvm-project fork.
// { dg-do compile { target c++26 } }
// { dg-additional-options "-fcontracts -fcontracts-p3400 -fcontract-evaluation-semantic=enforce -fcontract-group-evaluation-semantic=safety:ignore" }
// { dg-timeout 60 }

struct names_t {
  const char *a[4097];
};
constexpr names_t
make ()
{
  names_t r{};
  for (int i = 0; i != 4097; ++i)
    r.a[i] = "a";
  r.a[4095] = "safety";
  return r;
}
constexpr names_t names = make ();

// A range of N names over pointers.
template <int N> struct range {
  constexpr const char *const *begin () const { return names.a; }
  constexpr const char *const *end () const { return names.a + N; }
};

template <int N> struct range_label {
  using assertion_control_object = range_label;
  static constexpr range<N> group_names{};
};
constexpr range_label<4097> range_too_many{};
constexpr range_label<4096> range_most{};

// An array of N names, whose length is its type's.  Past the first, the
// names of array_too_many are null: the cap is diagnosed before any name
// is read.
template <int N> struct array_label {
  using assertion_control_object = array_label;
  static constexpr const char *group_names[N] = { "a" };
};
constexpr array_label<4097> array_too_many{};

// An array of 4096 names, the last "safety".
// (EDG: an integer sequence of its own for __integer_pack, which only GNU
// mode has.)
template <int... I> struct seq {};
template <class A, class B> struct cat;
template <int... I, int... J> struct cat<seq<I...>, seq<J...>>
{ using type = seq<I..., (sizeof... (I) + J)...>; };
template <int N> struct make_seq
{ using type = typename cat<typename make_seq<N / 2>::type,
			    typename make_seq<N - N / 2>::type>::type; };
template <> struct make_seq<0> { using type = seq<>; };
template <> struct make_seq<1> { using type = seq<0>; };
template <class> struct array_most_t;
template <int... I> struct array_most_t<seq<I...>> {
  using assertion_control_object = array_most_t;
  static constexpr const char *group_names[sizeof... (I)]
    = { (I == 4095 ? "safety" : "a")... };
};
template <int N>
using array_most_for = array_most_t<typename make_seq<N>::type>;
constexpr array_most_for<4096> array_most{};

// A range of N names through a class-type iterator, the last "safety".
struct name_iterator {
  int i, last;
  constexpr const char *operator* () const
  { return i == last ? "safety" : "xname"; }
  constexpr name_iterator &operator++ () { ++i; return *this; }
  constexpr bool operator== (const name_iterator &) const = default;
};

template <int N> struct class_range {
  constexpr name_iterator begin () const { return { 0, N - 1 }; }
  constexpr name_iterator end () const { return { N, N - 1 }; }
};

template <int N> struct class_label {
  using assertion_control_object = class_label;
  static constexpr class_range<N> group_names{};
};
constexpr class_label<4097> class_too_many{};
constexpr class_label<4096> class_most{};

void f1 (int x) pre<range_too_many> (x > 0) {}	// { dg-error "'group_names' facet of assertion-control object 'range_label<4097>' holds more than 4096 names" }
void f2 (int x) pre<array_too_many> (x > 0) {}	// { dg-error "'group_names' facet of assertion-control object 'array_label<4097>' holds more than 4096 names" }
void f3 (int x) pre<class_too_many> (x > 0) {}	// { dg-error "'group_names' facet of assertion-control object 'class_label<4097>' holds more than 4096 names" }

constexpr int g1 (int x) pre<range_most> (x > 0) { return x; }
static_assert (g1 (0) == 0);

constexpr int g3 (int x) pre<array_most> (x > 0) { return x; }
static_assert (g3 (0) == 0);

constexpr int g2 (int x) pre<class_most> (x > 0) { return x; }
static_assert (g2 (0) == 0);
