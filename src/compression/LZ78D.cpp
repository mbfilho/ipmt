#include "LZ78D.h"


LZ78D::LZ78D(FILE* inputFile, int compressionLevel): input(inputFile) {
	dictionary.push_back(make_pair(1, 0)); //Só para facilitar as contas
	nextAvailableBytePos = 0;

	if(compressionLevel == 0) 
		dictionaryMaxSize = 1 << 16;
	else if(compressionLevel == 1)
		dictionaryMaxSize = 1 << 19;
	else
		dictionaryMaxSize = 1 << 30; //infinito
}

void LZ78D::close() {
	input.close();
}

int LZ78D::decompressNextByte() {
	if(nextAvailableBytePos == buffer.size())
		readToken();
	return buffer[nextAvailableBytePos++];
}

void LZ78D::readToken() {
	int uncompressedSeqSize = 0;
	while(!input.getNextBit()) 
		++uncompressedSeqSize;
	
	if(uncompressedSeqSize != 0) {
		for(int i = 0; i < uncompressedSeqSize; ++i)
			buffer.push_back(input.getBunchOfBits(8));
		dictionary.push_back(make_pair(nextAvailableBytePos, buffer.size()-1));	
	} else {
		int entry = decodeInt(), mismatchingByte = input.getBunchOfBits(8); 

		for(int i = dictionary[entry].first; i <= dictionary[entry].second; ++i) 
			buffer.push_back(buffer[i]);
		buffer.push_back(mismatchingByte);	

		dictionary.push_back(make_pair(nextAvailableBytePos, buffer.size() - 1));
	}
	
	if(dictionary.size() >= dictionaryMaxSize) {
		dictionary.resize(1);
		for(int i = 0; i + nextAvailableBytePos < buffer.size(); ++i) {
			buffer[i] = buffer[nextAvailableBytePos+i];
		}
		buffer.resize(buffer.size()-nextAvailableBytePos);
		nextAvailableBytePos = 0;
	}
}

int LZ78D::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
