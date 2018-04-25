#pragma once

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <string>
#include <sstream>
#include <iostream>
#include <wiringPiI2C.h>

#include "HarvbotAccelData.h"
#include "HarvbotAccelerometer.h"
#include "HarvbotADXL345PiI2CAccelerometer.h"
#include "../ADXL345Pi.h"

class HarvbotADXL345PiI2CAccelerometer : public HarvbotAccelerometer {
public:
	HarvbotADXL345PiI2CAccelerometer(const char* device = "/dev/i2c-1", Scale scale = Scale_8G);
	~HarvbotADXL345PiI2CAccelerometer();

	void setScale(Scale scale);

	void setRate(Rate rate);
	Rate getRate();

	void setPowerMode(PowerMode mode);
	PowerMode getPowerMode();

	bool getActive();
	void setActive(bool active);

	void zeroOffsets();
	void initOffset();

	HarvbotAccelData readData();
private:
	ADXL345Pi* innerAccel;
};
