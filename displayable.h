#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <cstdlib>
#include <iostream>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 800

struct XInfo {
	// X11 structures
	Display *display;
	Window window;
	GC gc[8];

	// double buffering
	// ref: http://www.dis.uniroma1.it/~liberato/screensaver/double.html
	Pixmap pixmap;

	int screennum;
};

class Displayable {
public:
	// get the position
	virtual int get_x() = 0;
	virtual int get_y() = 0;
	virtual int get_w() = 0;
	virtual int get_h() = 0;
	virtual bool NotTransparent() = 0;

	// set the position
	virtual void set_x(int posx) = 0;
	virtual void set_y(int posy) = 0;

	// paint
	virtual void paint(XInfo &xinfo) = 0;
};

#endif
