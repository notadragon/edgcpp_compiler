//remark:contracts: without P3400 (and without P4451, which C++26 enables and which gives such a specifier its own meaning) an assertion-control specifier is diagnosed and skipped, and contract_control is an identifier
//type:fn
//options_all:--contract_evaluation_semantic=quick_enforce --no_contracts_p4451
//match_regex:line 9: error: assertion-control specifiers require --contracts_p3400
//match_regex:line 10: error: assertion-control specifiers require --contracts_p3400
//match_regex:line 11: error: assertion-control specifiers require --contracts_p3400
struct Label { using assertion_control_object = Label; };
constexpr Label lbl{};
void a(int x) pre<lbl>(x > 0);
int b(const int x) post<(lbl)>(r: r > x);
void c() { contract_assert<lbl>(true); }
int contract_control = 0;                   // OK, not a keyword
