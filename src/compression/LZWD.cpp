#include "LZWD.h"


LZWD::LZWD(FILE* inputFile): input(inputFile) {
	lastSequence = make_pair(-1,-1);
	for(int i = 0; i < 256; ++i)
		buffer.push_back(i);
	dictionary.push_back(make_pair(-1,-1));//soh para facilitar as contas
	for(int i= 0; i < 256; ++i)
		dictionary.push_back(make_pair(i, i));
	nextAvailableBytePos = buffer.size();
}

void LZWD::close() {
	input.close();
}

int LZWD::decompressNextByte() {
	if(nextAvailableBytePos == buffer.size())
		readToken();
		
	return buffer[nextAvailableBytePos++];
}

void LZWD::readToken() {
	int uncompressedSeqSize = 0;
	while(!input.getNextBit()) {
		++uncompressedSeqSize;
	}

	if(uncompressedSeqSize != 0) { //Vem aí uma sequência não comprimida
		if(lastSequence.first != -1) {
			int sizeOfLastSeq = lastSequence.second - lastSequence.first + 1;
			dictionary.push_back(make_pair(buffer.size()-sizeOfLastSeq, buffer.size()));
		}

		for(int i = 0; i < uncompressedSeqSize; ++i)
			buffer.push_back(input.getBunchOfBits(8));
		lastSequence = make_pair(nextAvailableBytePos, buffer.size() - 1);

	} else { //Vem ai um token comprimido
		int pos = decodeInt();
		if(pos == dictionary.size()) {
			for(int i = lastSequence.first; i <= lastSequence.second; ++i) 
				buffer.push_back(buffer[i]);
			buffer.push_back(buffer[lastSequence.first]);	
			dictionary.push_back(make_pair(nextAvailableBytePos, buffer.size()-1));
		} else {
			if(lastSequence.first != -1) {
				int sizeOfLastSeq = lastSequence.second - lastSequence.first + 1;
				dictionary.push_back(make_pair(buffer.size()-sizeOfLastSeq, buffer.size()));
			}
			for(int i = dictionary[pos].first; i <= dictionary[pos].second; ++i){
				buffer.push_back(buffer[i]);	
			}
		}

		lastSequence = dictionary[pos];
	}

	if(dictionary.size() + 1 >= (1<<19)) {
		lastSequence = make_pair(-1,1);
		
		for(int i = 0; nextAvailableBytePos + i < buffer.size(); ++i){
			buffer[256+i] = buffer[nextAvailableBytePos+i];
		}
		buffer.resize(256 + buffer.size() - nextAvailableBytePos);
		nextAvailableBytePos = 256;

		dictionary.resize(257);
	}
}

int LZWD::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
