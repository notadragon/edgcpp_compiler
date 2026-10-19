//remark:contracts: EDG-73: the C-generating back end puts out a function whose only parameter is ... as T f(), which a gcc defaulting to C23 rejects at a call with arguments
//require:DO_IL_LOWERING 1
//type:rp
// Records the current failure; a fix shows up as a deviation.
int h(...) { return 1; }
int main() { return h(1, 2) == 1 ? 0 : 1; }
