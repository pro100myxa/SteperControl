#include "LibStepper.h"
#include <wiringPi.h>
#include <math.h>
#include <stdio.h>
#include <pthread.h>

LibStepper::LibStepper(int step_pin, int dir_pin, int terminal_pin) : AccelStepper(AccelStepper::DRIVER, step_pin, dir_pin)
{
	_terminalPin = terminal_pin;

	if (_terminalPin > 0)
	{
		pinMode(_terminalPin, INPUT);
	}
}

void LibStepper::step(long step)
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
	printf("Step was done\n");
}