#include "HarvbotAccelerometer.h"

#include <sstream>
#include <climits>
#include <iostream>

HarvbotAccelerometer::HarvbotAccelerometer()
{
}

HarvbotAccelerometer::~HarvbotAccelerometer()
{
}

void HarvbotAccelerometer::setScale(Scale scale) {
	uint8_t old = readRegister(DATA_FORMAT);
	uint8_t bits = scale | (old & (~0x3));
	writeRegister(DATA_FORMAT, bits);
	this->scale = scale;
}

Scale HarvbotAccelerometer::getScale() {
	return scale;
}

void HarvbotAccelerometer::setRate(Rate rate) {
	uint8_t reg = readRegister(BW_RATE);
	reg &= ~0xf;
	reg |= rate;
	writeRegister(BW_RATE, reg);
}

Rate HarvbotAccelerometer::getRate() {
	uint8_t reg = readRegister(BW_RATE);
	return Rate(reg & 0xf);
}

void HarvbotAccelerometer::setPowerMode(PowerMode mode) {
	uint8_t reg = readRegister(BW_RATE);
	const uint8_t bit = 0x1 << 4;
	switch (mode) {
	case LowPower: reg |= bit; break;
	case NormalPower: reg &= ~bit; break;
	}
	writeRegister(BW_RATE, reg);
}

PowerMode HarvbotAccelerometer::getPowerMode() {
	uint8_t reg = readRegister(BW_RATE);
	const uint8_t bit = 0x1 << 4;
	return PowerMode(reg & bit);
}

void HarvbotAccelerometer::zeroOffsets() {
	uint8_t zeros[3] = { 0x29, 0x0E , 0x81 };
	writeRegisters(OFSX, zeros, sizeof(zeros));
}

void HarvbotAccelerometer::setFIFO(FIFOMode mode) {
	uint8_t reg = readRegister(FIFO_CTL);
	reg &= ~(0x3 << 6);
	reg |= mode << 6;
	writeRegister(FIFO_CTL, reg);
}

uint8_t HarvbotAccelerometer::getFIFOBytes() {
	uint8_t reg = readRegister(FIFO_CTL);
	return reg & 0x1f;
}

void HarvbotAccelerometer::initialize(Scale scale) {

	// Check the identity of the device
	uint8_t reg = readRegister(DEVID);
	/*if (reg != 0xe5) {
		std::stringstream ss;
		ss << "This address doesn't belong to an ADXL345 accelerometer. "
			<< "Wrong device id: "
			<< (int)reg
			<< std::endl;
		fatalError(ss.str());
	}*/

	setActive(false);
	setFIFO(Bypass);
	setScale(scale);
	setPowerMode(NormalPower);
	setRate(RATE_25);
}

int8_t HarvbotAccelerometer::checkoverflow(int32_t value) {
	if (value > SCHAR_MAX) {
		debug("WARN: offset overflow: MAX");
		return SCHAR_MAX;
	}
	else if (value < SCHAR_MIN) {
		debug("WARN: offset overflow: MIN");
		return SCHAR_MIN;
	}
	else                        return (int8_t)value;
}

void HarvbotAccelerometer::initOffset() {
	return; // this is broken
	const float offsetscale = 1.0f / 3.9f; // LSB/mg
	const bool active = getActive();
	zeroOffsets();

	if (!active)
		setActive(true);

	float x = 0.0f, y = 0.0f, z = 0.0f;

	for (size_t i = 0; i < 33; i++)
		readData(); // burn the fifo

	for (size_t i = 0; i < 100; i++) {
		HarvbotAccelData r = readData();
		x += r.x;
		y += r.y;
		z += r.z;
	}
	x /= 100.0f;
	y /= 100.0f;
	z /= 100.0f;

	int32_t off32[3];
	off32[0] = int32_t((-x)*offsetscale);
	off32[1] = int32_t((-y)*offsetscale);
	off32[2] = int32_t((-z)*offsetscale);

	uint8_t buff[3];

	for (size_t i = 0; i < 3; i++) {
		buff[i] = checkoverflow(off32[i]);
	}

	writeRegisters(OFSX, buff, sizeof(buff));

	if (!active)
		setActive(false);
}

bool HarvbotAccelerometer::getActive() {
	uint8_t old = readRegister(POWER_CTL);
	return 0 != (old & (1 << 3));
}

void HarvbotAccelerometer::setActive(bool active) {
	uint8_t reg = readRegister(POWER_CTL);
	const uint8_t bit = 0x1 << 3;

	if (active) {
		reg |= bit;
	}
	else {
		reg &= ~bit;
	}
	writeRegister(POWER_CTL, reg);
}

HarvbotAccelData HarvbotAccelerometer::readData() {
	HarvbotAccelData result;
	result.scale_g = scale;

	uint8_t buff[6];

	size_t read = readRegisters(DATAX0, buff, 6);
	if (read != 6) {
		fatalError("Failed to read all data bytes.");
	}

	// we could use the AccelData.getPtr() if we know the
	// architecture is little endian. But we cant always be sure...
	result.x = ((int)buff[0]) | ((int)buff[1] << 8);
	result.y = ((int)buff[2]) | ((int)buff[3] << 8);
	result.z = ((int)buff[4]) | ((int)buff[5] << 8);

	return result;
}

uint8_t HarvbotAccelerometer::readRegister(uint8_t reg) {
	uint8_t buff = 0;
	int numread = readRegisters(reg, &buff, 1);
	if (numread == 1)
		return buff;
	else {
		fatalError("Failed to read register.");
		return 0;
	}
}

void HarvbotAccelerometer::writeRegister(uint8_t reg, uint8_t value) {
	writeRegisters(reg, &value, 1);
}