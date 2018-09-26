#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <ctime>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "block.h"

Block::Block(int posX, int posY, int c) {
	this->x = posX;
	this->y = posY;
	this->ifDisplay = true;
	this->w = 90;
	this->h = 60;
	this->color = c;
}


void Block::paint(XInfo &xinfo) {
	if (this->NotTransparent()) {
		// random number 1-6 inclusive
		//srand((int)time(0));

		//int random = (rand() % 6) + 1;
		//cout << "start paint. x,y is " << x << ", " << y << "w,h is " << w << ", " << h << endl;
		XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[this->color], x, y, w, h);
		//XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, w, h);
	}
}

// Return the position of the block
int Block::get_x() {
	return this->x;
}

int Block::get_y() {
	return this->y;
}

int Block::get_w() {
	return this->w;
}

int Block::get_h() {
	return this->h;
}

void Block::set_x(int posx) {
	this->x = posx;
}

void Block::set_y(int posy) {
	this->y = posy;
}

// return true if the block is not transparent
bool Block::NotTransparent() {
	return this->ifDisplay;
}

void Block::setNonTransparent() {
	this->ifDisplay = True;
}

// set the block to transparent
void Block::setTransparent() {
	this->ifDisplay = False;
}
