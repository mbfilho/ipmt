#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

class SlidingWindow {
public:
	SlidingWindow(int maximumSize);
	
	int getValueAt(int pos);
	void appendElement(int element);
	int getSize();
	int slide(int slideSize);
private:
	int maxSize;
	int size;
	int curPos;
	int *buffer;
};

#endif
