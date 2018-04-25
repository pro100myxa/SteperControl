#pragma once
#include <stdint.h>
class HarvbotRangefinder
{
public:
	HarvbotRangefinder();
	~HarvbotRangefinder();

	virtual void powerOn() = 0;

	virtual void powerOff() = 0;

	virtual void startContinuous(uint32_t periodMilliseconds = 0) = 0;

	virtual void stopContinuous() = 0;

	virtual uint16_t readRangeSingleMillimeters() = 0;

	virtual uint16_t readRangeContinuousMillimeters() = 0;
};

