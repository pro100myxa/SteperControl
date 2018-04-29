#ifndef HarvbotStepper_h
#define HarvbotStepper_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#define TIMER_FREQUENCY 1000000.0
#else
#include <wiringPi.h>
#include <stdint.h>
#define TIMER_FREQUENCY 700.0
#include <iostream>

template<class T>
const T& constrain(const T& x, const T& a, const T& b) {
	if (x < a) {
		return a;
	}
	else if (b < x) {
		return b;
	}
	else
		return x;
}
#endif

// These defs cause trouble on some versions of Arduino
#undef round

class HarvbotStepper
{
public:
	HarvbotStepper(uint8_t stepPin = 2, uint8_t dirPin = 3, unsigned int frequency = 1000, bool enable = true);

    void    moveTo(long absolute); 

    void    move(long relative);

    bool run();

    bool runSpeed();

    void    setEngineFrequency(unsigned int frequency);

	unsigned int   engineFrequency();

    long    distanceToGo();

    long    targetPosition();

    long    currentPosition();  

    void    setCurrentPosition(long position);  
    
    void    runToPosition();

    bool runSpeedToPosition();

    void    runToNewPosition(long position);

    void stop();

    virtual void    disableOutputs();

    virtual void    enableOutputs();

    void setPulsePeriod(unsigned int waitInterval);

	unsigned int pulsePeriod();

    void    setEnablePin(uint8_t enablePin = 0xff);

	bool direction();

	uint8_t stepPin();

	uint8_t directionPin();

	unsigned int accelerationPercent();

	void setAccelerationPercent(unsigned int percent);
protected:

    typedef enum
    {
		DIRECTION_CCW = false,  ///< Clockwise
        DIRECTION_CW  = true   ///< Counter-Clockwise
    } Direction;

    virtual void   step(long step);

private:
    uint8_t        _stepPin;

	uint8_t        _dirPin;

    long           _currentPos;    // Steps

    long           _targetPos;     // Steps

	long           _startPos;     // Steps

    unsigned int   _engineFrequency;

    unsigned int   _pulsePeriod;

    bool           _enableInverted;

    uint8_t        _enablePin;

    bool _direction; // 1 == CW

	unsigned int _lastStepTime;

	unsigned int _accelerationPercent;
};

#endif 
