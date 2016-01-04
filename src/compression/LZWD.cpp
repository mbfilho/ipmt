#include "LZWD.h"


LZWD::LZWD(FILE* inputFile): input(inputFile) {
	lastPos = -1;
	for(int i = 0; i < 256; ++i)
		buffer.push_back(i);
	table.push_back(make_pair(-1,-1));//soh para facilitar as contas
	for(int i= 0; i < 256; ++i)
		table.push_back(make_pair(i, i));
	nextAvailableByte = buffer.size();
}

void LZWD::close() {
	input.close();
}

int LZWD::readByte() {
	if(nextAvailableByte == buffer.size())
		readToken();
		
	return buffer[nextAvailableByte++];
}

void LZWD::readUncompressedSequence(int size) {
}

void LZWD::readToken() {
	int uncompressedSeqSize = 0;
	while(!input.getNextBit()) {
		++uncompressedSeqSize;
	}

	if(uncompressedSeqSize != 0) {
		readUncompressedSequence(uncompressedSeqSize);
	} else {
		int pos = decodeInt();
		if(pos == table.size()) {
			table.push_back(make_pair(buffer.size(), -1));
			for(int i = table[lastPos].first; i <= table[lastPos].second; ++i) 
				buffer.push_back(buffer[i]);
			buffer.push_back(buffer[table[lastPos].first]);	
			table.back().second = buffer.size() - 1;
		} else {
			if(lastPos != -1) {
				int sizeOfLastSeq = table[lastPos].second - table[lastPos].first + 1;
				table.push_back(make_pair(buffer.size()-sizeOfLastSeq, buffer.size()));
			}
			for(int i = table[pos].first; i <= table[pos].second; ++i){
				buffer.push_back(buffer[i]);	
			}
		}

		lastPos = pos;
	}
	
	if(table.size() + 1 >= (1<<19)) {
		lastPos = -1;
		
		for(int i = 0; nextAvailableByte + i < buffer.size(); ++i){
			buffer[256+i] = buffer[nextAvailableByte+i];
		}
		buffer.resize(256 + buffer.size() - nextAvailableByte);
		nextAvailableByte = 256;

		table.resize(257);
	}
}

int LZWD::decodeInt() {
	int sizeOfSize = 0, size = 0;	

	while(input.getNextBit()) 
		++sizeOfSize; 
	size = (int) input.getBunchOfBits(sizeOfSize);	
	return (int) input.getBunchOfBits(size);
}
