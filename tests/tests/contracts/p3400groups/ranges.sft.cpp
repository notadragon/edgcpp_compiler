//remark:contracts: P3400 a label's group_names is read as a range-based for walks a range -- an array, begin and end as members or found by argument-dependent lookup -- whose elements convert to const char * (no <string_view>), for the semantic of a run-time check as for constant evaluation; a group_names that is not const, or whose elements do not convert, names no groups
//options_all:--contracts_p3400 --contracts_p4298 --contract_evaluation_semantic=ignore --contract_group_evaluation_semantic=safety:noexcept_observe
//type:rp
// (No system headers: also runs under the C-generating back end, where the
// front end chooses the semantics.)  Mirror:
// gcc/testsuite/g++.dg/contracts/cpp26/p3400-identification-label-range-no-header.C
// (gnu_gcc), there in constant evaluation.
extern "C" int printf(const char *, ...);
extern "C" int fflush(void *);
namespace std { namespace contracts { class contract_violation; } }

static int line;
void handle_contract_violation(const std::contracts::contract_violation &) {
  printf("violation in %d\n", line);
  fflush(0);
}

constexpr const char *names[4] = { "a", "b", "c", "safety" };

struct member_range {
  const char *const *b, *const *e;
  constexpr const char *const *begin() const { return b; }
  constexpr const char *const *end() const { return e; }
};

namespace adl {
  struct sentinel { const char *const *p; };
  struct iterator {
    const char *const *p;
    constexpr const char *operator*() const { return *p; }
    constexpr iterator &operator++() { ++p; return *this; }
    friend constexpr bool operator==(const iterator &i, const sentinel &s) {
      return i.p == s.p;
    }
  };
  struct range { const char *const *b, *const *e; };
  constexpr iterator begin(const range &r) { return { r.b }; }
  constexpr sentinel end(const range &r) { return { r.e }; }
}

struct name {
  const char *s;
  constexpr operator const char *() const { return s; }
};

struct member_t {
  using assertion_control_object = member_t;
  static constexpr member_range group_names{ names, names + 4 };
};
struct adl_t {
  using assertion_control_object = adl_t;
  static constexpr adl::range group_names{ names, names + 4 };
};
struct name_array_t {
  using assertion_control_object = name_array_t;
  static constexpr name group_names[2] = { { "x" }, { "safety" } };
};
struct ptr_array_t {
  using assertion_control_object = ptr_array_t;
  const char *const group_names[3] = { "x", "a_name_longer_than_a_pointer",
                                       "safety" };
};
struct short_t {
  using assertion_control_object = short_t;
  static constexpr member_range group_names{ names, names + 3 };
};
struct nonconst_t {
  using assertion_control_object = nonconst_t;
  const char *group_names[1] = { "safety" };
};
struct int_array_t {
  using assertion_control_object = int_array_t;
  static constexpr int group_names[2] = { 1, 2 };
};

void a(int x) pre<member_t{}>(x > 0) { }
void b(int x) pre<adl_t{}>(x > 0) { }
void c(int x) pre<name_array_t{}>(x > 0) { }
void d(int x) pre<ptr_array_t{}>(x > 0) { }
void e(int x) pre<short_t{}>(x > 0) { }
void f(int x) pre<nonconst_t{}>(x > 0) { }
void g(int x) pre<int_array_t{}>(x > 0) { }

int main() {
  line = 1; a(0);
  line = 2; b(0);
  line = 3; c(0);
  line = 4; d(0);
  line = 5; e(0);
  line = 6; f(0);
  line = 7; g(0);
  printf("end\n");
}
