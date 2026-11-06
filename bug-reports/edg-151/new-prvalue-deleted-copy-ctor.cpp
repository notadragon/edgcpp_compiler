// The C++-generating front end (cpfe-cp) rejects both new-expressions:
// "function "T::T(const T &)" ... cannot be referenced -- it is a deleted
// function".  C++17 and later initialize the allocated object directly
// from the prvalue; no copy constructor is involved.
struct T {
  T();
  T(const T &) = delete;
};
T make();
T *a = new T(T());
T *b = new T(make());
T *c = new T{T()};  // Accepted.
