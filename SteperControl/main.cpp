#include <wiringPi.h>
#include <math.h>
#include "Harvbot/HarvbotStepper.h"
#include "Accelerometers/HarvbotAccelData.h"
#include "Accelerometers/HarvbotADXL345PiI2CAccelerometer.h"
#include "RF24.h"
#include "ADXL345Pi.h"
#include "VL53L0XPi.hpp"
#include "ams_as5048bPi.h"

//					WiringPI			Shifter-sheld      
#define SX_STEP         4       //    		16
#define SX_DIR          5       // 		    18
#define SX_END          6       //    		22
#define SX_RATIO        64

#define SY_STEP         11      //   		26
#define SY_DIR          26      //    		32
#define SY_END          27  	//			36
#define SY_RATIO        64

#define SZ_STEP         23		//  		33   
#define SZ_DIR          22      //    		31 
#define SZ_END          21		//			29
#define SZ_RATIO        4

#define SJ_STEP         3       //  		15 
#define SJ_DIR          2       //  		13
#define SJ_END          30		//			27
#define SJ_RATIO        4 * 15


unsigned char recieved_data[4];
unsigned char address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

RF24 radio(10, 7);

HarvbotStepper* SX;
HarvbotStepper* SY;
HarvbotStepper* SZ;
HarvbotStepper* SJ;

#define offsetX     0    // OFFSET values
#define offsetY     0
#define offsetZ     0

#define gainX       1     // GAIN factors
#define gainY       1
#define gainZ       1

void setup() {

	wiringPiSetup();

	SX = new HarvbotStepper(SX_STEP, SX_DIR, 10000);
	SY = new HarvbotStepper(SY_STEP, SY_DIR, 14000);
	SZ = new HarvbotStepper(SZ_STEP, SZ_DIR, 1000);
	SJ = new HarvbotStepper(SJ_STEP, SJ_DIR, 5000);

	radio.begin(); //активировать модуль
	radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
	radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
	radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
	radio.setPayloadSize(32);     //размер пакета, в байтах

	radio.openReadingPipe(0, address[0]);     //хотим слушать трубу 0
	radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

	radio.setPALevel(RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	radio.setDataRate(RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
									 //должна быть одинакова на приёмнике и передатчике!
									 //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

	radio.powerUp(); //начать работу
	radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
}

int getDelta(unsigned char val)
{
	if (val < 1)
	{
		return -1;
	}
	else if (val > 140)
	{
		return 1;
	}
	return val = 0;
}

void runAllEnginesTillPostions()
{
	bool sxRun = false;
	bool syRun = false;
	bool szRun = false;
	bool sjRun = false;
	do
	{
		sxRun = SX->run();
		syRun = SY->run();
		szRun = SZ->run();
		sjRun = SJ->run();
	} while (sxRun || syRun || szRun || sjRun);
}

void loop() {

	unsigned char pipeNo;
	while (radio.available(&pipeNo)) {  // слушаем эфир со всех труб
		radio.read(&recieved_data, sizeof(recieved_data));

		int sx = getDelta(recieved_data[0])*SX_RATIO;
		int sy = getDelta(recieved_data[1])*SY_RATIO;
		int sz = getDelta(recieved_data[2])*SZ_RATIO;
		int sj = getDelta(recieved_data[3])*SJ_RATIO;

		SX->move(sx);
		SY->move(sy);
		SZ->move(sz);
		SJ->move(sj);

		runAllEnginesTillPostions();
	}
}


int main(void)
{
	printf("Program started\n");
	setup();

	AMS_AS5048BPi mysensor("/dev/i2c-1");

	//init AMS_AS5048B object
	mysensor.begin();

	//consider the current position as zero
	mysensor.setZeroReg();

	while (1)
	{
		cout << mysensor.angleR(U_RAW, true) << endl;
	}

	/*SX->setAccelerationPercent(20);
	SX->move(-50 * SJ_RATIO);
	runAllEnginesTillPostions();
*/
	/*SY->move(-1000 * SY_RATIO);
	SX->move(-1000 * SY_RATIO);
	runAllEnginesTillPostions();
*/
	//HarvbotADXL345PiI2CAccelerometer* accel = new HarvbotADXL345PiI2CAccelerometer();
	//accel->setPowerMode(NormalPower);
	//accel->setActive(true);

	//while (1)
	//{
	//	HarvbotAccelData data = accel->readData();

	//	double roll = atan2(data.x, data.y) * 180.0f / M_PI;
	//	double pitch = atan2((-data.x), sqrt(data.y * data.y + data.z * data.z)) * 57.3;
	//	cout << "Raw: " << data.x << ", " << data.y << ", " << data.z << endl;
	//	//cout << roll << ":" << pitch << endl;
	//}
	//delete accel;

	/*int x, y, z, i;
	double xyz[3], gains[3], gains_orig[3];
	gains[0] = 0;
	gains[1] = 0;
	gains[2] = 0;

	ADXL345 accel;
	accel.powerOn();
	accel.getAxisGains(gains_orig);
	while (1)
	{
		accel.readAccel(&x, &y, &z);
		cout << "x=" << x << " y=" << y << " z=" << z << endl;
	}*/

	/*VL53L0XPi sensor;
	sensor.init();
	sensor.setTimeout(500);
	sensor.startContinuous();

	while (1)
	{
		cout << sensor.readRangeContinuousMillimeters() << endl;
	}*/

	//while (1)
	//{
	//	int numberOfSteps = 100;
	//	//SX->move(-numberOfSteps * SX_RATIO);
	//	//SX->move(-numberOfSteps * SJ_RATIO);
	//	SY->move(-numberOfSteps * SY_RATIO);
	//	//SZ->move(-numberOfSteps * SZ_RATIO);
	//	runAllEnginesTillPostions();
	//	//delay(5);
	//	//SX->move(numberOfSteps * SX_RATIO);
	//	SY->move(numberOfSteps * SY_RATIO);
	//	//SZ->move(numberOfSteps * SZ_RATIO);
	//	runAllEnginesTillPostions();
	//}

	/*while (1)
		loop();
*/
	return 0;
}