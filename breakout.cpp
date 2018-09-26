#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <list>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "breakout.h"
using namespace std;


// ctor
Breakout::Breakout(int FPS, int speed) {
	this->FPS = FPS;
	this->speed = speed;

	this->score = 0;
}

// initialize XInfo
void Breakout::initXInfo(XInfo &xinfo) {
	cout<<"init xinfo"<<endl;
	// create window
	xinfo.display = XOpenDisplay("");
	if (xinfo.display == NULL) {
		cerr << "Failed to open display window." << endl;
		exit (-1);
	}

	xinfo.screennum = DefaultScreen(xinfo.display);
	long background = WhitePixel(xinfo.display, xinfo.screennum);
	long foreground = BlackPixel(xinfo.display, xinfo.screennum);
	xinfo.window = XCreateSimpleWindow(xinfo.display, DefaultRootWindow(xinfo.display),
                            10, 10, WINDOW_WIDTH, WINDOW_HEIGHT, 2, foreground, background);

	int depth = DefaultDepth(xinfo.display, DefaultScreen(xinfo.display));
	xinfo.pixmap = XCreatePixmap(xinfo.display, xinfo.window, 
		WINDOW_WIDTH, WINDOW_HEIGHT, depth);

	// set events to monitor and display window
	XSelectInput(xinfo.display, xinfo.window, ButtonPressMask | KeyPressMask);
	XMapRaised(xinfo.display, xinfo.window);
	XFlush(xinfo.display);

	Colormap colormap;
	XColor lightpink, tomato, palegreen, lightsalmon, lightskyblue, slateblue, plum;
	colormap = DefaultColormap(xinfo.display, xinfo.screennum);
	XAllocNamedColor(xinfo.display, colormap, "tomato", &tomato, &tomato);
	XAllocNamedColor(xinfo.display, colormap, "lightpink", &lightpink, &lightpink);
	XAllocNamedColor(xinfo.display, colormap, "palegreen", &palegreen, &palegreen);
	XAllocNamedColor(xinfo.display, colormap, "lightsalmon", &lightsalmon, &lightsalmon);
	XAllocNamedColor(xinfo.display, colormap, "lightskyblue", &lightskyblue, &lightskyblue);
	XAllocNamedColor(xinfo.display, colormap, "plum", &plum, &plum);
	XAllocNamedColor(xinfo.display, colormap, "slateblue", &slateblue, &slateblue);

	for (int i = 0; i < 7; i++) {
		xinfo.gc[i] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	}

	XFontStruct *font = 
		XLoadQueryFont(xinfo.display, 
		"-adobe-new century schoolbook-bold-r-normal--18-180-75-75-p-149-iso8859-9");
	XGCValues gr_value;
	gr_value.font = font->fid;
	xinfo.gc[7] = XCreateGC(xinfo.display, xinfo.window, GCFont, &gr_value);


	XSetForeground(xinfo.display, xinfo.gc[1], tomato.pixel);
	XSetForeground(xinfo.display, xinfo.gc[2], lightpink.pixel);
	XSetForeground(xinfo.display, xinfo.gc[3], palegreen.pixel);
	XSetForeground(xinfo.display, xinfo.gc[4], lightsalmon.pixel);
	XSetForeground(xinfo.display, xinfo.gc[5], lightskyblue.pixel);
	XSetForeground(xinfo.display, xinfo.gc[6], plum.pixel);
	XSetForeground(xinfo.display, xinfo.gc[7], slateblue.pixel);

	//xinfo.gc[0] = XCreateGC(xinfo.display, xinfo.window, 0, 0);
	XWindowAttributes w;
	XGetWindowAttributes(xinfo.display, xinfo.window, &w);

	// prevent users from resizing the window
	XSizeHints *size_hints;
	size_hints=XAllocSizeHints();

    if(size_hints==NULL)
    {
      fprintf(stderr,"XMallocSizeHints() failed\n");

      exit(1);
    }

    size_hints->flags=USPosition | PAspect | PMinSize | PMaxSize;
    size_hints->min_width = 1280;
    size_hints->min_height = 800;
    size_hints->max_width = 1280;
    size_hints->max_height = 800;

    XSetWMNormalHints(xinfo.display, xinfo.window, size_hints);

	cout<<"finish init"<<endl;

}


