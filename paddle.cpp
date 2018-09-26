#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "paddle.h"


Paddle::Paddle(int posX, int posY, int speed) {
	this->x = posX;
	this->y = posY;

	this->w = 140;
	this->h = 10;
	this->move_speed = speed;
}


void Paddle::paint(XInfo &xinfo) {
	XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[1], x, y, w, h);
	//XDrawRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, w, h);
}

// Return the position of the Paddle
int Paddle::get_x() {
	return this->x;
}

int Paddle::get_y() {
	return this->y;
}

int Paddle::get_w() {
	return this->w;
}

int Paddle::get_h() {
	return this->h;
}

void Paddle::set_x(int posx) {
	this->x = posx;
}

void Paddle::set_y(int posy) {
	this->y = posy;
}

bool Paddle::NotTransparent() {
	return false;
}

// return true if the Paddle is not transparent
void Paddle::move(bool ifLeft, int ifLaunched) {
	if (!ifLaunched) return;
	
	int nextPos = this->x;
	if (ifLeft) {
		nextPos = this->x - this->move_speed;
	} else {
		nextPos = this->x + this->move_speed;
	}

	// check if the paddle is within the screen
	if (nextPos >= 0 && nextPos <= WINDOW_WIDTH - this->w) {
		this->x = nextPos;
	} else if (nextPos < 0) {
		this->x = 0;
	} else if (nextPos > WINDOW_WIDTH - this->w) {
		this->x = WINDOW_WIDTH - this->w;
	}
}


