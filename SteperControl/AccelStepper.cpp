// AccelStepper.cpp
//
// Copyright (C) 2009 Mike McCauley
// $Id: AccelStepper.cpp,v 1.2 2010/10/24 07:46:18 mikem Exp mikem $

#include "AccelStepper.h"
#include <wiringPi.h>   ////Transforms Odroid into a very fast Arduino
#include <math.h>


void AccelStepper::moveTo(long absolute)
{
	_targetPos = absolute;
	computeNewSpeed();
}

void AccelStepper::move(long relative)
{
	moveTo(_currentPos + relative);
}

// Implements steps according to the current speed
// You must call this at least once per step
// returns true if a step occurred
bool AccelStepper::runSpeed()
{
	unsigned long time = millis();

	if (time > _lastStepTime + _stepInterval)
	{
		if (_speed > 0)
		{
			// Clockwise
			_currentPos += 1;
		}
		else if (_speed < 0)
		{
			// Anticlockwise  
			_currentPos -= 1;
		}
		step1(_currentPos & 0x3); // Bottom 2 bits (same as mod 4, but works with + and - numbers) 

		_lastStepTime = time;
		return true;
	}
	else
		return false;
}

long AccelStepper::distanceToGo()
{
	return _targetPos - _currentPos;
}

long AccelStepper::targetPosition()
{
	return _targetPos;
}

long AccelStepper::currentPosition()
{
	return _currentPos;
}

// Useful during initialisations or after initial positioning
void AccelStepper::setCurrentPosition(long position)
{
	_currentPos = position;
}

void AccelStepper::computeNewSpeed()
{
	setSpeed(desiredSpeed());
}

// Work out and return a new speed.
// Subclasses can override if they want
// Implement acceleration, deceleration and max speed
// Negative speed is anticlockwise
// This is called:
//  after each step
//  after user changes:
//   maxSpeed
//   acceleration
//   target position (relative or absolute)
float AccelStepper::desiredSpeed()
{
	long distanceTo = distanceToGo();

	// Max possible speed that can still decelerate in the available distance
	float requiredSpeed;
	if (distanceTo == 0)
		return 0.0; // Were there
	else if (distanceTo > 0) // Clockwise
		requiredSpeed = sqrt(2.0 * distanceTo * _acceleration);
	else  // Anticlockwise
		requiredSpeed = -sqrt(2.0 * -distanceTo * _acceleration);

	if (requiredSpeed > _speed)
	{
		// Need to accelerate in clockwise direction
		if (_speed == 0)
			requiredSpeed = sqrt(2.0 * _acceleration);
		else
			requiredSpeed = _speed + fabs(_acceleration / _speed); // fabs - Функция вычисляет абсолютное значение и возвращает модуль значения val (|val|).
		if (requiredSpeed > _maxSpeed)
			requiredSpeed = _maxSpeed;
	}
	else if (requiredSpeed < _speed)
	{
		// Need to accelerate in anticlockwise direction
		if (_speed == 0)
			requiredSpeed = -sqrt(2.0 * _acceleration);
		else
			requiredSpeed = _speed - fabs(_acceleration / _speed);
		if (requiredSpeed < -_maxSpeed)
			requiredSpeed = -_maxSpeed;
	}
	//  Serial.println(requiredSpeed);
	return requiredSpeed;
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if we are still running to position
bool AccelStepper::run()
{
	if (_targetPos == _currentPos)
		return false;

	if (runSpeed())
		computeNewSpeed();
	return true;
}

AccelStepper::AccelStepper(int pin1, int pin2)
{
	_currentPos = 0;
	_targetPos = 0;
	_speed = 0.0;
	_maxSpeed = 1.0;
	_acceleration = 1.0;
	_stepInterval = 0;
	_lastStepTime = 0;
	_pin1 = pin1;
	_pin2 = pin2;
	
}




void AccelStepper::setMaxSpeed(float speed)
{
	_maxSpeed = speed;
	computeNewSpeed();
}

void AccelStepper::setAcceleration(float acceleration)
{
	_acceleration = acceleration;
	computeNewSpeed();
}

void AccelStepper::setSpeed(float speed)
{
	_speed = speed;
	_stepInterval = fabs(1000.0 / _speed);
}

float AccelStepper::speed()
{
	return _speed;
}




// 1 pin step function (ie for stepper drivers)
// This is passed the current step number (0 to 3)
// Subclasses can override
void AccelStepper::step1(int step)
{
	digitalWrite(_pin2, _speed > 0); // Direction
									 // Caution 200ns setup time 
	digitalWrite(_pin1, HIGH);
	// Caution, min Step pulse width for 3967 is 1microsec
	// Delay 1microsec
	delayMicroseconds(1);
	digitalWrite(_pin1, LOW);
}


// Blocks until the target position is reached
void AccelStepper::runToPosition()
{
	while (run())
		;
}

bool AccelStepper::runSpeedToPosition()
{
	return _targetPos != _currentPos ? AccelStepper::runSpeed() : false;
}

// Blocks until the new target position is reached
void AccelStepper::runToNewPosition(long position)
{
	moveTo(position);
	runToPosition();
}

