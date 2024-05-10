/*
 * wiringPi.H:
 *	Arduino like Wiring library for the Raspberry Pi.
 *	Copyright (c) 2012-2017 Gordon Henderson
 ***********************************************************************
 * This file is part of wiringPi:
 *	https://github.com/WiringPi/WiringPi/
 *
 *    wiringPi is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU Lesser General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    wiringPi is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Lesser General Public License for more details.
 *
 *    You should have received a copy of the GNU Lesser General Public License
 *    along with wiringPi.  If not, see <http://www.gnu.org/licenses/>.
 ***********************************************************************
 */

#ifndef __WIRING_PI_H__
#define __WIRING_PI_H__

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <asm/ioctl.h>

// #include "softPwm.H"
// #include "softTone.H"

#include "../version.H"

#include "wpiTypedefs.H"

// C doesn't have true/false by default and I can never remember which
//	way round they are, so ...
//	(and yes, I know about stdbool.H but I like capitals for these and I'm old)

// GCC warning suppressor

#define UNU __attribute__((unused))

// Mask for the bottom 64 pins which belong to the Raspberry Pi
//	The others are available for the other devices

// #define PI_GPIO_MASK (0xFFFFFFC0)

constexpr pinLabel PI_GPIO_MASK = 0xFFFFFFC0;

// Handy defines

// wiringPi modes

#define WPI_MODE_PINS 0
#define WPI_MODE_GPIO 1
#define WPI_MODE_GPIO_SYS 2
#define WPI_MODE_PHYS 3
#define WPI_MODE_PIFACE 4
#define WPI_MODE_UNINITIALISED -1

// Pin modes

#define INPUT 0
#define OUTPUT 1
#define PWM_OUTPUT 2
#define GPIO_CLOCK 3
#define SOFT_PWM_OUTPUT 4
#define SOFT_TONE_OUTPUT 5
#define PWM_TONE_OUTPUT 6

// #define LOW 0
// #define HIGH 1

// Pull up/down/none

#define PUD_OFF 0
#define PUD_DOWN 1
#define PUD_UP 2

// PWM

#define PWM_MODE_MS 0
#define PWM_MODE_BAL 1

// Interrupt levels

#define INT_EDGE_SETUP 0
#define INT_EDGE_FALLING 1
#define INT_EDGE_RISING 2
#define INT_EDGE_BOTH 3

// Pi model types and version numbers
//	Intended for the GPIO program Use at your own risk.
// https://www.raspberrypi.com/documentation/computers/raspberry-pi.html#new-style-revision-codes

#define PI_MODEL_A 0
#define PI_MODEL_B 1
#define PI_MODEL_AP 2
#define PI_MODEL_BP 3
#define PI_MODEL_2 4
#define PI_ALPHA 5
#define PI_MODEL_CM 6
#define PI_MODEL_07 7
#define PI_MODEL_3B 8
#define PI_MODEL_ZERO 9
#define PI_MODEL_CM3 10
#define PI_MODEL_ZERO_W 12
#define PI_MODEL_3BP 13
#define PI_MODEL_3AP 14
#define PI_MODEL_CM3P 16
#define PI_MODEL_4B 17
#define PI_MODEL_ZERO_2W 18
#define PI_MODEL_400 19
#define PI_MODEL_CM4 20

#define PI_VERSION_1 0
#define PI_VERSION_1_1 1
#define PI_VERSION_1_2 2
#define PI_VERSION_2 3

#define PI_MAKER_SONY 0
#define PI_MAKER_EGOMAN 1
#define PI_MAKER_EMBEST 2
#define PI_MAKER_UNKNOWN 3

constexpr const char *piModelNames[21] = {
    "Model A",    //  0
    "Model B",    //  1
    "Model A+",   //  2
    "Model B+",   //  3
    "Pi 2",       //  4
    "Alpha",      //  5
    "CM",         //  6
    "Unknown07",  // 07
    "Pi 3",       // 08
    "Pi Zero",    // 09
    "CM3",        // 10
    "Unknown11",  // 11
    "Pi Zero-W",  // 12
    "Pi 3B+",     // 13
    "Pi 3A+",     // 14
    "Unknown15",  // 15
    "CM3+",       // 16
    "Pi 4B",      // 17
    "Pi Zero2-W", // 18
    "Pi 400",     // 19
    "CM4",        // 20
};

constexpr const char *piRevisionNames[16] = {
    "00",
    "01",
    "02",
    "03",
    "04",
    "05",
    "06",
    "07",
    "08",
    "09",
    "10",
    "11",
    "12",
    "13",
    "14",
    "15",
};

constexpr const char *piMakerNames[16] = {
    "Sony",      //	 0
    "Egoman",    //	 1
    "Embest",    //	 2
    "Unknown",   //	 3
    "Embest",    //	 4
    "Unknown05", //	 5
    "Unknown06", //	 6
    "Unknown07", //	 7
    "Unknown08", //	 8
    "Unknown09", //	 9
    "Unknown10", //	10
    "Unknown11", //	11
    "Unknown12", //	12
    "Unknown13", //	13
    "Unknown14", //	14
    "Unknown15", //	15
};

