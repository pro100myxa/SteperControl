// AccelStepper.h
//
/// \mainpage AccelStepper library for Arduino
///
/// This is the Arduino AccelStepper library.
/// It provides an object-oriented interface for 2, 3 or 4 pin stepper motors.
///
/// The standard Arduino IDE includes the Stepper library
/// (http://arduino.cc/en/Reference/Stepper) for stepper motors. It is
/// perfectly adequate for simple, single motor applications.
///
/// AccelStepper significantly improves on the standard Arduino Stepper library in several ways:
/// \li Supports acceleration and deceleration
/// \li Supports multiple simultaneous steppers, with independent concurrent stepping on each stepper
/// \li API functions never delay() or block
/// \li Supports 2, 3 and 4 wire steppers, plus 3 and 4 wire half steppers.
/// \li Supports alternate stepping functions to enable support of AFMotor (https://github.com/adafruit/Adafruit-Motor-Shield-library)
/// \li Supports stepper drivers such as the Sparkfun EasyDriver (based on 3967 driver chip)
/// \li Very slow speeds are supported
/// \li Extensive API
/// \li Subclass support
///
/// The latest version of this documentation can be downloaded from 
/// http://www.airspayce.com/mikem/arduino/AccelStepper
/// The version of the package that this documentation refers to can be downloaded 
/// from http://www.airspayce.com/mikem/arduino/AccelStepper/AccelStepper-1.48.zip
///
/// Example Arduino programs are included to show the main modes of use.
///
/// You can also find online help and discussion at http://groups.google.com/group/accelstepper
/// Please use that group for all questions and discussions on this topic. 
/// Do not contact the author directly, unless it is to discuss commercial licensing.
/// Before asking a question or reporting a bug, please read http://www.catb.org/esr/faqs/smart-questions.html
///
/// Tested on Arduino Diecimila and Mega with arduino-0018 & arduino-0021 
/// on OpenSuSE 11.1 and avr-libc-1.6.1-1.15,
/// cross-avr-binutils-2.19-9.1, cross-avr-gcc-4.1.3_20080612-26.5.
/// Tested on Teensy http://www.pjrc.com/teensy including Teensy 3.1 built using Arduino IDE 1.0.5 with 
/// teensyduino addon 1.18 and later.
///
/// \par Installation
///
/// Install in the usual way: unzip the distribution zip file to the libraries
/// sub-folder of your sketchbook. 
///
/// \par Theory
///
/// This code uses speed calculations as described in 
/// "Generate stepper-motor speed profiles in real time" by David Austin 
/// http://fab.cba.mit.edu/classes/MIT/961.09/projects/i0/Stepper_Motor_Speed_Profile.pdf
/// with the exception that AccelStepper uses steps per second rather than radians per second
/// (because we dont know the step angle of the motor)
/// An initial step interval is calculated for the first step, based on the desired acceleration
/// On subsequent steps, shorter step intervals are calculated based 
/// on the previous step until max speed is achieved.
/// 
/// \par Donations
///
/// This library is offered under a free GPL license for those who want to use it that way. 
/// We try hard to keep it up to date, fix bugs
/// and to provide free support. If this library has helped you save time or money, please consider donating at
/// http://www.airspayce.com or here:
///
/// \htmlonly <form action="https://www.paypal.com/cgi-bin/webscr" method="post"><input type="hidden" name="cmd" value="_donations" /> <input type="hidden" name="business" value="mikem@airspayce.com" /> <input type="hidden" name="lc" value="AU" /> <input type="hidden" name="item_name" value="Airspayce" /> <input type="hidden" name="item_number" value="AccelStepper" /> <input type="hidden" name="currency_code" value="USD" /> <input type="hidden" name="bn" value="PP-DonationsBF:btn_donateCC_LG.gif:NonHosted" /> <input type="image" alt="PayPal — The safer, easier way to pay online." name="submit" src="https://www.paypalobjects.com/en_AU/i/btn/btn_donateCC_LG.gif" /> <img alt="" src="https://www.paypalobjects.com/en_AU/i/scr/pixel.gif" width="1" height="1" border="0" /></form> \endhtmlonly
/// 
/// \par Trademarks
///
/// AccelStepper is a trademark of AirSpayce Pty Ltd. The AccelStepper mark was first used on April 26 2010 for
/// international trade, and is used only in relation to motor control hardware and software.
/// It is not to be confused with any other similar marks covering other goods and services.
///
/// \par Copyright
///
/// This software is Copyright (C) 2010 Mike McCauley. Use is subject to license
/// conditions. The main licensing options available are GPL V2 or Commercial:
///
/// \par Open Source Licensing GPL V2
/// This is the appropriate option if you want to share the source code of your
/// application with everyone you distribute it to, and you also want to give them
/// the right to share who uses it. If you wish to use this software under Open
/// Source Licensing, you must contribute all your source code to the open source
/// community in accordance with the GPL Version 2 when your application is
/// distributed. See http://www.gnu.org/copyleft/gpl.html
/// 
/// \par Commercial Licensing
/// This is the appropriate option if you are creating proprietary applications
/// and you are not prepared to distribute and share the source code of your
/// application. Contact info@airspayce.com for details.
///
/// \par Revision History
/// \version 1.0 Initial release
///
/// \version 1.1 Added speed() function to get the current speed.
/// \version 1.2 Added runSpeedToPosition() submitted by Gunnar Arndt.
/// \version 1.3 Added support for stepper drivers (ie with Step and Direction inputs) with _pins == 1
/// \version 1.4 Added functional contructor to support AFMotor, contributed by Limor, with example sketches.
/// \version 1.5 Improvements contributed by Peter Mousley: Use of microsecond steps and other speed improvements
///              to increase max stepping speed to about 4kHz. New option for user to set the min allowed pulse width.
///              Added checks for already running at max speed and skip further calcs if so. 
/// \version 1.6 Fixed a problem with wrapping of microsecond stepping that could cause stepping to hang. 
///              Reported by Sandy Noble.
///              Removed redundant _lastRunTime member.
/// \version 1.7 Fixed a bug where setCurrentPosition() did not always work as expected. 
///              Reported by Peter Linhart.
/// \version 1.8 Added support for 4 pin half-steppers, requested by Harvey Moon
/// \version 1.9 setCurrentPosition() now also sets motor speed to 0.
/// \version 1.10 Builds on Arduino 1.0
/// \version 1.11 Improvments from Michael Ellison:
///   Added optional enable line support for stepper drivers
///   Added inversion for step/direction/enable lines for stepper drivers
/// \version 1.12 Announce Google Group
/// \version 1.13 Improvements to speed calculation. Cost of calculation is now less in the worst case, 
///    and more or less constant in all cases. This should result in slightly beter high speed performance, and
///    reduce anomalous speed glitches when other steppers are accelerating. 
///    However, its hard to see how to replace the sqrt() required at the very first step from 0 speed.
/// \version 1.14 Fixed a problem with compiling under arduino 0021 reported by EmbeddedMan
/// \version 1.15 Fixed a problem with runSpeedToPosition which did not correctly handle
///    running backwards to a smaller target position. Added examples
/// \version 1.16 Fixed some cases in the code where abs() was used instead of fabs().
/// \version 1.17 Added example ProportionalControl
/// \version 1.18 Fixed a problem: If one calls the funcion runSpeed() when Speed is zero, it makes steps 
///    without counting. reported by  Friedrich, Klappenbach.
/// \version 1.19 Added MotorInterfaceType and symbolic names for the number of pins to use
///               for the motor interface. Updated examples to suit.
///               Replaced individual pin assignment variables _pin1, _pin2 etc with array _pin[4].
///               _pins member changed to _interface.
///               Added _pinInverted array to simplify pin inversion operations.
///               Added new function setOutputPins() which sets the motor output pins.
///               It can be overridden in order to provide, say, serial output instead of parallel output
///               Some refactoring and code size reduction.
/// \version 1.20 Improved documentation and examples to show need for correctly
///               specifying AccelStepper::FULL4WIRE and friends.
/// \version 1.21 Fixed a problem where desiredSpeed could compute the wrong step acceleration
///               when _speed was small but non-zero. Reported by Brian Schmalz.
///               Precompute sqrt_twoa to improve performance and max possible stepping speed
/// \version 1.22 Added Bounce.pde example
///               Fixed a problem where calling moveTo(), setMaxSpeed(), setAcceleration() more 
///               frequently than the step time, even
///               with the same values, would interfere with speed calcs. Now a new speed is computed 
///               only if there was a change in the set value. Reported by Brian Schmalz.
/// \version 1.23 Rewrite of the speed algorithms in line with 
///               http://fab.cba.mit.edu/classes/MIT/961.09/projects/i0/Stepper_Motor_Speed_Profile.pdf
///               Now expect smoother and more linear accelerations and decelerations. The desiredSpeed()
///               function was removed.
/// \version 1.24  Fixed a problem introduced in 1.23: with runToPosition, which did never returned
/// \version 1.25  Now ignore attempts to set acceleration to 0.0
/// \version 1.26  Fixed a problem where certina combinations of speed and accelration could cause
///                oscillation about the target position.
/// \version 1.27  Added stop() function to stop as fast as possible with current acceleration parameters.
///                Also added new Quickstop example showing its use.
/// \version 1.28  Fixed another problem where certain combinations of speed and accelration could cause
///                oscillation about the target position.
///                Added support for 3 wire full and half steppers such as Hard Disk Drive spindle.
///                Contributed by Yuri Ivatchkovitch.
/// \version 1.29  Fixed a problem that could cause a DRIVER stepper to continually step
///                with some sketches. Reported by Vadim.
/// \version 1.30  Fixed a problem that could cause stepper to back up a few steps at the end of
///                accelerated travel with certain speeds. Reported and patched by jolo.
/// \version 1.31  Updated author and distribution location details to airspayce.com
/// \version 1.32  Fixed a problem with enableOutputs() and setEnablePin on Arduino Due that
///                prevented the enable pin changing stae correctly. Reported by Duane Bishop.
/// \version 1.33  Fixed an error in example AFMotor_ConstantSpeed.pde did not setMaxSpeed();
///                Fixed a problem that caused incorrect pin sequencing of FULL3WIRE and HALF3WIRE.
///                Unfortunately this meant changing the signature for all step*() functions.
///                Added example MotorShield, showing how to use AdaFruit Motor Shield to control
///                a 3 phase motor such as a HDD spindle motor (and without using the AFMotor library.
/// \version 1.34  Added setPinsInverted(bool pin1Invert, bool pin2Invert, bool pin3Invert, bool pin4Invert, bool enableInvert) 
///                to allow inversion of 2, 3 and 4 wire stepper pins. Requested by Oleg.
/// \version 1.35  Removed default args from setPinsInverted(bool, bool, bool, bool, bool) to prevent ambiguity with 
///                setPinsInverted(bool, bool, bool). Reported by Mac Mac.
/// \version 1.36  Changed enableOutputs() and disableOutputs() to be virtual so can be overridden.
///                Added new optional argument 'enable' to constructor, which allows you toi disable the 
///                automatic enabling of outputs at construction time. Suggested by Guido.
/// \version 1.37  Fixed a problem with step1 that could cause a rogue step in the 
///                wrong direction (or not,
///                depending on the setup-time requirements of the connected hardware). 
///                Reported by Mark Tillotson.
/// \version 1.38  run() function incorrectly always returned true. Updated function and doc so it returns true 
///                if the motor is still running to the target position.
/// \version 1.39  Updated typos in keywords.txt, courtesey Jon Magill.
/// \version 1.40  Updated documentation, including testing on Teensy 3.1
/// \version 1.41  Fixed an error in the acceleration calculations, resulting in acceleration of haldf the intended value
/// \version 1.42  Improved support for FULL3WIRE and HALF3WIRE output pins. These changes were in Yuri's original
///                contribution but did not make it into production.<br>
/// \version 1.43  Added DualMotorShield example. Shows how to use AccelStepper to control 2 x 2 phase steppers using the 
///                Itead Studio Arduino Dual Stepper Motor Driver Shield model IM120417015.<br>
/// \version 1.44  examples/DualMotorShield/DualMotorShield.ino examples/DualMotorShield/DualMotorShield.pde
///                was missing from the distribution.<br>
/// \version 1.45  Fixed a problem where if setAcceleration was not called, there was no default
///                acceleration. Reported by Michael Newman.<br>
/// \version 1.45  Fixed inaccuracy in acceleration rate by using Equation 15, suggested by Sebastian Gracki.<br>
///                Performance improvements in runSpeed suggested by Jaakko Fagerlund.<br>
/// \version 1.46  Fixed error in documentation for runToPosition().
///                Reinstated time calculations in runSpeed() since new version is reported 
///                not to work correctly under some circumstances. Reported by Oleg V Gavva.<br>
/// \version 1.48  2015-08-25
///                Added new class MultiStepper that can manage multiple AccelSteppers, 
///                and cause them all to move
///                to selected positions at such a (constant) speed that they all arrive at their
///                target position at the same time. Suitable for X-Y flatbeds etc.<br>
///                Added new method maxSpeed() to AccelStepper to return the currently configured maxSpeed.<br>
///
/// \author  Mike McCauley (mikem@airspayce.com) DO NOT CONTACT THE AUTHOR DIRECTLY: USE THE LISTS
// Copyright (C) 2009-2013 Mike McCauley
// $Id: AccelStepper.h,v 1.22 2015/08/25 02:22:45 mikem Exp mikem $

