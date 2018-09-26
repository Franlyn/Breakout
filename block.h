#ifndef __BLOCK_H__
#define __BLOCK_H__

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "displayable.h"

using namespace std;

class Block : public Displayable {
private:
	// fixed width and height of each block as the window is not resizable
	int w;
	int h;

	// position of the block
	int x;
	int y;

	// color of the block: 2-6
	int color;

	// True if the block is displayed
	bool ifDisplay;
public:
	Block(int posX, int posY, int c);

	virtual void paint(XInfo &xinfo);

	// get the position
	int get_x();
	int get_y();
	int get_w();
	int get_h();

	// set the position
	void set_x(int posx);
	void set_y(int posy);

	// return true if the block is not transparent
	bool NotTransparent();

	// set the block to transparent
	void setTransparent();

	// set the block to non-transparent
	void setNonTransparent();
};


#endif
