#pragma once
#include <wiringPi.h>
class LibStepper
{
public:
	LibStepper(int step_pin, int dir_pin, int end_pin = 0);
	
	void speed(int set_speed);
	
	void moveTo(int position);

protected:

	

private:

	int	_currentPos;

	int	_targetPos;

	int	_speed;

	int	_step;

	int _dir;

	int _end;

	int _endDir;

	bool _endPressed;

	unsigned long _lastStepTime;
};

