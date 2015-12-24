#include "SuffixArray.h"
#include "SuffixArray2.h"
#include "SuffixTree.h"
#include <cstring>
#define SIZE (1<<27)

char buffer[SIZE];


int main(int argc, char* argv[]){
//	if(FILE* in = fopen(argv[1], "r")){
//		SuffixArray* array = new SuffixArray();
//		
//		size_t read = fread(buffer, sizeof(char), SIZE-1, in);
//		buffer[read] = 0;
//
//		array->build(buffer, read);
//		for(int i = 0; i < read; ++i)
//			printf("%d ", array->getArray()[i]);
//		printf("\n");
//
//		delete array;
//	}

	if(FILE* in = fopen(argv[1], "r")){
		size_t read = fread(buffer, sizeof(char), SIZE-2, in);
		if(buffer[read-1] == '\n') --read;
		buffer[read] = '$', buffer[read+1] = 0;
		SuffixTree tree(NULL);
//		printf("Buinding tree for the text |%s|\n", buffer);
		if(read+1 != strlen(buffer)) throw "xau";
		tree.build(buffer, read+1);
		
		if(FILE* que = fopen(argv[2], "r")) {
			char line[1000];
			while(fgets(line, sizeof(line), que)) {
				int m = strlen(line);
				if(line[m-1] == '\n') line[--m] = 0;
				printf("MatchingLines for Pattern %s\n", line);
				tree.findMatchings(line, size_t(m), false);
			}
		}else printf("File |%s| not found\n", argv[2]);
	}else printf("File |%s| not found\n", argv[1]);
}
