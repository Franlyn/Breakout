#ifndef __PADDLE_H__
#define __PADDLE_H__

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "displayable.h"

using namespace std;

class Paddle : public Displayable {
private:
	// fixed width and height of paddle
	int w;
	int h;

	// position of the paddle
	int x;
	int y;

	// Moving speed of the paddle
	int move_speed;
public:
	Paddle(int posX, int posY, int speed);

	virtual void paint(XInfo &xinfo);

	// get the position
	int get_x();
	int get_y();
	int get_w();
	int get_h();
	bool NotTransparent();

	// set the position
	void set_x(int posx);
	void set_y(int posy);

	// move the paddle
	void move(bool ifLeft, int ifLaunched);
};


#endif