constexpr const int piMemorySize[8] = {
    256,  //	 0
    512,  //	 1
    1024, //	 2
    2048, //	 3
    4096, //	 4
    8192, //	 5
    0,    //	 6
    0,    //	 7
};

//	Intended for the GPIO program Use at your own risk.

// Threads

#define PI_THREAD(X) void *X(UNU void *dummy)

// Failure modes

#define WPI_FATAL (1 == 1)
#define WPI_ALMOST (1 == 2)

// wiringPiNodeStruct:
//	This describes additional device nodes in the extended wiringPi
//	2.0 scheme of things.
//	It's a simple linked list for now, but will hopefully migrate to
//	a binary tree for efficiency reasons - but then again, the chances
//	of more than 1 or 2 devices being added are fairly slim, so who
//	knows....

struct wiringPiNodeStruct
{
    pinLabel pinBase;
    pinLabel pinMax;

    int fd;             // Node specific
    unsigned int data0; //  ditto
    unsigned int data1; //  ditto
    // unsigned int data2; //  ditto
    int data2;
    unsigned int data3; //  ditto

    void (*pinMode)(struct wiringPiNodeStruct *node, pinLabel pin, int mode);
    void (*pullUpDnControl)(struct wiringPiNodeStruct *node, pinLabel pin, int mode);
    pinLabel (*digitalRead)(struct wiringPiNodeStruct *node, pinLabel pin);
    void (*digitalWrite)(struct wiringPiNodeStruct *node, pinLabel pin, int value);
    void (*pwmWrite)(struct wiringPiNodeStruct *node, pinLabel pin, int value);
    int (*analogRead)(struct wiringPiNodeStruct *node, pinLabel pin);
    void (*analogWrite)(struct wiringPiNodeStruct *node, pinLabel pin, int value);

    struct wiringPiNodeStruct *next;
};

extern struct wiringPiNodeStruct *wiringPiNodes;

// Export variables for the hardware pointers

extern volatile unsigned int *_wiringPiGpio;
extern volatile unsigned int *_wiringPiPwm;
extern volatile unsigned int *_wiringPiClk;
extern volatile unsigned int *_wiringPiPads;
extern volatile unsigned int *_wiringPiTimer;
extern volatile unsigned int *_wiringPiTimerIrqRaw;

// Function prototypes
//	c++ wrappers thanks to a comment by Nick Lott
//	(and others on the Raspberry Pi forums)

// Data

// Internal

extern int wiringPiFailure(const int fatal, const char *message, ...);

// Core wiringPi functions

extern struct wiringPiNodeStruct *wiringPiFindNode(const pinLabel pin);                            // Implemented
extern struct wiringPiNodeStruct *wiringPiNewNode(const pinLabel pinBase, const pinLabel numPins); // Implemented

extern void wiringPiVersion(int *major, int *minor); // Implemented
extern int wiringPiSetup(void);                      // Implemented
extern int wiringPiSetupSys(void);                   // Implemented
extern int wiringPiSetupGpio(void);                  // Implemented
extern int wiringPiSetupPhys(void);                  // Implemented

extern void pinModeAlt(pinLabel pin, const pinLabel mode);
extern void pinMode(pinLabel pin, const int mode);
extern void pullUpDnControl(pinLabel pin, const int pud);
extern pinLabel digitalRead(pinLabel pin);                    // Implemented
extern void digitalWrite(pinLabel pin, const int value);      // Implemented
extern void pwmWrite(pinLabel pin, const int value);          // Implemented
extern int analogRead(const pinLabel pin);                    // Implemented
extern void analogWrite(const pinLabel pin, const int value); // Implemented

// PiFace specifics
//	(Deprecated)

// On-Board Raspberry Pi hardware specific stuff

extern int piGpioLayout(void);
extern int piBoardRev(void); // Deprecated
extern void piBoardId(int *model, int *rev, int *mem, int *maker, int *overVolted);
extern unsigned int wpiPinToGpio(const int wpiPin);
extern unsigned int physPinToGpio(const int physPin);
extern void setPadDrive(const unsigned int group, const unsigned int value);
extern int getAlt(pinLabel pin);
extern void pwmToneWrite(pinLabel pin, const pinLabel freq);
extern void pwmSetMode(const int mode);
extern void pwmSetRange(const unsigned int range);
extern void pwmSetClock(unsigned int divisor);
extern void gpioClockSet(pinLabel pin, const int freq);
extern unsigned int digitalReadByte(void);
extern unsigned int digitalReadByte2(void);
extern void digitalWriteByte(int value);
extern void digitalWriteByte2(int value);

// Interrupts
//	(Also Pi hardware specific)

extern int waitForInterrupt(pinLabel pin, int mS);
extern int wiringPiISR(pinLabel pin, int mode, void (*function)(void));

// Threads

extern int piThreadCreate(void *(*fn)(void *));
extern void piLock(int key);
extern void piUnlock(int key);

// Schedulling priority

extern int piHiPri(const int pri);

// Extras from arduino land

extern void delay(unsigned int howLong);               // Implemented
extern void delayMicroseconds(const __time_t howLong); // Implemented
extern unsigned int millis(void);                      // Implemented
extern unsigned int micros(void);                      // Implemented

#endif