#ifndef AccelStepper_h
#define AccelStepper_h

#include <stdlib.h>
#if ARDUINO >= 100
#include <Arduino.h>
#define TIMER_FREQUENCY 1000000.0
#else
#include <wiringPi.h>
#include <stdint.h>
#define TIMER_FREQUENCY 700.0
typedef bool boolean;
#include <iostream>

template<class T>
const T& constrain(const T& x, const T& a, const T& b) {
	if (x < a) {
		return a;
	}
	else if (b < x) {
		return b;
	}
	else
		return x;
}
#endif

// These defs cause trouble on some versions of Arduino
#undef round

class AccelStepper
{
public:
    AccelStepper(uint8_t stepPin = 2, uint8_t dirPin = 3, bool enable = true);

    /// Set the target position. The run() function will try to move the motor (at most one step per call)
    /// from the current position to the target position set by the most
    /// recent call to this function. Caution: moveTo() also recalculates the speed for the next step. 
    /// If you are trying to use constant speed movements, you should call setSpeed() after calling moveTo().
    /// \param[in] absolute The desired absolute position. Negative is
    /// anticlockwise from the 0 position.
    void    moveTo(long absolute); 

    /// Set the target position relative to the current position
    /// \param[in] relative The desired position relative to the current position. Negative is
    /// anticlockwise from the current position.
    void    move(long relative);

