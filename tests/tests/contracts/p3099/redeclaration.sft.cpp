//remark:contracts: P3099: the declarations of a function give the same diagnostic message, as text, or none
//type:fn
//options_all:--contracts_p3099
//match_regex:line 11: error: mismatched contract diagnostic message in declaration
//match_regex:line 14: error: mismatched contract diagnostic message in declaration
//match_regex:line 17: error: mismatched contract diagnostic message in declaration
// The same text written differently (concatenated, escaped) is the same
// message.
int f(int x) pre(x > 0, "m");
int f(int x) pre(x > 0, "" "m");        // OK
int f(int x) pre(x > 0, "n");           // error
int f(int x) pre(x > 0, "\x6d");        // OK
int g(int x) pre(x > 0);
int g(int x) pre(x > 0, "m");           // error
int g(int x) pre(x > 0);                // OK
int h(int x) pre(x > 0, "m");
int h(int x) pre(x > 0);                // error
