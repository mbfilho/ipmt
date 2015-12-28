class Decompressor {
public:
	virtual int readInt() = 0;
	virtual char readByte() = 0;
	virtual void close() = 0;
};
