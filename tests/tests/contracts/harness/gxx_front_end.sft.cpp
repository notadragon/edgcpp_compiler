//remark:gxx configuration: front end only, compile and link modes
//options:-DMODE=1;fp:-DMODE=2;fn:-DMODE=3;cp:-DMODE=4;lp
#if MODE == 2
int f() { return undeclared; }  // Error
#else
int main() { return 0; }
#endif
