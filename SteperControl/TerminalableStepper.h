#pragma once
#include <wiringPi.h>
#include <pthread.h>
#include "AccelStepper/AccelStepper.h"

class TerminalableStepper : public AccelStepper
{
public:
	TerminalableStepper(int stepPin, int dirPin, int terminalPin = 0);
	
	void moveTillTerminal(bool direction);

	bool isTerminated();
protected:
	void step(long step);
	
private:

	// Stores terminal pin.
	int _terminalPin;

	// Stores stepper direction in time of terminal fire.
	bool _terminalPressedDir;

	// Stores value indicating where terminal was pressed.
	bool _terminalPressed;
};

