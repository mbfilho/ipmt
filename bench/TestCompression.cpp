#include "Global.h"
#include "LZ78C.h"
#include "LZ78D.h"
#include "LZWC.h"
#include "LZWD.h"
#include "LZ77C.h"
#include "LZ77D.h"

struct Config {
	string inputFile;
	string outputFile;
	string mode;
	int level;
	int wb, wl;
	string algo;
	int originalFileSize;
} config;

struct option options[] =
{
	{"input", required_argument, 0, 'i'},
	{"output", required_argument, 0, 'o'},
	{"mode", required_argument, 0, 'm'},
	{"level", required_argument, 0, 'l'},
	{"wb", required_argument, 0, 'b'},
	{"wl", required_argument, 0, 'w'},
	{"algo", required_argument, 0, 'a'},
	{"originalFileSize", required_argument, 0, 's'},
	{0, 0, 0, 0}
};


void parse(int argc, char* argv[]) {
	
	while(true) {
		int c = getopt_long(argc, argv, "", options, NULL);

		if(c == -1) 
			break;

		if(c == 'i')
			config.inputFile = optarg;
		else if(c == 'o')
			config.outputFile = optarg;
		else if(c == 'm')
			config.mode = optarg;
		else if(c == 'l')
			config.level = atoi(optarg);
		else if(c == 'b')
			config.wb = atoi(optarg);
		else if(c == 'w')
			config.wl = atoi(optarg);
		else if(c == 'a')
			config.algo = optarg;
		else if(c == 's')
			config.originalFileSize = atoi(optarg);
		else
			throw 1;
	}
}

int buffer[1<<20];
void testCompression() {
	Compressor* compressor = NULL;
	FILE* input = fopen(config.inputFile.c_str(), "rb");
	FILE* output = fopen(config.outputFile.c_str(), "wb");
	
	assert(input != NULL);
	assert(output != NULL);

	if(config.algo == "lz77") {
		compressor = new LZ77C(output, config.wb, config.wl);
		int args[] = { 0, config.wb, config.wl };
		fwrite(args, sizeof(int), 3, output); 
	} else if(config.algo == "lz78") {
		int args[] = {1, config.level};
		compressor = new LZ78C(output, config.level);
		fwrite(args, sizeof(int), 2, output); 
	} else if(config.algo == "lzw") {
		int args[] = {2, config.level};
		compressor = new LZWC(output, config.level);
		fwrite(args, sizeof(int), 2, output); 
	}
	
	assert(compressor != NULL);
	
	int read;
	while((read = fread(buffer, sizeof(int), 1<<20, input))) {
		for(int i = 0; i < read; ++i)
			compressor->feedRawBits(buffer[i], 32);	
	}
		
	compressor->flushAndClose();	
	fclose(input);
}

void testDecompression() {
	Decompressor* decompressor = NULL;
	FILE* input = fopen(config.inputFile.c_str(), "rb");
	FILE* output = fopen(config.outputFile.c_str(), "wb");

	assert(input != NULL);
	assert(output != NULL);
	int algo = -1;
	assert(1 == fread(&algo, sizeof(algo), 1, input));

	if(algo == 0) { //lz77
		int args[2];
		assert(2 == fread(args, sizeof(int), 2, input));
		decompressor = new LZ77D(input, args[0], args[1]);
	} else if(algo == 1) { //lz78
		assert(1 == fread(&config.level, sizeof(int), 1, input));
		decompressor = new LZ78D(input, config.level);
	} else if(algo == 2) { //lzw
		assert(1 == fread(&config.level, sizeof(int), 1, input));
		decompressor = new LZWD(input, config.level);
	}
	assert(decompressor != NULL);
	
	int numberOfInts =  config.originalFileSize / 4;
	for(int i = 0; i < numberOfInts; ++i) {
		ui arg = decompressor->readBits(32);
		fwrite(&arg, sizeof(arg), 1, output);
	}

	if(config.originalFileSize % 4) {
		int remaining = config.originalFileSize % 4;
		for(int i = 0; i < remaining; ++i) {
			ui arg = decompressor->readBits(8);
			fwrite(&arg, sizeof(char), 1, output);
		}
	}

	decompressor->close();
	fclose(output);
}

int main(int argc, char* argv[]) {
	parse(argc, argv);
	if(config.mode == "c")
		testCompression();	
	else if(config.mode == "d")
		testDecompression();
	else
		throw config.mode;
}

