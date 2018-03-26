#include <wiringPiSPI.h>
#include <wiringPi.h>
#include "LibStepper.h"
#include "RF24.h"

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
#define SJ_END          30		//			27


unsigned char recieved_data[4];
unsigned char address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

RF24 radio(10, 7);

LibStepper* SX;
LibStepper* SY;
LibStepper* SZ;
LibStepper* SJ;

void setup() {

	wiringPiSetup();

	SX = new LibStepper(SX_STEP, SX_DIR, SX_END);
	SY = new LibStepper(SY_STEP, SY_DIR, SY_END);
	SZ = new LibStepper(SZ_STEP, SZ_DIR, SZ_END);
	SJ = new LibStepper(SJ_STEP, SJ_DIR, SJ_END);

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

	SX->setMinPulseWidth(200);
	SY->setMinPulseWidth(200);
	SZ->setMinPulseWidth(200);
	SJ->setMinPulseWidth(200);
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

void loop() {
	int speed = 200;

	// чиатем входящий сигнал
	unsigned char pipeNo;
	while (radio.available(&pipeNo)) {  // слушаем эфир со всех труб
		radio.read(&recieved_data, sizeof(recieved_data));

		int sx = getDelta(recieved_data[0]);
		int sy = getDelta(recieved_data[3]);
		int sz = getDelta(recieved_data[1]);
		int sj = getDelta(recieved_data[2]);

		SX->move(sx * 50);
		SY->move(sy * 50);
		SZ->move(sz * 50);
		SJ->move(sj * 50);

		while (SX->run() || SY->run() || SZ->run() || SJ->run())
			;
	}
}

int main(void)
{
	printf("Program started\n");

	setup();

	/*int i = 0;
	while (i < 1000)
	{
		digitalWrite(SY_DIR, HIGH);
		digitalWrite(SY_STEP, HIGH);
		delayMicroseconds(150);
		digitalWrite(SY_STEP, LOW);
		i++;
	}*/
	SY->move(100);
	SY->runToPosition();

	while (1)
		loop();

	return 0;
}