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
				if (_terminalPressedDir == direction())
				{
					printf("Stepper %d: Ender blocked: %d\n", stepPin(), _terminalPin);
					return;
				}
			}
			else
			{
				// Stop movement
				moveTo(currentPosition());
				_terminalPressed = true;
				_terminalPressedDir = direction();
				printf("Stepper %d: Ender fire: %d\n", stepPin(), _terminalPin);
				return;
			}

		}
		else
		{
			_terminalPressed = false;
		}
	}

	AccelStepper::step(step);
	printf("Stepper %d: Step %d was done\n", stepPin(), step);
}

bool TerminalableStepper::isTerminated()
{
	return _terminalPressed;
}

void TerminalableStepper::moveTillTerminal(bool direction)
{
	do 
	{
		move(direction ? 1 : -1);
		run();
	}
	while (!isTerminated());
}