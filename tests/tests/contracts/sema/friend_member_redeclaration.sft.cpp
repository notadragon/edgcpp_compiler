//remark:contracts: a friend declaration naming a member function redeclares its contracts
//require:BACK_END_IS_CP_GEN_BE 1
//type:fn
//match_regex:line 22: error: mismatched contract condition in declaration
//match_regex:line 23: error: identifier "nope" is undefined
//match_regex:line 24: error: declaration adds contracts to
//match_regex:line 34: error: mismatched contract condition in declaration
// A friend declaration whose declarator-id names an existing member function
// redeclares it ([dcl.contract.func]): its function contract specifiers, if
// any, are scanned (when the befriending class is complete) and must be the
// same as the member's (formerly EDG-43).
struct Inner {
  void fn(int n) pre(n > 0);
  void fn2(int n) pre(n > 0);
  void fn3(int n) pre(n > 0);
  void fn4(int n) pre(n > 0);
  void fn5(int n);
};
struct Outer {
  friend void Inner::fn3(int m) pre(m > 0);   // OK, parameter renamed
  friend void Inner::fn4(int n);              // OK, omitted
  friend void Inner::fn(int n) pre(n < 0);    // Error, condition
  friend void Inner::fn2(int n) pre(n > 0 && nope > 1);   // Error
  friend void Inner::fn5(int n) pre(n > 0);   // Error, adds contracts
};

// The befriending class is complete in the contract assertions.
struct O2 {
  struct In {
    void fn(int n) pre(n > 0 && bob > 1);
    void fn2(int n) pre(n > 0);
  };
  friend void In::fn(int n) pre(n > 0 && bob > 1);  // OK
  friend void In::fn2(int n) pre(n > 1);            // Error, condition
  static int bob;
};
