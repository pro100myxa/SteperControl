#pragma once
#include "HarvbotRangefinder.h"
#include "VL53L0XPi.hpp"

class HarvbotVL53L0XRangefinder
{
public:
	HarvbotVL53L0XRangefinder(const char* device="/dev/i2c-1");
	~HarvbotVL53L0XRangefinder();

	void powerOn();

	void powerOff();

	void startContinuous(uint32_t periodMilliseconds = 0);

	void stopContinuous();

	uint16_t readRangeSingleMillimeters();

	uint16_t readRangeContinuousMillimeters();
private:
	VL53L0XPi * innerRangefinder;
};

