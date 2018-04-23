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

#define ADXL345_DEVADDR 0x53    // I2C address

class HarvbotADXL345PiI2CAccelerometer : public HarvbotAccelerometer {
public:
	HarvbotADXL345PiI2CAccelerometer(Scale scale = Scale_8G);
	virtual ~HarvbotADXL345PiI2CAccelerometer();

private:
	int handle;
	virtual size_t readRegisters(uint8_t startreg, uint8_t* buff, size_t size);
	virtual void writeRegisters(uint8_t reg, uint8_t* buff, size_t size);
	virtual void fatalError(std::string error);
	virtual void debug(std::string msg);
};
