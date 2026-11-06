//remark:a floating literal whose exponent is beyond every format's range is out of range like 1.0e400, and a raw literal operator receives it
//type:fn
//match_regex:line 18: error: floating constant is out of range
//match_regex:line 19: error: floating constant is out of range
//match_regex:line 20: error: floating constant is out of range
//require:DO_IL_LOWERING 1
// Not contracts-specific (an upstream fix, EDG-64): with the software
// floating-point conversion, an exponent too large for the conversion's
// bignum failed an assertion (reported as "write_orig_source_line: bad
// lexical escape").  A raw literal operator receives the characters, so its
// literals are valid.
int operator""_w(const char *) { return 0; }
template <char...> int operator""_t() { return 0; }
int i = 1.0e+1234567890_w;                      // OK
int j = 1.0e-1234567890_w;                      // OK
int k = 1.0e+1234567890_t;                      // OK
int l = 1.0e+400_w;                             // OK
double d = 1.0e+400;                            // Error
double e = 1.0e+1234567890;                     // Error
double f = 1.0e-1234567890;                     // Error
