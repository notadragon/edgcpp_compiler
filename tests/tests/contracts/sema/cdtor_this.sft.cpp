//remark:contracts: a constructor's preconditions and a destructor's postconditions need an explicit this
//type:fn
struct T {
  int m;
  int get() const;
  T(int v) pre(m > 0) : m(v) {}                  // Error
  T() pre(get() > 0) : m(0) {}                   // Error, a member function too
  ~T() post(m > 0) {}                            // Error
};
struct U {
  int m;
  U(int v) pre(this->m > 0) post(m > 0) : m(v) {}   // OK
  ~U() pre(m > 0) post(this->m > 0) {}              // OK
};
