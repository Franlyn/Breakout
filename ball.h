#ifndef __BALL_H__
#define __BALL_H__

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <list>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "displayable.h"
#include "block.h"
#include "paddle.h"

using namespace std;

class Ball : public Displayable {
private:
	// fixed radius
	int r;

	// position of the ball
	int x;
	int y;

	// Moving speed of the ball
	int move_speed;

	// bounce direction
	int dirX;
	int dirY;

	// if launched
	bool launched;
public:
	Ball(int posX, int posY, int radius, int speed);

	virtual void paint(XInfo &xinfo);

	// get the position, (x,y) is upper left
	int get_x();
	int get_y();
	int get_r();

	bool ifLaunched();

	// set launched = false
	void setNotLaunched();

	// virtual method from displayable.h
	int get_w();
	int get_h();
	bool NotTransparent();

	// set the position
	void set_x(int posx);
	void set_y(int posy);
	void set_dirX(int dx);
	void set_dirY(int dy);

	// bounce the ball
	void bounce();

	/* return 1 if the ball hits the paddle; 0 otherwise */
	int hitPaddle(Paddle *paddle);

	// detect if the ball hits a block
	int hitBlock(Block *dp);

	/* move the ball 
		return 0 if the ball didn't hit paddle
		return 1 if hits a paddle
		return 2 if the paddle didn't catch the ball -> gameover
		return 3 if all blocks are transparent -> player wins */
	int move(list<Displayable*> blocks, Paddle *paddle);
};


#endif
