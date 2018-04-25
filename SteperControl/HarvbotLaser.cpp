#include "HarvbotLaser.h"
#include <wiringPi.h>

HarvbotLaser::HarvbotLaser(unsigned short pin)
{
	_pin = pin;
	pinMode(_pin, OUTPUT);
}


HarvbotLaser::~HarvbotLaser()
{
}

void HarvbotLaser::powerOn()
{
	digitalWrite(_pin, HIGH);
}

void HarvbotLaser::powerOff()
{
	digitalWrite(_pin, LOW);
}