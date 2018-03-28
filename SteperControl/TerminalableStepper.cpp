#include <wiringPi.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>
#include "TerminalableStepper.h"

TerminalableStepper::TerminalableStepper(int stepPin, int dirPin, int terminalPin) : AccelStepper(stepPin, dirPin)
{
	_terminalPin = terminalPin;

	if (_terminalPin > 0)
	{
		pinMode(_terminalPin, INPUT);
	}
}

void TerminalableStepper::step(long step)
{
	if (_terminalPin != 0)
	{
		int endVal = digitalRead(_terminalPin);
		if (endVal == LOW)
		{
			if (_terminalPressed)
			{
				if (_terminalPressedDir == (int)(speed() > 0))
				{
					printf("Ender blocked: %d\n", _terminalPin);
					return;
				}
			}
			else
			{
				// Stop movement
				moveTo(currentPosition());
				_terminalPressed = true;
				_terminalPressedDir = (int)(speed() > 0);
				printf("Ender fire: %d\n", _terminalPin);
				return;
			}

		}
		else
		{
			_terminalPressed = false;
		}
	}

	AccelStepper::step(step);
	printf("Step %d was done\n", step);
}

void TerminalableStepper::moveTillTerminal(bool direction)
{
	do 
	{
		move(direction ? 1 : -1);
	}
	while (run);
}