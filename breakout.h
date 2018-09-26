#ifndef __BREAKOUT_H__
#define __BREAKOUT_H__

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <list>
#include <memory>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "displayable.h"
#include "paddle.h"
#include "ball.h"

using namespace std;

struct XInfo;

class Breakout {
private:
	// all displayables
	list<Displayable* > dList;	// blocks only
	Paddle *paddle;
	Ball *ball;

	// A new game is waiting to be started, press Space to start
	bool ifstart;
	// Game over, ask if the user wants to paly again / quit
	//bool gameover;

	int score;		// user score, increment for every bounce of the ball
	int FPS;		// frame-rate entered by the user
	int speed;		// speed of the ball entered by the user

	int lives;		// TODO: user should have 3 lives

public:
	Breakout(int FPS, int speed);

	/* initialize XInfo */
	void initXInfo(XInfo &xinfo);

	/* initialize blocks, paddle, and the ball
		Draw Welcome Screen. 
		Call startGame() to start the game iff Space is pressed
	*/
	void welcome(XInfo &xinfo);
	
	/* start the game, end iff gameover or press "q"
		gameover: prompt and ask if user wants to play again or quit
		"q" is pressed: back to the Welcome screen
	*/
	void game(XInfo &xinfo);

	/* paint the blocks, the padlle, and the ball */
	void paint(XInfo &xinfo);

	/* event handler for key press */
	void keyPress(XInfo &xinfo, XEvent &event);

	/* event handler for mouse click */
	void mouseClick(XInfo &xinfo, XEvent &event);

	/* game over: ask if user wants to restart */
	void gameover(XInfo &xinfo, int state);

	// get current time
	unsigned long now();

};

#endif
