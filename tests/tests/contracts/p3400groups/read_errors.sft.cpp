//remark:contracts: P3400 a label's group_names that is a range of names but cannot be read is an error, not a label silently in no group: a name that is not a constant expression, more than 4096 names; names are read in full, however long, and with every byte value, a name after a 0xFF name is still read, a newline inside a name does not split it, and a name returned by value is read before its temporary is destroyed
//options_sep:!
//options:--contracts_p3400 --contract_evaluation_semantic=quick_enforce --contract_group_evaluation_semantic=safety:ignore --contract_group_evaluation_semantic=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx:ignore --contract_group_evaluation_semantic=xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxzz:ignore
//type:fn
//match_regex:line 41: error: the group_names facet of assertion-control object "n_t<4097>" holds more than 4096 names$
//match_regex:line 54: error: the group_names facet of assertion-control object "runtime_t" must be usable in a constant expression$
//match_regex:line 78: error: contract predicate is false in constant expression$
//match_regex:line 87: error: contract predicate is false in constant expression$
//match_regex:line 125: error: contract predicate is false in constant expression$
//match_regex:^5 errors detected
// (No system headers: also runs under the C-generating back end.)
// Mirror (the errors): gcc/testsuite/g++.dg/contracts/cpp26/
// p3400-group-names-not-constant.C (gnu_gcc).
// Mirror (names read in full): gcc/testsuite/g++.dg/contracts/cpp26/
// p3400-group-names-read-in-full.C (gnu_gcc).

struct name_iterator {
  int i, last;
  constexpr const char *operator*() const { return i == last ? "safety" : "x"; }
  constexpr name_iterator &operator++() { ++i; return *this; }
  constexpr bool operator==(const name_iterator &) const = default;
};

template <int N> struct n_names {
  constexpr name_iterator begin() const { return { 0, N - 1 }; }
  constexpr name_iterator end() const { return { N, N - 1 }; }
};

template <int N> struct n_t {
  using assertion_control_object = n_t;
  static constexpr n_names<N> group_names{};
};

// 4096 names, the last "safety": read in full, so ignored.
constexpr n_t<4096> names_4096{};
constexpr bool f4096() { contract_assert<names_4096>(false); return true; }
static_assert(f4096());

// 4097 names: more than a group_names may hold.
constexpr n_t<4097> names_4097{};
void g4097(int x) pre<names_4097>(x > 0);

// A name that is not a constant.
extern const char *const runtime_names[1];
struct runtime_range {
  constexpr const char *const *begin() const { return runtime_names; }
  constexpr const char *const *end() const { return runtime_names + 1; }
};
struct runtime_t {
  using assertion_control_object = runtime_t;
  static constexpr runtime_range group_names{};
};
constexpr runtime_t runtime_label{};
void grt(int x) pre<runtime_label>(x > 0);

// A name longer than 256 characters is read in full: 256 x then y is not
// the group of the 256-x entry, so it is enforced, while 256 x alone is.
struct long_name {
  char s[258];
  constexpr long_name(bool y) : s{} {
    for (int i = 0; i < 256; ++i) s[i] = 'x';
    s[256] = y ? 'y' : 0;
  }
};
constexpr long_name xy{true}, x{false};
struct long_xy_t {
  using assertion_control_object = long_xy_t;
  static constexpr const char *group_names[1] = { xy.s };
};
constexpr long_xy_t long_xy{};
struct long_x_t {
  using assertion_control_object = long_x_t;
  static constexpr const char *group_names[1] = { x.s };
};
constexpr long_x_t long_x{};
constexpr bool fx() { contract_assert<long_x>(false); return true; }
static_assert(fx());
constexpr bool fxy() { contract_assert<long_xy>(false); return true; }
constexpr bool vxy = fxy();

// Every byte value is part of a name: "safety\xfe" is not "safety".
struct byte_t {
  using assertion_control_object = byte_t;
  static constexpr const char *group_names[1] = { "safety\xfe" };
};
constexpr byte_t byte_label{};
constexpr bool fbyte() { contract_assert<byte_label>(false); return true; }
constexpr bool vbyte = fbyte();

// A 0xFF name does not end the range: "safety" after it selects the
// configured group, so ignored.
struct ff_then_safety_t {
  using assertion_control_object = ff_then_safety_t;
  static constexpr const char *group_names[2] = { "safety\xff", "safety" };
};
constexpr ff_then_safety_t ff_safety_label{};
constexpr bool fffsafety() { contract_assert<ff_safety_label>(false); return true; }
static_assert(fffsafety());

// 256 x then zz, 258 characters, is configured on its own: ignored.
struct long_zz_name {
  char s[259];
  constexpr long_zz_name() : s{} {
    for (int i = 0; i < 256; ++i) s[i] = 'x';
    s[256] = 'z';
    s[257] = 'z';
  }
};
constexpr long_zz_name xzz{};
struct long_xzz_t {
  using assertion_control_object = long_xzz_t;
  static constexpr const char *group_names[1] = { xzz.s };
};
constexpr long_xzz_t long_xzz{};
constexpr bool fxzz() { contract_assert<long_xzz>(false); return true; }
static_assert(fxzz());

// A newline inside a name does not split it: "x\nsafety" is not "safety",
// so enforced.
struct newline_t {
  using assertion_control_object = newline_t;
  static constexpr const char *group_names[1] = { "x\nsafety" };
};
constexpr newline_t newline_label{};
constexpr bool fnewline() { contract_assert<newline_label>(false); return true; }
constexpr bool vnewline = fnewline();

// Names returned by value, as objects holding their characters: each is
// read within the full-expression that dereferences the iterator, before
// its temporary is destroyed; "safety" is the second, so ignored.
struct owned_name {
  char s[8];
  constexpr operator const char *() const { return s; }
};
struct owned_iterator {
  int i;
  constexpr owned_name operator*() const {
    return i == 1 ? owned_name{"safety"} : owned_name{"xname"};
  }
  constexpr owned_iterator &operator++() { ++i; return *this; }
  constexpr bool operator==(const owned_iterator &) const = default;
};
struct owned_range {
  constexpr owned_iterator begin() const { return { 0 }; }
  constexpr owned_iterator end() const { return { 2 }; }
};
struct owned_t {
  using assertion_control_object = owned_t;
  static constexpr owned_range group_names{};
};
constexpr owned_t owned_label{};
constexpr bool fowned() { contract_assert<owned_label>(false); return true; }
static_assert(fowned());
