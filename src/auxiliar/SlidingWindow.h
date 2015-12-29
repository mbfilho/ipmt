#ifndef SLIDING_WINDOW_H
#define SLIDING_WINDOW_H

class SlidingWindow {
public:
	SlidingWindow(int maximumSize);
	
	int get(int pos);
	void append(int element);
	int getSize();
	bool isFull();
	int slide(int slideSize);
private:
	int maxSize;
	int size;
	int curPos;
	int *buffer;
};

#endif
