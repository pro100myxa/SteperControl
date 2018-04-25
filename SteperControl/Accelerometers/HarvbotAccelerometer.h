#pragma once
#include "HarvbotAccelData.h"

class HarvbotAccelerometer
{
public:
	HarvbotAccelerometer(Scale scale = Scale_FullRes);
	~HarvbotAccelerometer();

	virtual void setScale(Scale scale);

	virtual Scale getScale();

	virtual void setRate(Rate rate) {};

	virtual Rate getRate() = 0;

	virtual void setPowerMode(PowerMode mode) {};

	virtual PowerMode getPowerMode() = 0;

	virtual bool getActive() = 0;

	virtual void setActive(bool active) {};

	virtual void zeroOffsets() = 0;

	virtual void initOffset() = 0;

	virtual HarvbotAccelData readData() = 0;

private:
	Scale scale;
};

