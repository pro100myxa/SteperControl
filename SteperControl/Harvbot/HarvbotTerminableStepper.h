#ifndef HarvbotTerminableStepper_h
#define HarvbotTerminableStepper_h

#include <wiringPi.h>
#include <pthread.h>
#include "HarvbotStepper.h"

class HarvbotTerminableStepper : public HarvbotStepper
{
public:
	HarvbotTerminableStepper(uint8_t stepPin, uint8_t dirPin, uint8_t terminalPin = 0);
	
	void runTillTerminal(bool direction);

	bool isTerminated();
protected:
	void step(long step);
	
private:

	// Stores terminal pin.
	int _terminalPin;

	// Stores stepper direction in time of terminal fire.
	bool _terminalPressedDir;

	// Stores value indicating where terminal was pressed.
	bool _terminalPressed;
};
#endif
