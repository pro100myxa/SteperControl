#include <wiringPi.h>////Transforms Odroid into a very fast Arduino
//#include <stdio.h>
#include "AccelStepper.h"


/*
////Defining & Configuring Motors
AccelStepper mystepper(DRIVER, step, direction);              // Шаговый двигатель, управляемый выделенной платой.

mystepper.setMaxSpeed(stepsPerSecond);            // Установка максимальной скорости (оборотов в минуту). Скорость по умолчанию очень низкая, так что её требуется переопределить. При движении шаговый двигатель будет ускоряться до этой максимальной скорости и замедляться при подходе к концу движения.
mystepper.setAcceleration(stepsPerSecondSquared); // Установка ускорения, в шагах в секунду за секунду.

////Position Based Control
mystepper.isRunning();                            // Возвращает true, если ШД в режиме вращения.
mystepper.currentPosition();                      // Возвращает текущее абсолютное положение (в шагах).
mystepper.setCurrentPosition(long position);      // Установить счетчик как текущую позицию. Полезно как задание нулевой координаты. Обнуляет текущую скорость до нуля.
mystepper.targetPosition();                       // Конечное положение (в шагах).
mystepper.distanceToGo();                         // Вернуть расстояние до указанного положения. Может использоваться для проверки: достиг ли двигатель указанной конечной точки.
mystepper.moveTo(long absolute);                  // Переместиться в абсолютно указанное положение. Само движение запускается функцией run().
mystepper.move(long relative);                    // Переместиться в относительно указанное положение. Само движение запускается функцией run(). Значение distance может быть больше или меньше нуля.
mystepper.run();                                  // Начать движение с ускорением. Один шаг. Для продолжения движения следует вызывать функцию повторно как можно чаще - в loop() или иной функции.
mystepper.runToPosition();                        // Начать движение и подождать когда двигатель достигнет указанной точки. БЛОКИРУЮЩАЯ ФУНКЦИЯ - функция не прерывается, пока двигатель не остановится!
mystepper.runToNewPosition();                     // Начать движение с ускорением, до заданной позиции. БЛОКИРУЮЩАЯ ФУНКЦИЯ!
mystepper.stop();                                 // Максимально быстрая остановка (без замедления), используя текущие параметры скорости и ускорения. Также задает новое конечное положение.

////Speed Based Control
mystepper.setSpeed(stepsPerSecond);               // Установить скорость (в шагах за секунду). Само движение запускается функцией runSpeed().
mystepper.runSpeed();                             // Начать движение с текущей заданной скоростью (без плавного ускорения). Для продолжения движения двигателя следует вызывать функцию повторно как можно чаще.
mystepper.runSpeedToPosition();                   // Начать движение с текущей заданной скоростью (без плавного ускорения), до заданной позиции. БЛОКИРУЮЩАЯ ФУНКЦИЯ!

////Power Control
mystepper.disableOutputs();                       // Деактивировать занятые пины и установить их в LOW. Снимает напряжение с обмоток двигателя, экономя энергию (вал при это может проворачиваться под внешними нагрузками)
mystepper.enableOutputs();                        // Активировать зарезервированные пины и устанавивает их в режим OUTPUT. Вызывается автоматически при запуске движения.
*/


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

	AccelStepper SX(1, SX_DIR, SX_STEP); //using 5 and 4 pins for direction and step, 1 - "external driver" mode (A4988) // Stepper X asix
	SX.setMaxSpeed(1000); // set max speed of stepper motor (When driving, the stepper motor will accelerate to this maximum speed and slow down when approaching the end of the movement.)
	SX.setAcceleration(100); //Set the acceleration, in steps per second
	SX.setSpeed(100); //Set the speed in steps per second. The movement itself is started by runSpeed ().
	SX.move(positionX);
	SX.run();
	if (SX_END == 0) {
		SX.stop();
		SX.setCurrentPosition(0);
	}


	AccelStepper SY(1, SX_DIR, SX_STEP); //Stepper Y asix
	SY.setMaxSpeed(1000);
	SY.setAcceleration(100);
	SY.setSpeed(100);
	SY.move(positionY);
	SY.run();
	if (SY_END == 0) {
		SY.stop();
		SY.setCurrentPosition(0);
	}

	AccelStepper SZ(1, SX_DIR, SX_STEP); //Stepper Z asix
	SZ.setMaxSpeed(1000);
	SZ.setAcceleration(100);
	SZ.setSpeed(100);
	SZ.move(positionZ);
	SZ.run();
	if (SZ_END == 0) {
		SZ.stop();
		SZ.setCurrentPosition(0);
	}

	AccelStepper SJ(1, SX_DIR, SX_STEP); //Stepper jaws
	SJ.setMaxSpeed(1000);
	SJ.setSpeed(100);
	SJ.move(positionJ);
	SJ.run();
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	

	
	



	return 0;
}