// event handler for key press
void Breakout::keyPress(XInfo &xinfo, XEvent &event) {
	KeySym key;
	char text[10];
	int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0);

	// move right
	if ( i == 1 && text[0] == 'd' ) {
		//cout << "Key D is pressed" << endl;
		(this->paddle)->move(False, (this->ball)->ifLaunched());
	}

	// move left
	if ( i == 1 && text[0] == 'a' ) {
		//cout << "Key A is pressed" << endl;
		(this->paddle)->move(True, (this->ball)->ifLaunched());
	}

	// bounce the ball
	if ( i == 1 && text[0] == ' ' ) {
		//cout << "Key Space is pressed" << endl;
		(this->ball)->bounce();
	}

	// quit game
	if ( i == 1 && text[0] == 'q' ) {
		//cout << "Key Q is pressed" << endl;
		XCloseDisplay(xinfo.display);
		exit(0);
	}
}

// event handler for mouse click
void Breakout::mouseClick(XInfo &xinfo, XEvent &event) {
	this->ifstart = True;
	this->game(xinfo);
}


/* initialize blocks, paddle, and the ball
		Draw Welcome Screen. 
		Call startGame() to start the game iff mouse click
*/
void Breakout::welcome(XInfo &xinfo) {
	XEvent event;

	/* initialize blocks, paddle, and the ball */
	// initialize blocks
	const int rows = 5;
	const int cols = 13;
	const int w = 92;	// + 2
	const int h = 62;	// + 2
	const int borderX = 35;
	const int borderY = 40;
	int color = 2;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			// cout << "ROW " << i << " COL " << j << endl;
			(this->dList).push_back(new Block(borderX + j * w, borderY + i * h, color));
		}

		if (color < 6) color++;
		else color = 2;
	}

	// initialize the paddle
	this->paddle = new Paddle(540, 780, 40 + 4 * this->speed);

	// initialize the ball
	this->ball = new Ball(585, 730, 25, 2);


	// Draw welcome screen
	string welcome_msg[] = {"Welcome to the breakout game!",
		"Author: Yushan Liu (20617101)", "Click Anywhere to start the game!", "", "RULES:",
		"Launch the ball: press SPACE", "Move the paddle to the left: press A",
		"Move the paddle to the right: press D", "Quit the game: press Q"};

	// call startGame() when mouse click
	cout << "Wait for mouse click" << endl;
	bool mouseClicked = false;
	while (!mouseClicked) {
		//XClearWindow(xinfo.display, xinfo.window);
		if (XPending(xinfo.display) > 0) {
			XNextEvent(xinfo.display, &event);

			switch(event.type) {
				// mouse button press
				case ButtonPress:
					cout << "CLICK" << endl;
					mouseClicked = True;
					// mouseClick(xinfo, event);
					break;

				// any key press
				case KeyPress:
					KeySym key;
					char text[10];
					int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0);
					if ( i == 1 && text[0] == 'q' ) {
						cout << "Key Q is pressed" << endl;
						XCloseDisplay(xinfo.display);
						exit(0);
					}
					break;
			}
		}

		int stringStart = 250;
		for (const string &msg : welcome_msg) {
			XDrawString(xinfo.display, xinfo.window, xinfo.gc[7], 400, stringStart, 
			msg.c_str(), msg.length());
			stringStart += 30;
		}

		usleep(5000);
	}

	// reach here only if mouse click event happens
	XClearWindow(xinfo.display, xinfo.window);
	this->game(xinfo);
}


