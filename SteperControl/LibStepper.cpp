#include "LibStepper.h"
#include <wiringPi.h>
#include <math.h>
#include <stdio.h>

LibStepper::LibStepper(int step_pin, int dir_pin, int end_pin)
{
	_currentPos = 0;
	_targetPos = 0;
	_speed = 0;
	_step = step_pin;
	_dir = dir_pin;
	_end = end_pin;
}

void LibStepper::speed(int set_speed)
{
	_speed = set_speed;
}

void LibStepper::moveTo(int position)
{
	if (position == 0)
	{
		return;
	}

	_targetPos = position;
	if (_targetPos < 0)
	{
		//_targetPos = ~_targetPos + 1;
		digitalWrite(_dir, LOW);
	}
	else digitalWrite(_dir, HIGH);

	int i = 0;
	while (i < fabs(_targetPos))
	{
		if (_end != 0)
		{
			int endVal = digitalRead(_end);
			if (endVal == LOW)
			{
				if (_endPressed)
				{
					if (_endDir == digitalRead(_dir))
					{
						printf("Ender blocked: %d\n", _end);
						return;
					}
				}
				else
				{
					_endPressed = true;
					_endDir = digitalRead(_dir);
					printf("Ender fire: %d\n", _end);
					return;
				}
				
			}
			else
			{
				_endPressed = false;
			}
		}

		i++;
		digitalWrite(_step, HIGH);
		delayMicroseconds(_speed);
		digitalWrite(_step, LOW);
		delayMicroseconds(_speed);
	}
}





