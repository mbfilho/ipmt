class Compressor {
public:
	virtual void writeInt(int arg) = 0;
	virtual void writeByte(char arg) = 0;
	virtual void flushAndClose() = 0;
};
