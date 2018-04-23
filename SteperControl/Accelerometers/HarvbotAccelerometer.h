#pragma once
#include "HarvbotAccelData.h"
class HarvbotAccelerometer
{
public:
	HarvbotAccelerometer();
	~HarvbotAccelerometer();

	void setScale(Scale scale);
	Scale getScale();

	void setRate(Rate rate);
	Rate getRate();

	void setFIFO(FIFOMode mode);
	uint8_t getFIFOBytes();

	void setPowerMode(PowerMode mode);
	PowerMode getPowerMode();

	bool getActive();
	void setActive(bool active);

	void zeroOffsets();
	void initOffset();

	virtual HarvbotAccelData readData();

	uint8_t readRegister(uint8_t register);
	void writeRegister(uint8_t reg, uint8_t value);

protected:
	void initialize(Scale scale = Scale_FullRes);

private:
	Scale scale;

	int8_t checkoverflow(int32_t value);

	virtual size_t readRegisters(uint8_t startreg, uint8_t* buff, size_t size) = 0;
	virtual void writeRegisters(uint8_t reg, uint8_t* buff, size_t size) = 0;
	virtual void fatalError(std::string error) = 0;
	virtual void debug(std::string msg) = 0;
};

