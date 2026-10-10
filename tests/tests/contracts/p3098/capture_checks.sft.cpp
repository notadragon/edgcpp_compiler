//remark:contracts: P3098: a capture of a constructor names members through an explicit this; a capture's copy constructor and destructor must be usable
//type:fn
//options_all:--contracts_p3098
//match_regex:line 9: error: in a postcondition capture of a constructor, a member must be accessed through an explicit "this"
//match_regex:line 12: error: .*is inaccessible
//match_regex:line 14: error: .*deleted function
struct A1 {
  int v = 0;
  A1() post [c = v] (c == 0) {}
};
class P { ~P(); public: P(); P(const P &); int v; };
void f(P p) post [q = p] (q.v == 0);
struct Del { Del(); Del(const Del &) = delete; int v; };
void g(Del d) post [e = d] (true);
struct A2 { int v = 0; ~A2() post [c = v] (c == 0) {} };  // OK
struct A3 { int v = 0; A3() post [c = this->v] (c == 0) {} };  // OK
