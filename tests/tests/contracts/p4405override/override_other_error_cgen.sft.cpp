//remark:contracts: P4405 virtual functions with the C-generating back end, after an unrelated error in the translation unit (our GCC's rule since GCC-947): the override obligations of each definition that would have been put out are diagnosed -- a definition that is not inline; an inline one whose class's virtual function table is defined here (its decider function defined here, or none and a constructor defined here); a member of a class template instance constructed here -- and not those of an inline overrider whose virtual function table is defined elsewhere, or of a class nothing here constructs (DECISIONS.md E150).  Mirror: gnu_gcc g++.dg/contracts/cpp26/p4405-override-after-error.C
//require:DO_IL_LOWERING 1
//type:fn
//options:--contracts_p3097 --contracts_p4405 --contract_evaluation_semantic=ignore --contract_analysis_semantic=prove
//match_regex:line ([0-9]+): (error|warning): .*
//match_regex:the predicate of the contract assertion at line [0-9]+
// (No system headers: the C-generating back end cannot find them natively.)
// The same as without the error on the next line.  Differences from GCC:
// F::f's pre (false) is "cannot be proven", not "always false" (the front
// end folds nothing to false for an override obligation).
int bad = undeclared;

struct B {
  virtual int f(int x)
    pre (x > 0)
    post (r: r > 0);
};

// Its precondition and B::f's postcondition.
struct D : B {
  int f(int x) override
    pre (x > 0)
  {
    return x;
  }
};

// Likewise.
struct F : B {
  int f(int x) override
    pre (false)
  {
    return 1;
  }
};

// Its virtual function table is defined elsewhere (the key function is
// defined in another translation unit): nothing.
struct Elsewhere : B {
  virtual void key();
  int f(int x) override
    pre (false)
  {
    return 1;
  }
};

// Its virtual function table is defined here (the key function): both.
struct KeyHere : B {
  virtual void key();
  int f(int x) override pre (x > 1) { return 1; }
};
void KeyHere::key() {}

// Not inline: both, wherever its virtual function table is.
struct OutOfLine : B {
  virtual void key();
  int f(int x) override pre (x > 2);
};
int OutOfLine::f(int x) { return x; }

// Nothing constructs it: nothing.
struct Unused : B {
  int f(int x) override pre (x > 3) { return x; }
};

// A class template's instance, constructed here: both.
template <typename T> struct TI : B {
  int f(int x) override pre (x > 4) { return x; }
};
TI<int> ti;

D d;
F fo;
