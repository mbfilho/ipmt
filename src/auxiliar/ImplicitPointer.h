#ifndef IMPLICIT_POINTER_H
#define IMPLICIT_POINTER_H

struct ImplicitPointer {
	int v, st, end;

	ImplicitPointer(int vv, int ss, int ee): v(vv), st(ss), end(ee){}

	bool isImplicit() {	return st <= end; }
	int strSize(){ return end - st + 1; }
};
#endif
