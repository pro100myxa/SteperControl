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

LibStepper SX(SX_STEP, SX_DIR, SX_END);
LibStepper SY(SY_STEP, SY_DIR, SY_END);
LibStepper SZ(SZ_STEP, SZ_DIR, SZ_END);
LibStepper SJ(SJ_STEP, SJ_DIR, SJ_END);

void setup() {

	wiringPiSetup();

	pinMode(SX_STEP, OUTPUT);       	pinMode(SY_STEP, OUTPUT);       	pinMode(SZ_STEP, OUTPUT); 		pinMode(SJ_STEP, OUTPUT);
	pinMode(SX_DIR, OUTPUT);		 	pinMode(SY_DIR, OUTPUT);		 	pinMode(SZ_DIR, OUTPUT); 		pinMode(SJ_DIR, OUTPUT);
	pinMode(SX_END, INPUT); 			pinMode(SY_END, INPUT); 			pinMode(SZ_END, INPUT);			pinMode(SJ_END, INPUT);

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

	int speed = 200;
	SX.setSpeed(speed);
	SY.setSpeed(speed);
	SZ.setSpeed(speed);
	SJ.setSpeed(speed);
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
	
	// чиатем входящий сигнал
	unsigned char pipeNo;
	while (radio.available(&pipeNo)) {  // слушаем эфир со всех труб
		radio.read(&recieved_data, sizeof(recieved_data));

		int sx = getDelta(recieved_data[0]);
		int sy = getDelta(recieved_data[3]);
		int sz = getDelta(recieved_data[1]);
		int sj = getDelta(recieved_data[2]);

		SX.move(sx);
		SY.move(sy);
		SZ.move(sz);
		SJ.move(sj);

		SX.run();
		SY.run();
		SZ.run();
		SJ.run();
	}
}

int main(void)
{
	setup();
	while (1)
		loop();

	return 0;
}