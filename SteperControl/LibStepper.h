#pragma once
#include <wiringPi.h>
class LibStepper
{
public:
	LibStepper(int step_pin, int dir_pin);
	
	void speed(int set_speed);
	
	void moveTo(int position);

protected:

	

private:

	int	_currentPos;

	int	_targetPos;

	int	_speed;

	int	_step;

	bool _dir;



};

