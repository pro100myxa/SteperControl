/*
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Name        : PCA9685.cpp
 * Author      : Georgi Todorov
 * Version     :
 * Created on  : Dec 9, 2012
 *
 * Copyright © 2012 Georgi Todorov  <terahz@geodar.com>
 */


#include <stdio.h>      /* Standard I/O functions */
#include <inttypes.h>
#include <errno.h>
#include <math.h>

#include "PCA9685Pi.h"

//! Constructor takes bus and address arguments
/*!
 \param bus the bus to use in /dev/i2c-%d.
 \param address the device address on bus
 */
PCA9685Pi::PCA9685Pi(const char* device, int address) {
	handle = wiringPiI2CSetupInterface(device, address);
	reset();
	setPWMFreq(1000);
}

PCA9685Pi::~PCA9685Pi() {
}

//! Sets PCA9685 mode to 00
void PCA9685Pi::reset() {

	wiringPiI2CWriteReg8(handle, MODE1, 0x00);
	wiringPiI2CWriteReg8(handle, MODE2, 0x04);
}
//! Set the frequency of PWM
/*!
 \param freq desired frequency. 40Hz to 1000Hz using internal 25MHz oscillator.
 */
void PCA9685Pi::setPWMFreq(int freq) {

		uint8_t prescale_val = (CLOCK_FREQ / 4096 / freq)  - 1;
		wiringPiI2CWriteReg8(handle, MODE1, 0x10);
		wiringPiI2CWriteReg8(handle, PRE_SCALE, prescale_val);
		wiringPiI2CWriteReg8(handle, MODE1, 0x80);
		wiringPiI2CWriteReg8(handle, MODE2, 0x04);
}

//! PWM a single channel
/*!
 \param led channel (1-16) to set PWM value for
 \param value 0-4095 value for PWM
 */
void PCA9685Pi::setPWM(uint8_t led, int value) {
	setPWM(led, 0, value);
}
//! PWM a single channel with custom on time
/*!
 \param led channel (1-16) to set PWM value for
 \param on_value 0-4095 value to turn on the pulse
 \param off_value 0-4095 value to turn off the pulse
 */
void PCA9685Pi::setPWM(uint8_t led, int on_value, int off_value) {
	wiringPiI2CWriteReg8(handle, LED0_ON_L + LED_MULTIPLYER * (led - 1), on_value & 0xFF);
	wiringPiI2CWriteReg8(handle, LED0_ON_H + LED_MULTIPLYER * (led - 1), on_value >> 8);
	wiringPiI2CWriteReg8(handle, LED0_OFF_L + LED_MULTIPLYER * (led - 1), off_value & 0xFF);
	wiringPiI2CWriteReg8(handle, LED0_OFF_H + LED_MULTIPLYER * (led - 1), off_value >> 8);
}

//! Get current PWM value
/*!
 \param led channel (1-16) to get PWM value from
 */
int PCA9685Pi::getPWM(uint8_t led){
	int ledval = 0;
	ledval = wiringPiI2CReadReg8(handle, LED0_OFF_H + LED_MULTIPLYER * (led - 1));
	ledval = ledval & 0xf;
	ledval <<= 8;
	ledval += wiringPiI2CReadReg8(handle, LED0_OFF_L + LED_MULTIPLYER * (led - 1));
	return ledval;
}