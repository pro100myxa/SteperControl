#include "LibStepper.h"
#include <wiringPi.h>


LibStepper::LibStepper(int step_pin, int dir_pin)
{
	_currentPos = 0;
	_targetPos = 0;
	_speed = 0;
	_step = step_pin;
	_dir = dir_pin;
}

void LibStepper::speed(int set_speed)
{
	_speed = set_speed;
}

void LibStepper::moveTo(int position)
{
	_targetPos = position;
	if (_targetPos < 0)
	{
		_targetPos = ~_targetPos + 1;
		digitalWrite(_dir, LOW);
		
	}
	else digitalWrite(_dir, HIGH);

	int i = 0;
	while (i < _targetPos)
	{

		i++;
		digitalWrite(_step, HIGH);
		delayMicroseconds(_speed);
		digitalWrite(_step, LOW);
		delayMicroseconds(_speed);
	}
}





