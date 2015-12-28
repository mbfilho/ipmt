#include "Global.h"
#include "LZ78C.h"

char buffer[1<<20];
int main(int argc, char* argv[]) {
	FILE* in = fopen(argv[1], "rb");
	
	assert(in);
	LZ78C comp(argv[2]);
	int tot = 0;	
	while(true) {
		int read = fread(buffer, sizeof(char), 1<<20, in);
		if(!read) break;
		tot += read;
		comp.writeText(buffer, read);
	}
	printf("> %d\n", tot);
	comp.flushAndClose();
	
}

