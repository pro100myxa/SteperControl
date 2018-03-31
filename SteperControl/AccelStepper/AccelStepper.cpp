// AccelStepper.cpp
//
// Copyright (C) 2009-2013 Mike McCauley
// $Id: AccelStepper.cpp,v 1.20 2015/08/25 02:22:45 mikem Exp mikem $

#include <math.h>
#include "AccelStepper.h"

void AccelStepper::moveTo(long absolute)
{
    if (_targetPos != absolute)
    {
		_direction = (absolute > _targetPos) ? DIRECTION_CW : DIRECTION_CCW;
		_targetPos = absolute;
    }
}

void AccelStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

// Implements steps according to the current step interval
// You must call this at least once per step
// returns true if a step occurred
boolean AccelStepper::runSpeed()
{
    if (distanceToGo() != 0)
    {
		unsigned int time = micros();
		if (time < _lastStepTime + 2 * _minPulseWidth)
		{
			return true;
		}

		_lastStepTime = time;

		if (_direction == DIRECTION_CW)
		{
			// Clockwise
			_currentPos += 1;
		}
		else
		{
			// Anticlockwise  
			_currentPos -= 1;
		}

		step(_currentPos);
		return true;
    }
    else
    {
		return false;
    }
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
// Sets speed to 0
void AccelStepper::setCurrentPosition(long position)
{
    _targetPos = _currentPos = position;
}

// Run the motor to implement speed and acceleration in order to proceed to the target position
// You must call this at least once per step, preferably in your main loop
// If the motor is in the desired position, the cost is very small
// returns true if the motor is still running to the target position.
boolean AccelStepper::run()
{
    if (runSpeed())
    return _speed != 0.0 || distanceToGo() != 0;
}

AccelStepper::AccelStepper(uint8_t stepPin, uint8_t dirPin, bool enable)
{
	_stepPin = stepPin;
	_dirPin = dirPin;
    _currentPos = 0;
    _targetPos = 0;
    _speed = 0.0;
    _maxSpeed = 1.0;
    _minPulseWidth = 1;
    _enablePin = 0xff;
	_lastStepTime = 0;

    // NEW
    _direction = DIRECTION_CCW;

    if (enable)
		enableOutputs();
}

void AccelStepper::setMaxSpeed(float speed)
{
    if (_maxSpeed != speed)
    {
		_maxSpeed = speed;
    }
}

float AccelStepper::maxSpeed()
{
    return _maxSpeed;
}

void AccelStepper::setSpeed(float speed)
{
    if (speed == _speed)
        return;

    speed = constrain(speed, -_maxSpeed, _maxSpeed);

	_direction = (speed > 0.0) ? DIRECTION_CW : DIRECTION_CCW;
    _speed = speed;
}

float AccelStepper::speed()
{
    return _speed;
}

// Subclasses can override
void AccelStepper::step(long step)
{
	digitalWrite(_dirPin, _direction ? HIGH : LOW);
	digitalWrite(_stepPin, HIGH);			 // Set direction first else get rogue pulses
											 // step HIGH
											 // Caution 200ns setup time 
											 // Delay the minimum allowed pulse width
	delayMicroseconds(1);
	digitalWrite(_stepPin, LOW);			 // step LOW
}

// Prevents power consumption on the outputs
void    AccelStepper::disableOutputs()
{   
	digitalWrite(_dirPin, LOW);
	digitalWrite(_stepPin, LOW);
    if (_enablePin != 0xff)
        digitalWrite(_enablePin, LOW ^ _enableInverted);
}

void    AccelStepper::enableOutputs()
{
    pinMode(_dirPin, OUTPUT);
    pinMode(_stepPin, OUTPUT);

    if (_enablePin != 0xff)
    {
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, HIGH ^ _enableInverted);
    }
}

void AccelStepper::setMinPulseWidth(unsigned int minWidth)
{
    _minPulseWidth = minWidth;
}

void AccelStepper::setEnablePin(uint8_t enablePin)
{
    _enablePin = enablePin;

    // This happens after construction, so init pin now.
    if (_enablePin != 0xff)
    {
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, HIGH ^ _enableInverted);
    }
}

// Blocks until the target position is reached and stopped
void AccelStepper::runToPosition()
{
    while (run())
	;
}

boolean AccelStepper::runSpeedToPosition()
{
    if (_targetPos == _currentPos)
	return false;
    if (_targetPos >_currentPos)
	_direction = DIRECTION_CW;
    else
	_direction = DIRECTION_CCW;
    return runSpeed();
}

// Blocks until the new target position is reached
void AccelStepper::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

void AccelStepper::stop()
{
	digitalWrite(_dirPin, LOW);
	digitalWrite(_stepPin, LOW);
}

uint8_t AccelStepper::stepPin()
{
	return _stepPin;
}

uint8_t AccelStepper::directionPin()
{
	return _dirPin;
}