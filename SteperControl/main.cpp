#include <wiringPi.h>
#include <math.h>
#include "Harvbot/HarvbotStepper.h"
#include "Accelerometers/HarvbotAccelData.h"
#include "Accelerometers/HarvbotADXL345PiI2CAccelerometer.h"
#include "RF24.h"
#include "ADXL345Pi.h"
#include "VL53L0XPi.hpp"
#include "AS5048APi.h"
#include "PCA9685Pi.h"
#include <linux/spi/spidev.h>
#include <stdlib.h>
#include <errno.h>

//					WiringPI			Shifter-sheld      
#define SX_STEP         4       //    		16
#define SX_DIR          5       // 		    18
#define SX_END          6       //    		22
#define SX_RATIO        50 * 32

#define SY_STEP         11      //   		26
#define SY_DIR          0	    //    		32
#define SY_END          27  	//			36
#define SY_RATIO        50 * 32

#define SZ_STEP         23		//  		33   
#define SZ_DIR          22      //    		31 
#define SZ_END          21		//			29
#define SZ_RATIO        50 * 32

#define SJ_STEP         3       //  		15 
#define SJ_DIR          2       //  		13
#define SJ_END          26		//			27
#define SJ_RATIO        1 * 32


unsigned char recieved_data[4];
unsigned char address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

//RF24 radio(10, 7);

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

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX 4096 // this is the 'maximum' pulse length count (out of 4096)

void setup() {

	wiringPiSetup();

	SX = new HarvbotStepper(SX_STEP, SX_DIR, 22000);
	SY = new HarvbotStepper(SY_STEP, SY_DIR, 22000);
	SZ = new HarvbotStepper(SZ_STEP, SZ_DIR, 22000);
	SJ = new HarvbotStepper(SJ_STEP, SJ_DIR, 22000);

	//radio.begin(); //активировать модуль
	//radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
	//radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
	//radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
	//radio.setPayloadSize(32);     //размер пакета, в байтах

	//radio.openReadingPipe(0, address[0]);     //хотим слушать трубу 0
	//radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

	//radio.setPALevel(RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
	//radio.setDataRate(RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
	//								 //должна быть одинакова на приёмнике и передатчике!
	//								 //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

	//radio.powerUp(); //начать работу
	//radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
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
//
//void loop() {
//
//	unsigned char pipeNo;
//	while (radio.available(&pipeNo)) {  // слушаем эфир со всех труб
//		radio.read(&recieved_data, sizeof(recieved_data));
//
//		int sx = getDelta(recieved_data[0])*SX_RATIO;
//		int sy = getDelta(recieved_data[1])*SY_RATIO;
//		int sz = getDelta(recieved_data[2])*SZ_RATIO;
//		int sj = getDelta(recieved_data[3])*SJ_RATIO;
//
//		SX->move(sx);
//		SY->move(sy);
//		SZ->move(sz);
//		SJ->move(sj);
//
//		runAllEnginesTillPostions();
//	}
//}

#define PIN_BASE 300
#define MAX_PWM 4096
#define HERTZ 50

int calcTicks(float impulseMs, int hertz)
{
	float cycleMs = 1000.0f / hertz;
	return (int)(MAX_PWM * impulseMs / cycleMs + 0.5f);
}


#define BITCOUNT(x)     (((BX_(x)+(BX_(x)>>4)) & 0x0F0F0F0F) % 255)
#define BX_(x)          ((x) - (((x)>>1)&0x77777777) \
                             - (((x)>>2)&0x33333333) \
                             - (((x)>>3)&0x11111111))

// SPI commands

#define FLAG_READ 0x4000
#define CMD_ANGLEDATA 0x3fff
#define CMD_MAGDATA 0x3ffe
#define CMD_DIAGDATA 0x3ffd
#define CMD_NOOP 0x0000

