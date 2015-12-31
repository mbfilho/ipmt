#include "Global.h"
#include "LZ78C.h"
#include "LZ78D.h"
#include "LZWC.h"
#include "LZWD.h"
#include "LZ77C.h"
#include "LZ77D.h"

char buffer[1<<20];
void testCompression(const char *input, const char *output) {
	printf("NIE\n");
	FILE* in = fopen(input, "rb");
	assert(in);
	LZ77C comp(output, 4096, 16);
	int tot = 0;	
	while(true) {
		int read = fread(buffer, sizeof(char), 1<<20, in);
		if(!read) break;
		tot += read;
		comp.writeText(buffer, read);
	}
	printf("> %d\n", tot);
	comp.flushAndClose();

	LZ77D dec(output);
	FILE* decomp = fopen("decomp", "wb");
	int b;
//	int tot =52428800;
	for(int i = 0; i < tot; ++i) {
		b = dec.readByte();
		fwrite(&b, sizeof(char), 1, decomp);
	}
	fclose(decomp);
}

void testCompressionAndDecompression() {
	LZ77C comp("comprimido", 8, 4);
	int qtd = 10000;
	for(int i = 0; i < qtd; ++i)
		comp.writeInt(i);

	comp.flushAndClose();

	LZ77D decomp("comprimido");
	for(int i = 0; i < qtd; ++i)
		printf("> %d\n", decomp.readInt());
	decomp.close();
}

void testOnlyCompression() {
	LZ77C comp("out", 4, 4);
	int seq[] = {0, 1, 1, 2, 0, 1, 1, 0, 2, 3, 0, 1, 0, 0, 0};
	int s = 15;

	for(int i = 0; i < s; ++i)
		comp.writeByte(seq[i]);
	comp.flushAndClose();
}

int main(int argc, char* argv[]) {
//	testOnlyCompression();
	testCompression(argv[1], argv[2]);	
//	testCompressionAndDecompression();
}

