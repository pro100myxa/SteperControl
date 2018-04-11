#include <math.h>
#include "HarvbotStepper.h"

HarvbotStepper::HarvbotStepper(uint8_t stepPin, uint8_t dirPin, unsigned int frequency, bool enable)
{
	_stepPin = stepPin;
	_dirPin = dirPin;
	_currentPos = 0;
	_targetPos = 0;
	setEngineFrequency(frequency);
	_enablePin = 0xff;
	_lastStepTime = 0;

	// NEW
	_direction = DIRECTION_CCW;

	if (enable)
		enableOutputs();
}

void HarvbotStepper::moveTo(long absolute)
{
    if (_targetPos != absolute)
    {
		_direction = (absolute > _targetPos) ? DIRECTION_CW : DIRECTION_CCW;

		unsigned int time = 0;
		do
		{
			time = micros();
		} while (time < _lastStepTime + _pulsePeriod);

		_targetPos = absolute;
    }
}

void HarvbotStepper::move(long relative)
{
    moveTo(_currentPos + relative);
}

bool HarvbotStepper::runSpeed()
{
    if (distanceToGo() != 0)
    {
		unsigned int time = micros();
		if (time < _lastStepTime + _pulsePeriod)
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

long HarvbotStepper::distanceToGo()
{
    return _targetPos - _currentPos;
}

long HarvbotStepper::targetPosition()
{
    return _targetPos;
}

long HarvbotStepper::currentPosition()
{
    return _currentPos;
}

void HarvbotStepper::setCurrentPosition(long position)
{
    _targetPos = _currentPos = position;
}

bool HarvbotStepper::run()
{
    if (runSpeed())
    return distanceToGo() != 0;
}

void HarvbotStepper::setEngineFrequency(unsigned int frequency)
{
	_engineFrequency = frequency;
	_pulsePeriod = 1000000 / frequency;
}

unsigned int HarvbotStepper::engineFrequency()
{
	return _engineFrequency;
}

void HarvbotStepper::setPulsePeriod(unsigned int waitInterval)
{
	if (_engineFrequency < 1000000 / _engineFrequency)
	{
		_pulsePeriod = 1000000 / _engineFrequency;
	}
	else
	{
		_pulsePeriod = waitInterval;
	}
}

unsigned int HarvbotStepper::pulsePeriod()
{
	return _pulsePeriod;
}

bool HarvbotStepper::direction()
{
	return _direction;
}

// Subclasses can override
void HarvbotStepper::step(long step)
{
	digitalWrite(_dirPin, _direction ? HIGH : LOW);
	digitalWrite(_stepPin, HIGH);			 // Set direction first else get rogue pulses
	delayMicroseconds(1);
	digitalWrite(_stepPin, LOW);			 // step LOW
}

// Prevents power consumption on the outputs
void HarvbotStepper::disableOutputs()
{   
	digitalWrite(_dirPin, LOW);
	digitalWrite(_stepPin, LOW);
    if (_enablePin != 0xff)
        digitalWrite(_enablePin, LOW ^ _enableInverted);
}

void HarvbotStepper::enableOutputs()
{
    pinMode(_dirPin, OUTPUT);
    pinMode(_stepPin, OUTPUT);

    if (_enablePin != 0xff)
    {
        pinMode(_enablePin, OUTPUT);
        digitalWrite(_enablePin, HIGH ^ _enableInverted);
    }
}

void HarvbotStepper::setEnablePin(uint8_t enablePin)
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
void HarvbotStepper::runToPosition()
{
    while (run())
	;
}

bool HarvbotStepper::runSpeedToPosition()
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
void HarvbotStepper::runToNewPosition(long position)
{
    moveTo(position);
    runToPosition();
}

void HarvbotStepper::stop()
{
	_targetPos = _currentPos;
	digitalWrite(_dirPin, LOW);
	digitalWrite(_stepPin, LOW);
}

uint8_t HarvbotStepper::stepPin()
{
	return _stepPin;
}

uint8_t HarvbotStepper::directionPin()
{
	return _dirPin;
}