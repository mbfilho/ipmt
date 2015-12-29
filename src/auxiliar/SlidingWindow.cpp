#include "SlidingWindow.h"
#define FIX_POS(a) (((a) >= maxSize) ? ((a) - maxSize) : (a))

SlidingWindow::SlidingWindow(int maximumSize) {
	maxSize = maximumSize;
	size = curPos = 0;
	buffer = new int[maxSize];
}

int SlidingWindow::getValueAt(int pos) {
	return buffer[FIX_POS(pos + curPos)];
}

void SlidingWindow::appendElement(int arg) {
	buffer[FIX_POS(curPos + size)] = arg;
	++size;
}

int SlidingWindow::getSize() {
	return size;
}

int SlidingWindow::slide(int slideSize) {
	curPos = FIX_POS(curPos + slideSize);
	size -= slideSize;
}


