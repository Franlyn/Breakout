#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cstdlib>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "displayable.h"
#include "breakout.h"

using namespace std;

struct XInfo;


int main(int argc, char *argv[]) {
	if (argc != 3) {
		cerr << "Error: Wrong number of argument." << endl;
		cerr << "The argument should be frame-rate and speed" << endl;
	}

	int FPS = atoi(argv[1]);
	int speed = atoi(argv[2]);

	Breakout *breakout = new Breakout(FPS, speed);

	XInfo xinfo;

	breakout->initXInfo(xinfo);

	// start the game
	breakout->welcome(xinfo);
}