int fd;
uint16_t sendReceive(uint16_t command, bool verbose = false)
{
	if (BITCOUNT(command) % 2)
		command |= 0x8000;

	unsigned char data[2];
	data[0] = command >> 8;
	data[1] = command & 0xff;

	if (verbose)
		printf("sending %02x%02x   ", data[0], data[1]);

	wiringPiSPIDataRW(fd, data, 2);

	if (verbose)
		printf("received %02x%02x\n", data[0], data[1]);

	return (data[0] << 8) + data[1];
}


int main(void)
{
	//wiringPiSetup();
	//printf("Program started\n");
	setup();

	//fd = wiringPiSPISetupMode(0, 500000, SPI_MODE_1);
	//if (fd == -1)
	//{
	//	perror("wiringPiSPISetupMode");
	//	exit(2);
	//}

	//while (1)
	//{
	//	sendReceive(CMD_ANGLEDATA | FLAG_READ);

	//	// Flush the command with a NOOP and record the reply.
	//	uint16_t angledata = sendReceive(CMD_NOOP);

	//	cout << ((angledata & 0x3fff) * 360.0f / 0x3fff) << endl;
	//	delay(300);
	//}

	//AMS_AS5048BPi mysensor("/dev/i2c-1");

	////init AMS_AS5048B object
	//mysensor.begin();

	////consider the current position as zero
	//mysensor.setZeroReg();

	//while (1)
	//{
	//	cout << mysensor.angleR(U_RAW, true) << endl;
	//}

	/*PCA9685Pi pca9685;
	unsigned int pulse_length = 1000000;
	pulse_length = 200;
	
	pca9685.setPWMFreq(60);

	while (1)
	{
		pca9685.setPWM(1, calcTicks(1, 60));

		delay(2000);

		pca9685.setPWM(1, calcTicks(1.5, 60));

		delay(2000);

		pca9685.setPWM(1, calcTicks(2, 60));

		delay(2000);
	}*/

	/*pinMode(26, OUTPUT);
	while (1)
	{
		digitalWrite(26, HIGH);
		delay(500);
		digitalWrite(26, LOW);
		delay(500);
	}*/

	//pinMode(10, OUTPUT);

	//wiringPiSPISetupMode(0, 1000000, SPI_MODE_1);
	/*word command = 0;
	while (1)
	{
		command = 0;
		wiringPiSPIDataRW(0, (byte*)&command, 2);

		command = command & ~0xC000;
		cout << command << endl;
		delay(500);
	}
*/
	//AS5048APi as5048a(10);
	//as5048a.init();
	//while (1)
	//{
	//	//as5048a.getRawRotation();
	//	cout << as5048a.getRawRotation() << endl;
	//	delay(500);
	//}
	//

	/*int i = 0;
	unsigned int lastStepTime = micros();
	while (1)
	{
		while(i<4096)
		{
			unsigned int time = micros();
			if (time > lastStepTime + pulse_length)
			{
				pca9685.setPWM(1, i);
				lastStepTime = time;
				i++;
			}

			delayMicroseconds(100);
		}

		delay(1000);

		lastStepTime = micros();
		while (i>=0)
		{
			unsigned int time = micros();
			if (time > lastStepTime + pulse_length)
			{
				pca9685.setPWM(1, i);
				lastStepTime = time;
				i--;
			}

			delayMicroseconds(100);
		}

		delay(1000);
		i = 0;
	}*/

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

	/*while (1)
	{*/
		int numberOfSteps = 20;
		SX->move(-numberOfSteps * SX_RATIO);
		SY->move(-numberOfSteps * SY_RATIO);
		SZ->move(-numberOfSteps / 2 * SZ_RATIO);
		SJ->move(-numberOfSteps * 30 * SJ_RATIO);
		runAllEnginesTillPostions();
		//delay(5);
		SX->move(numberOfSteps * SX_RATIO);
		SY->move(numberOfSteps * SY_RATIO);
		SZ->move(numberOfSteps / 2 * SZ_RATIO);
		SJ->move(numberOfSteps * 30 * SJ_RATIO);
		runAllEnginesTillPostions();
	//}

	/*while (1)
		loop();
*/
	return 0;
}