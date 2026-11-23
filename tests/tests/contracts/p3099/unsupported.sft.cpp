//remark:contracts: P3099: a message that is not a string literal is not yet supported, and an encoding prefix is not allowed
//type:fn
//options_all:--contracts_p3099
//match_regex:line 13: error: a diagnostic message that is not a string literal is not yet supported
//match_regex:line 14: error: a diagnostic message cannot have an encoding prefix
//match_regex:line 15: error: a diagnostic message cannot have an encoding prefix
struct msg {
  constexpr const char *data() const { return "m"; }
  constexpr unsigned long size() const { return 1; }
};
constexpr msg m;
void f(int x) {
  contract_assert(x > 0, m);
  contract_assert(x > 0, u8"m");
  contract_assert(x > 0, L"m");
}
