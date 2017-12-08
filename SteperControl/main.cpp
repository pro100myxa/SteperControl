#include <wiringPi.h>   ////Transforms Odroid into a very fast Arduino
#include <stdio.h>
#include "AccelStepper.h"



bool SX_END, SY_END, SZ_END;



//					WiringPI			Shifter-sheld      
#define SX_STEP         4       //    		16
#define SX_DIR          5       // 		    18
#define SX_END          6       //    		22

#define SY_STEP         11      //   		26
#define SY_DIR          26      //    		32
#define SY_END          27  	//			36

#define SZ_STEP         23		//  		33   
#define SZ_DIR          22      //    		31 
#define SZ_END          21		//			29

#define SJ_STEP         3       //  		15 
#define SJ_DIR          2       //  		13
			  

int main(void)
{
	wiringPiSetup();

	pinMode(SX_STEP, OUTPUT);       	pinMode(SY_STEP, OUTPUT);       	pinMode(SZ_STEP, OUTPUT); 		pinMode(SJ_STEP, OUTPUT);
	pinMode(SX_DIR, OUTPUT);		 	pinMode(SY_DIR, OUTPUT);		 	pinMode(SZ_DIR, OUTPUT); 		pinMode(SJ_DIR, OUTPUT);
	pinMode(SX_END, INPUT); 			pinMode(SY_END, INPUT); 			pinMode(SZ_END, INPUT);


	AccelStepperSetup();

	AccelStepper SX(1, SX_DIR, SX_STEP); //using 5 and 4 pins for direction and step, 1 - "external driver" mode (A4988) // Stepper X asix

	SX.setMaxSpeed(1000); // set max speed of stepper motor (When driving, the stepper motor will accelerate to this maximum speed and slow down when approaching the end of the movement.)

	SX.setAcceleration(100); //Set the acceleration, in steps per second

	SX.setSpeed(100); //Set the speed in steps per second. The movement itself is started by runSpeed ().


	AccelStepper SY(1, SX_DIR, SX_STEP); //Stepper Y asix

	SY.setMaxSpeed(stepsPerSecond);

	SY.setAcceleration(stepsPerSecondSquared);

	SY.setSpeed(stepsPerSecond);


	AccelStepper SZ(1, SX_DIR, SX_STEP); //Stepper Z asix

	SZ.setMaxSpeed(stepsPerSecond);

	SZ.setAcceleration(stepsPerSecondSquared);

	SZ.setSpeed(stepsPerSecond);


	AccelStepper SJ(1, SX_DIR, SX_STEP); //Stepper jaws

	SJ.setMaxSpeed(stepsPerSecond);

	SJ.setSpeed(stepsPerSecond);

	//SX.runSpeed();




	return 0;
}