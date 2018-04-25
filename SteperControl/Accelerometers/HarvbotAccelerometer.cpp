#include "HarvbotAccelerometer.h"

#include <sstream>
#include <climits>
#include <iostream>

HarvbotAccelerometer::HarvbotAccelerometer(Scale scale)
{
	setActive(false);
	setScale(scale);
	setPowerMode(NormalPower);
	setRate(RATE_25);
}

HarvbotAccelerometer::~HarvbotAccelerometer()
{
}

void HarvbotAccelerometer::setScale(Scale scale)
{
	this->scale = scale;
}

Scale HarvbotAccelerometer::getScale()
{
	return this->scale;
}