//remark:contracts: P3098: the declarations of a function give the same captures, as written
//type:fn
//options_all:--contracts_p3098
//match_regex:line 11: error: mismatched postcondition captures in declaration
//match_regex:line 13: error: mismatched postcondition captures in declaration
//match_regex:line 14: error: mismatched postcondition captures in declaration
//match_regex:line 17: error: mismatched contract condition in declaration
int f(int x) post [y = x] (r: r > y);
int f(int x) post [y = x] (r: r > y);   // OK
int f(int a) post [y = a] (r: r > y);   // OK: a parameter is identified by its position
int f(int x) post [y = x + 0] (r: r > y);
int g(int x) post [y = x] (true);
int g(int x) post [y = x, z = x] (true);
int g(int x) post [z = x] (true);
int h(int x) post [y = x] (y > 0);
int h(int x) post [y = x] (y > 0);      // OK
int h(int x) post [y = x] (x > 0);
