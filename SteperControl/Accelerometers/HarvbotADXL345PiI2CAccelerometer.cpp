#include "HarvbotADXL345PiI2CAccelerometer.h"

HarvbotADXL345PiI2CAccelerometer::HarvbotADXL345PiI2CAccelerometer(const char* device, Scale scale) : HarvbotAccelerometer(scale)
{
	innerAccel = new ADXL345Pi(device);
}

HarvbotADXL345PiI2CAccelerometer::~HarvbotADXL345PiI2CAccelerometer()
{
	delete innerAccel;
}

void HarvbotADXL345PiI2CAccelerometer::setScale(Scale scale)
{
	innerAccel->setRangeSetting(scale);
	HarvbotAccelerometer::setScale(scale);
}

void HarvbotADXL345PiI2CAccelerometer::setRate(Rate rate)
{
	innerAccel->setRate(rate);
}

Rate HarvbotADXL345PiI2CAccelerometer::getRate()
{
	return (Rate)innerAccel->getRate();
}

void HarvbotADXL345PiI2CAccelerometer::setPowerMode(PowerMode mode)
{
	innerAccel->setLowPower(mode == LowPower);
}

PowerMode HarvbotADXL345PiI2CAccelerometer::getPowerMode()
{
	return innerAccel->isLowPower() ? LowPower : NormalPower;
}

bool HarvbotADXL345PiI2CAccelerometer::getActive()
{
	innerAccel->isActivityAc();
}

void HarvbotADXL345PiI2CAccelerometer::setActive(bool active)
{
	innerAccel->setActivityAc(active);
}

void HarvbotADXL345PiI2CAccelerometer::zeroOffsets()
{
	innerAccel->setAxisOffset(0, 0, 0);
}

void HarvbotADXL345PiI2CAccelerometer::initOffset()
{
	zeroOffsets();
}

HarvbotAccelData HarvbotADXL345PiI2CAccelerometer::readData()
{
	int x = 0, y = 0, z = 0;
	innerAccel->readAccel(&x, &y, &z);

	HarvbotAccelData data;
	data.x = x;
	data.y = y;
	data.z = z;
	return data;
}
