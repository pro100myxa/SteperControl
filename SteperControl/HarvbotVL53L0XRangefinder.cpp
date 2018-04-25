#include "HarvbotVL53L0XRangefinder.h"

HarvbotVL53L0XRangefinder::HarvbotVL53L0XRangefinder(const char* device = "/dev/i2c-1")
{
	innerRangefinder = new VL53L0XPi(device);
}

HarvbotVL53L0XRangefinder::~HarvbotVL53L0XRangefinder()
{
	delete innerRangefinder;
}

void HarvbotVL53L0XRangefinder::powerOn()
{
	innerRangefinder->powerOn();
}

void HarvbotVL53L0XRangefinder::powerOff()
{
	innerRangefinder->powerOff();
}

void HarvbotVL53L0XRangefinder::startContinuous(uint32_t periodMilliseconds)
{
	innerRangefinder->startContinuous(periodMilliseconds);
}

void HarvbotVL53L0XRangefinder::stopContinuous()
{
	innerRangefinder->stopContinuous();
}

uint16_t HarvbotVL53L0XRangefinder::readRangeSingleMillimeters()
{
	return innerRangefinder->readRangeSingleMillimeters();
}

uint16_t HarvbotVL53L0XRangefinder::readRangeContinuousMillimeters()
{
	return innerRangefinder->readRangeContinuousMillimeters();
}
