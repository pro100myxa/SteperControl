#pragma once
#include <wiringPi.h>
#include <pthread.h>
#include "AccelStepper/AccelStepper.h"

class TerminalableStepper : public AccelStepper
{
public:
	TerminalableStepper(int step_pin, int dir_pin, int terminal_pin = 0);

protected:
	void step(long step);
	
private:

	// Stores terminal pin.
	int _terminalPin;

	// Stores stepper direction in time of terminal fire.
	int _terminalPressedDir;

	// Stores value indicating where terminal was pressed.
	bool _terminalPressed;
};

