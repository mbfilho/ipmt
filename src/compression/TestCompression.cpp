#include "Global.h"
#include "LZ78C.h"
#include "LZ78D.h"

char buffer[1<<20];
void testCompression(const char *input, const char *output) {
	FILE* in = fopen(input, "rb");
	
	assert(in);
	LZ78C comp(output);
	int tot = 0;	
	while(true) {
		int read = fread(buffer, sizeof(char), 1<<20, in);
		if(!read) break;
		tot += read;
		comp.writeText(buffer, read);
	}
	printf("> %d\n", tot);
	comp.flushAndClose();

	LZ78D dec(output);
	FILE* decomp = fopen("decomp", "wb");
	int b;

	while((b = dec.readByte()) != 256) {
		fwrite(&b, sizeof(char), 1, decomp);
	}
	fclose(decomp);
}

void testCompressionAndDecompression() {
	LZ78C comp("comprimido");
	int qtd = 10000;
	for(int i = 0; i < qtd; ++i)
		comp.writeInt(i);

	comp.flushAndClose();

	LZ78D decomp("comprimido");
	for(int i = 0; i < qtd; ++i)
		printf("> %d\n", decomp.readInt());
	decomp.close();
}

int main(int argc, char* argv[]) {
	testCompression(argv[1], argv[2]);	
//	testCompressionAndDecompression();
}

