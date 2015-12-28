#include "Global.h"
#include "LZ78C.h"

int main(int argc, char* argv[]) {
	LZ78C comp(argv[1]);
	int x[] = {8224};

	for(int i = 0; i < 1; ++i)
		comp.writeInt(x[i]);
	comp.flushAndClose();
}

