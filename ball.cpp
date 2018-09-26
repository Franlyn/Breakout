#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "ball.h"


Ball::Ball(int posX, int posY, int radius, int speed) {
	this->x = posX;
	this->y = posY;
	this->r = radius;
	this->launched = false;

	// initially, the ball bounce left and up
	this->move_speed = speed;
	this->dirX = (0 - this->move_speed / 2);
	this->dirY = (0 - this->move_speed);
}


void Ball::paint(XInfo &xinfo) {
	XFillArc(xinfo.display, xinfo.pixmap, xinfo.gc[7],
			         this->x,
			         this->y,
			         2 * this->r, 2 * this->r,
			         0, 360 * 64);
}

// Return the position of the ball
int Ball::get_x() {
	return this->x;
}

int Ball::get_y() {
	return this->y;
}

int Ball::get_r() {
	return this->r;
}

bool Ball::ifLaunched() {
	return this->launched;
}

void Ball::setNotLaunched() {
	this->launched = false;
}

int Ball::get_w() {
	return 2 * this->r;
}

int Ball::get_h() {
	return 2 * this->r;
}

bool Ball::NotTransparent() {
	return false;
}

void Ball::set_x(int posx) {
	this->x = posx;
}

void Ball::set_y(int posy) {
	this->y = posy;
}

void Ball::set_dirX(int dx) {
	this->dirX = dx;
}

void Ball::set_dirY(int dy) {
	this->dirY = dy;
}


// bounce the ball when game starts
void Ball::bounce() {
	if (launched) return;
	launched = true;
	this->x += this->dirX;
	this->y += this->dirY;
	cout << "Ball bouncing up!" << endl;
}

/* return 1 if the ball hits the paddle; 0 otherwise */
int Ball::hitPaddle(Paddle *paddle) {
	int leftside = this->x;
	int rightside = this->x + 2 * this->r;
	int upperside = this->y;
	int lowerside = this->y + 2 * this->r;

	int paddleL = paddle->get_x();
	int paddleR = paddle->get_x() + paddle->get_w();

	// hit the left or right side
	if ((lowerside >= paddle->get_y()) and 
		(rightside >= paddleL and leftside <= paddleL) and
		(this->dirX > 0)) {
		this->dirX *= -1;
		return 1;
	}
	if ((lowerside >= paddle->get_y()) and 
		(this->dirX < 0) and
		(leftside <= paddleR and rightside >= paddleR)) {
		this->dirX *= -1;
		return 1;
	}

	// hit the upper interface of the paddle
	if (lowerside >= paddle->get_y() and 
		((rightside >= paddleL and rightside <= paddleR) or
		(leftside >= paddleL and leftside <= paddleR))) {
		//cout << "hit paddle! lowerside is " << lowerside <<"rightside "<<rightside<<",leftside:"<<leftside<< 
	//",paddleY is "<< paddle->get_y() << ",paddleX"<<paddle->get_x()<<",paddleW "<<paddle->get_w()<< endl;
		return 1;
	}
	return 0;
}

/* return 1 if the ball hits the block; 0 otherwise */
int Ball::hitBlock(Block *dp) {
	int leftside = this->x;
	int rightside = this->x + 2 * this->r;
	int upperside = this->y;
	int lowerside = this->y + 2 * this->r;

	int blockL = dp->get_x();
	int blockR = dp->get_x() + dp->get_w();
	int blockUp = dp->get_y();
	int blockLo = dp->get_y() + dp->get_h();

	// hit the block
	// not sufficient. TODO: update the logic
	if (((upperside <= blockLo and upperside >= blockUp) or
		(lowerside <= blockLo and lowerside >= blockUp)) and
		((rightside >= blockL and rightside <= blockR) or
		(leftside >= blockL and leftside <= blockR)) ) {
		dp->setTransparent();
		/*cout << "hit blocks! ls:"<< leftside <<", rs:"<<rightside<<
		",us: "<<upperside<<",los: "<<lowerside<<"bl:"<< blockL<<
		", br:"<<blockR<<",bup:"<<blockUp<<",bd:"<<blockLo << endl;*/
		return 1;
	}
	return 0;
}

/* move the ball 
	return 0 if the ball didn't hit paddle
	return 1 if hits a paddle
	return 2 if the paddle didn't catch the ball -> gameover
	return 3 if all blocks are transparent -> player wins */
int Ball::move(list<Displayable*> blocks, Paddle *paddle) {
	// if the ball has not launched yet, do not move
	if (this->launched == false) return 0;

	// check if the ball hits the paddle
	if (hitPaddle(paddle) == 1) {
		// bounce up
		this->dirY *= -1;

		// will not hit the blocks, update x, y
		this->x += this->dirX;
		this->y += this->dirY;
		return 1;
	}

	// check if the ball hits the border of the window
	if (this->y <= 0) {	// up
		this->dirY = (0 - this->dirY);
	} else if (this->y + 2 * this->r >= WINDOW_HEIGHT) {	// down
		cout << "Player lost!" << endl;
		return 2;
	}
	if (this->x <= 0 || this->x + 2 * this->r >= WINDOW_WIDTH) { // left or right
		this->dirX = (0 - this->dirX);
	}

	// check if the ball hits the blocks
	list<Displayable*>::const_iterator begin = blocks.begin();
	list<Displayable*>::const_iterator end = blocks.end();
	bool allTransparent = true;
	bool hit = false;

	while(begin != end) {
		//Displayable* d = *begin;
		Block *d = dynamic_cast<Block*>(*begin);
		if (d->NotTransparent()) {
			allTransparent = false;
			if (hitBlock(d) == 1) {
				hit = true;
			}
		} 
		begin++;
	}

	if (hit) this->dirY = (0 - this->dirY);
	if (allTransparent) return 3;

	// update x, y
	this->x += this->dirX;
	this->y += this->dirY;
	return 0;
}


