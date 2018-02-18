
#include <wiringPi.h>////Transforms Odroid into a very fast Arduino
#include "LibStepper.h"





bool SX_END, SY_END, SZ_END;
int positionX, positionY, positionZ, positionJ;


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

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	
	
	



	LibStepper SJ(SJ_STEP, SJ_DIR);
	SJ.speed(600);
	SJ.moveTo(-30);
	







	


    return 0;
}
