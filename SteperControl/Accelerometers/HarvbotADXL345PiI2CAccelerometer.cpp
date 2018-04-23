#include "HarvbotADXL345PiI2CAccelerometer.h"

HarvbotADXL345PiI2CAccelerometer::HarvbotADXL345PiI2CAccelerometer(Scale scale)
{
	handle = wiringPiI2CSetup(ADXL345_DEVADDR);
	initialize(scale);
}

HarvbotADXL345PiI2CAccelerometer::~HarvbotADXL345PiI2CAccelerometer()
{
}

size_t HarvbotADXL345PiI2CAccelerometer::readRegisters(uint8_t reg, uint8_t* buff, size_t size) {
	for (int i = 0; i < size; i++)
	{
		buff[i] = wiringPiI2CReadReg8(handle, reg);
	}
	return size;
}

void HarvbotADXL345PiI2CAccelerometer::writeRegisters(uint8_t reg, uint8_t* buff, size_t size) {
	for (int i = 0; i < size; i++)
	{
		wiringPiI2CWriteReg8(handle, reg, buff[i]);
	}
}

void HarvbotADXL345PiI2CAccelerometer::fatalError(std::string error) {
	throw error;
}

void HarvbotADXL345PiI2CAccelerometer::debug(std::string msg) {
	std::cout << "DEBUG: " << msg << std::endl;
}