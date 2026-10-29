//remark:contracts: without P3097 a virtual function cannot have function contract specifiers
//type:fn
//options:--no_contracts_p3097:--contracts_p3850 --no_contracts_p3097
//match_regex:line 6: error: a virtual function cannot have a function contract specifier
struct B {
  virtual int f(int x) pre(x > 0);
};
