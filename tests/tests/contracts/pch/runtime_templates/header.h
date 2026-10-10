extern "C" int puts(const char *);
template <class T> T t0(T x) pre(x > 0) post(r: r < 9) { return x; }
template <class T> T t1(T x) pre(x > 1) post(r: r < 10) { return x; }
template <class T> T t2(T x) pre(x > 2) post(r: r < 11) { return x; }
template <class T> T t3(T x) pre(x > 3) post(r: r < 12) { return x; }
template <class T> T t4(T x) pre(x > 4) post(r: r < 13) { return x; }
template <class T> T t5(T x) pre(x > 5) post(r: r < 14) { return x; }
template <class T> T t6(T x) pre(x > 6) post(r: r < 15) { return x; }
template <class T> T t7(T x) pre(x > 7) post(r: r < 16) { return x; }