    /// Poll the motor and step it if a step is due, implementing
    /// accelerations and decelerations to acheive the target position. You must call this as
    /// frequently as possible, but at least once per minimum step time interval,
    /// preferably in your main loop. Note that each call to run() will make at most one step, and then only when a step is due,
    /// based on the current speed and the time since the last step.
    /// \return true if the motor is still running to the target position.
    boolean run();

    /// Poll the motor and step it if a step is due, implementing a constant
    /// speed as set by the most recent call to setSpeed(). You must call this as
    /// frequently as possible, but at least once per step interval,
    /// \return true if the motor was stepped.
    boolean runSpeed();

    /// Sets the maximum permitted speed. The run() function will accelerate
    /// up to the speed set by this function.
    /// Caution: the maximum speed achievable depends on your processor and clock speed.
    /// \param[in] speed The desired maximum speed in steps per second. Must
    /// be > 0. Caution: Speeds that exceed the maximum speed supported by the processor may
    /// Result in non-linear accelerations and decelerations.
    void    setMaxSpeed(float speed);

    /// returns the maximum speed configured for this stepper
    /// that was previously set by setMaxSpeed();
    /// \return The currently configured maximum speed
    float   maxSpeed();

    /// Sets the desired constant speed for use with runSpeed().
    /// \param[in] speed The desired constant speed in steps per
    /// second. Positive is clockwise. Speeds of more than 1000 steps per
    /// second are unreliable. Very slow speeds may be set (eg 0.00027777 for
    /// once per hour, approximately. Speed accuracy depends on the Arduino
    /// crystal. Jitter depends on how frequently you call the runSpeed() function.
    void    setSpeed(float speed);

