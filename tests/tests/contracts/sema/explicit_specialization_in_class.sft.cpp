//remark:contracts: the contract assertions of an explicit specialization declared in a class are a complete-class context
//require:DO_IL_LOWERING 1
//type:fn
//match_regex:line 27: error: mismatched contract condition in declaration
//match_regex:line 29: error: declaration adds contracts to
//match_regex:line 30: error: identifier "zz" is undefined
// The function contract specifiers of an explicit specialization declared in
// a class (CWG727), like any member function's, are a complete-class context
// ([class.mem.general]): they name members declared after them, in a class
// and in a class template, on a declaration and on a definition.  A
// redeclaration of the specialization in the class repeats them or omits
// them, matched when the class is complete.  (Only under the edg_x86_64
// configuration: in GNU mode, as in g++, an explicit specialization cannot
// be declared in a class.)

struct S {
  template <class T> void f(T a) pre(a > 0);
  template <> void f<int>(int a) pre(a > lim());            // OK
  template <> void f<char>(char a) pre(a > lim()) {}        // OK
  template <class T> int g(T a) post(r: r > 0);
  template <> int g<int>(int a) post(r: r > lim());         // OK
  template <> void f<short>(short a) pre(a > lim());
  template <> void f<short>(short b) pre(b > lim()) {}      // OK, renamed
  template <> void f<unsigned>(unsigned a) pre(a > lim());
  template <> void f<unsigned>(unsigned a) {}               // OK, none
  template <> void f<long>(long a) pre(a > lim());
  template <> void f<long>(long a) pre(a > 2);              // Error, condition
  template <> void f<float>(float a);
  template <> void f<float>(float a) pre(a > lim());        // Error, adds
  template <> void f<double>(double a) pre(a > zz());       // Error
  static constexpr int lim() { return 1; }
};

template <class U> struct C {
  template <class T> void f(T a) pre(a > 0);
  template <> void f<int>(int a) pre(a > lim());            // OK
  template <> void f<char>(char a) pre(a > lim()) {}        // OK
  static constexpr U lim() { return 1; }
};
template struct C<int>;
void use() { C<long>().f(1); C<long>().f('a'); }
