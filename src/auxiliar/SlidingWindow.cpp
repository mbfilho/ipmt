#include "SlidingWindow.h"
#define FIX_POS(a) (((a) >= maxSize) ? ((a) - maxSize) : (a))

SlidingWindow::SlidingWindow(int maximumSize) {
	maxSize = maximumSize;
	size = curPos = 0;
	buffer = new int[maxSize];
}

int SlidingWindow::get(int pos) {
	return buffer[FIX_POS(pos + curPos)];
}

void SlidingWindow::append(int arg) {
	buffer[FIX_POS(curPos + size)] = arg;
	++size;
}

bool SlidingWindow::isFull() {
	return size == maxSize;
}

int SlidingWindow::getSize() {
	return size;
}

int SlidingWindow::slide(int slideSize) {
	curPos = FIX_POS(curPos + slideSize);
	size -= slideSize;
}


