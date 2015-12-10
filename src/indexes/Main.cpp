#include "SuffixArray.h"
#include "SuffixArray2.h"
#include "SuffixTree.h"
#include <cstring>
#define SIZE (1<<27)

char buffer[SIZE];


int main(int argc, char* argv[]){
	if(FILE* in = fopen(argv[1], "r")){
		SuffixArray* array = new SuffixArray();
		
		size_t read = fread(buffer, sizeof(char), SIZE-1, in);
		buffer[read] = 0;

		array->build(buffer, read);
		for(int i = 0; i < read; ++i)
			printf("%d ", array->getArray()[i]);
		printf("\n");

		delete array;
	}

	SuffixTree tree("tree.dot");
	tree.build("ABABABC", 7);
}