    /// The most recently set speed
    /// \return the most recent speed in steps per second
    float   speed();

    /// The distance from the current position to the target position.
    /// \return the distance from the current position to the target position
    /// in steps. Positive is clockwise from the current position.
    long    distanceToGo();

    /// The most recently set target position.
    /// \return the target position
    /// in steps. Positive is clockwise from the 0 position.
    long    targetPosition();

    /// The currently motor position.
    /// \return the current motor position
    /// in steps. Positive is clockwise from the 0 position.
    long    currentPosition();  

    /// Resets the current position of the motor, so that wherever the motor
    /// happens to be right now is considered to be the new 0 position. Useful
    /// for setting a zero position on a stepper after an initial hardware
    /// positioning move.
    /// Has the side effect of setting the current motor speed to 0.
    /// \param[in] position The position in steps of wherever the motor
    /// happens to be right now.
    void    setCurrentPosition(long position);  
    
    /// Moves the motor (with acceleration/deceleration) 
    /// to the target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    void    runToPosition();

    /// Runs at the currently selected speed until the target position is reached
    /// Does not implement accelerations.
    /// \return true if it stepped
    boolean runSpeedToPosition();

    /// Moves the motor (with acceleration/deceleration)
    /// to the new target position and blocks until it is at
    /// position. Dont use this in event loops, since it blocks.
    /// \param[in] position The new target position.
    void    runToNewPosition(long position);

