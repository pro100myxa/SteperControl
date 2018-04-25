#pragma once
class HarvbotLaser
{
public:
	HarvbotLaser(unsigned short pin);
	~HarvbotLaser();

	void powerOn();

	void powerOff();
private:
	unsigned short _pin;
};

