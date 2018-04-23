#pragma once
#include <string>
#include <stddef.h>
#include <stdint.h>

enum PowerMode {
	NormalPower = 0x0,
	LowPower = 0x1 << 4,
};

enum Scale {
	Scale_2G = 0x0,
	Scale_4G = 0x1,
	Scale_8G = 0x2,
	Scale_16G = 0x3,
	Scale_FullRes = 0xB
};

enum FIFOMode {
	Bypass = 0x0,
	FIFO = 0x1,
	Stream = 0x2,
	Trigger = 0x3
};

enum {
	DEVID = 0x00,  // readonly
	OFSX = 0x1e,
	OFSY = 0x1f,
	OFSZ = 0x20,
	THRESH_ACT = 0x24,
	THRESH_INACT = 0x25,
	TIME_INACT = 0x26,
	ACT_INACT_CTL = 0x27,
	ACT_TAP_STATUS = 0x2b,        // readonly
	BW_RATE = 0x2c,
	POWER_CTL = 0x2d,
	INT_ENABLE = 0x2e,
	INT_MAP = 0x2f,
	INT_SOURCE = 0x30,   // readonly
	DATA_FORMAT = 0x31,
	DATAX0 = 0x32,
	DATAX1 = 0x33,
	DATAY0 = 0x34,
	DATAY1 = 0x35,
	DATAZ0 = 0x36,
	DATAZ1 = 0x37,
	FIFO_CTL = 0x38,
	FIFO_STATUS = 0x39
};

enum IntEnable {
	DATA_READY = 0x1 << 7,
	SIGNLE_TAP = 0x1 << 6,
	DOUBLE_TAP = 0x1 << 5,
	ACTIVITY = 0x1 << 4,
	INACTIVITY = 0x1 << 3,
	FREE_FALL = 0x1 << 2,
	WATERMARK = 0x1 << 1,
	OVERRUN = 0x1 << 0
};

enum Rate {
	RATE_3200 = 0xF,
	RATE_1600 = 0xE,
	RATE_800 = 0xD,
	RATE_400 = 0xC,
	RATE_200 = 0xB,
	RATE_100 = 0xA,
	RATE_50 = 0x9,
	RATE_25 = 0x8,
	RATE_12_5 = 0x7,
	RATE_6_25 = 0x6,
	RATE_3_13 = 0x5,
	RATE_1_56 = 0x4,
	RATE_0_78 = 0x3,
	RATE_0_39 = 0x2,
	RATE_0_20 = 0x1,
	RATE_0_10 = 0x0
};

struct HarvbotAccelData
{
	inline HarvbotAccelData() : x(raw[0]), y(raw[1]), z(raw[2]) {}
	inline uint8_t* getPtr() { return (uint8_t*)raw; }

	static const int size = 6;

	int16_t raw[3];
	int16_t &x;
	int16_t &y;
	int16_t &z;

	Scale scale_g;

	float gx();
	float gy();
	float gz();
};