    /// Sets a new target position that causes the stepper
    /// to stop as quickly as possible, using the current speed and acceleration parameters.
    void stop();

    /// Disable motor pin outputs by setting them all LOW
    /// Depending on the design of your electronics this may turn off
    /// the power to the motor coils, saving power.
    /// This is useful to support Arduino low power modes: disable the outputs
    /// during sleep and then reenable with enableOutputs() before stepping
    /// again.
    virtual void    disableOutputs();

    /// Enable motor pin outputs by setting the motor pins to OUTPUT
    /// mode. Called automatically by the constructor.
    virtual void    enableOutputs();

    /// Sets the minimum pulse width allowed by the stepper driver. The minimum practical pulse width is 
    /// approximately 20 microseconds. Times less than 20 microseconds
    /// will usually result in 20 microseconds or so.
    /// \param[in] minWidth The minimum pulse width in microseconds. 
    void    setMinPulseWidth(unsigned int minWidth);

    /// Sets the enable pin number for stepper drivers.
    /// 0xFF indicates unused (default).
    /// Otherwise, if a pin is set, the pin will be turned on when 
    /// enableOutputs() is called and switched off when disableOutputs() 
    /// is called.
    /// \param[in] enablePin Arduino digital pin number for motor enable
    /// \sa setPinsInverted
    void    setEnablePin(uint8_t enablePin = 0xff);

protected:

    /// \brief Direction indicator
    /// Symbolic names for the direction the motor is turning
    typedef enum
    {
		DIRECTION_CCW = 0,  ///< Clockwise
        DIRECTION_CW  = 1   ///< Counter-Clockwise
    } Direction;

    virtual void   step(long step);

private:
    uint8_t        _stepPin;

	uint8_t        _dirPin;

    /// The current absolution position in steps.
    long           _currentPos;    // Steps

    /// The target position in steps. The AccelStepper library will move the
    /// motor from the _currentPos to the _targetPos, taking into account the
    /// max speed, acceleration and deceleration
    long           _targetPos;     // Steps

    /// The current motos speed in steps per second
    /// Positive is clockwise
    float          _speed;         // Steps per second

    /// The maximum permitted speed in steps per second. Must be > 0.
    float          _maxSpeed;

    /// The minimum allowed pulse width in microseconds
    unsigned int   _minPulseWidth;

    /// Is the enable pin inverted?
    bool           _enableInverted;

    /// Enable pin for stepper driver, or 0xFF if unused.
    uint8_t        _enablePin;

    /// Current direction motor is spinning in
    boolean _direction; // 1 == CW

	unsigned int  _lastStepTime;
};

#endif 