/* start the game, end iff gameover or press "q"
		gameover: prompt and ask if user wants to play again or quit
		"q" is pressed: quit the game
*/
void Breakout::game(XInfo &xinfo) {
	//cout << "Game in progress" << endl;

	// save time of last window paint
	unsigned long lastRepaint = 0;

	// gameover state: 0 win, 1 lose
	int gameoverState = -1;

	/* Event loop:
	 * space to launch the ball. No action if this->ifstart is True
	 * left and right arrow to move the paddle. No action if this->ifstart is False
	 * q to quit the game */
	 XEvent event;
	 while (True) {
	 	
		if (XPending(xinfo.display) > 0) {
			XNextEvent(xinfo.display, &event);

			switch(event.type) {
				// any key press
				case KeyPress:
					keyPress(xinfo, event);
					break;
			}
		}

		unsigned long end = now();

		int speedUse = this->speed * 3;

		//if (end - lastRepaint > (1000000 / speedUse) / 2) {
		int result = (this->ball)->move(this->dList, this->paddle);
		if (result == 1) {
			// hit the paddle
			this->score += 10;
		} if (result == 2) {
			// gameover, lost
			gameoverState = 1;
			break;
		} else if (result == 3) {
			// gameover, win
			gameoverState = 0;
			break;
		}
		//}

		if (end - lastRepaint > (1000000 / this->FPS)) {
			// clear background
			//XClearWindow(xinfo.display, xinfo.window);
			XSetForeground(xinfo.display, xinfo.gc[0], 
				WhitePixel(xinfo.display, DefaultScreen(xinfo.display)));
			XFillRectangle(xinfo.display, xinfo.pixmap, xinfo.gc[0], 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

			// paint
			this->paint(xinfo);

			lastRepaint = now();
		}

		// IMPORTANT: sleep for a bit to let other processes work
		if (XPending(xinfo.display) == 0) {
			usleep(100000 / speedUse);
			//usleep(1000000 / this->FPS - (now() - lastRepaint));
			//usleep((1000000 / speedUse - (now() - lastRepaint)) / 2);
		}
	}

	gameover(xinfo, gameoverState);
}


/* Paint the blocks, the paddle and the ball */
void Breakout::paint(XInfo &xinfo) {
	XSetForeground(xinfo.display, xinfo.gc[0], 
				BlackPixel(xinfo.display, DefaultScreen(xinfo.display)));

	// draw the blocks
	list<Displayable*>::const_iterator begin = (this->dList).begin();
	list<Displayable*>::const_iterator end = (this->dList).end();

	while(begin != end) {
		Displayable* d = *begin;
		d->paint(xinfo);
		begin++;
	}

	(this->paddle)->paint(xinfo);

	(this->ball)->paint(xinfo);

	string scoreMsg1 = "Score: ";
	scoreMsg1 += to_string(this->score);
	XDrawString(xinfo.display, xinfo.pixmap, xinfo.gc[7], 40, 30, 
			scoreMsg1.c_str(), scoreMsg1.length());

	XCopyArea(xinfo.display, xinfo.pixmap, xinfo.window, xinfo.gc[0], 
		0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, 0);
	XFlush(xinfo.display);
}


/* game over: ask if user wants to restart */
void Breakout::gameover(XInfo &xinfo, int state) {
	XClearWindow(xinfo.display, xinfo.window);

	XEvent event;

	// set the blocks to NotTransparent
	list<Displayable*>::const_iterator begin = dList.begin();
	list<Displayable*>::const_iterator end = dList.end();

	while(begin != end) {
		Block *d = dynamic_cast<Block*>(*begin);
		d->setNonTransparent();
		begin++;
	}

	// re-position the ball and the paddle
	paddle->set_x(540);
	paddle->set_y(770);
	ball->set_x(585);
	ball->set_y(720);

	ball->set_dirX(-1);
	ball->set_dirY(-2);

	ball->setNotLaunched();

	string scoreMsg = "Your score is ";
	scoreMsg += to_string(this->score);

	// Draw welcome screen
	string welcome_msg[] = {"You Lost!", scoreMsg,
		"Press 'Y' to restart", "Press 'Q' to leave the game", "", 
		"Recall the Rules:", "Launch the ball: press SPACE", 
		"Move the paddle to the left: press A",
		"Move the paddle to the right: press D"};

	if (state == 0) {
		welcome_msg[0] = "You win!";
	} else {
		this->score = 0;
	}

	// call startGame() when mouse click
	cout << "wait for press Y" << endl;
	bool Ypressed = false;
	while (!Ypressed) {
		//XClearWindow(xinfo.display, xinfo.window);
		if (XPending(xinfo.display) > 0) {
			XNextEvent(xinfo.display, &event);

			switch(event.type) {
				// any key press
				case KeyPress:
					KeySym key;
					char text[10];
					int i = XLookupString( (XKeyEvent*)&event, text, 10, &key, 0);
					if ( i == 1 && text[0] == 'y' ) {
						cout << "Key Y is pressed" << endl;
						Ypressed = true;
					}
					if ( i == 1 && text[0] == 'q' ) {
						cout << "Key Q is pressed" << endl;
						XCloseDisplay(xinfo.display);
						exit(0);
					}
					break;
			}
		}

		int stringStart = 250;
		for (const string &msg : welcome_msg) {
			XDrawString(xinfo.display, xinfo.window, xinfo.gc[7], 400, stringStart, 
			msg.c_str(), msg.length());
			stringStart += 20;
		}

		usleep(5000);
	}

	// reach here only if mouse click event happens
	XClearWindow(xinfo.display, xinfo.window);
	this->game(xinfo);
}


// get current time
unsigned long Breakout::now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}

