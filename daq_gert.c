/*
 *     comedi/drivers/daq_gert.c
 *
 *     COMEDI - Linux Control and Measurement Device Interface
 *     Copyright (C) 1998 David A. Schleef <ds@schleef.org>
 *
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 *
 *     This program is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License
 *     along with this program; if not, write to the Free Software
 *     Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */

/*
Driver: "experimental" daq_gert in progress ... for 3.18+ kernels
Test program code in /fujitsu/nidaq700/raspigert/bmc/bmc 
 *** edit arch/arm/mach-bcm2709/bcm2709.c to add spigert device info to spidev info
 *** edit arch/arm/mach-bcm2708/bcm2708.c
recompile kernel

	{
		.modalias = "spidev",
		.max_speed_hz = 500000,
		.bus_num = 1,
		.chip_select = 0,
		.mode = SPI_MODE_0,
	}, {
		.modalias = "spidev",
		.max_speed_hz = 500000,
		.bus_num = 1,
		.chip_select = 1,
		.mode = SPI_MODE_0,
	},
	{
		.modalias = "spigert",
		.max_speed_hz = 500000,
		.bus_num = 0,
		.chip_select = 0,
		.mode = SPI_MODE_0,
	}, {
		.modalias = "spigert",
		.max_speed_hz = 500000,
		.bus_num = 0,
		.chip_select = 1,
		.mode = SPI_MODE_0,
	}
 * 
 * 
 *  Added daq_gert.o to the COMEDI_MISC_DRIVERS
 *  comedi/drivers/Makefile
 *  obj-$(CONFIG_COMEDI_DAQ_GERT)           += daq_gert.o
 *  Added daq_gert to the comedi Kconfig file
 * 

 * section COMEDI_MISC_DRIVERS

 config COMEDI_DAQ_GERT
	tristate "GERTBOARD support"
	depends on COMEDI_KCOMEDILIB
	---help---
       Enable support for a raspi gertboard

       To compile this driver as a module, choose M here: the module will be
       called daq_gert.

 * 
 * 
Description: GERTBOARD daq-gert
Author: Fred Brooks <spam@sma2.rain.com>
 * 
Most of the actual GPIO setup code was copied from
 * 
WiringPI 

 ***********************************************************************
 * This file is part of wiringPi:
 *      https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 * 
 Also 
 * 
 * Driver for Broadcom BCM2708 SPI Controllers
 *
 * Copyright (C) 2012 Chris Boot
 *
 * This driver is inspired by:
 * spi-ath79.c, Copyright (C) 2009-2011 Gabor Juhos <juhosg@openwrt.org>
 * spi-atmel.c, Copyright (C) 2006 Atmel Corporation

Devices: [] GERTBOARD (daq_gert)
Status: inprogress (DIO 95%) (AI 90%) AO (90%) (My code cleanup 75%)
Updated: Apr 2015 12:07:20 +0000

The DAQ-GERT appears in Comedi as a  digital I/O subdevice (0) with
17 or 21 or 30 channels, 
a analog input subdevice (1) with 2 single-ended channels with onboard adc, OR
a analog input subdevice (1) with single-ended channels set by the SPI slave device
a analog output subdevice(2) with 2 channels with onboard dac

Digital:  The comedi channel 0 corresponds to the GPIO WPi table order
channel numbers [0..7] will be outputs, [8..16/20/29] will be inputs
 * 0/2
 * 1/3
 * 4
 * 7    SPI CE1
 * 8    SPI CE0
 * 9    SPI SO
 * 10   SPI SI
 * 11   SPI CLK
 * 14   UART
 * 15   UART
 * 17
 * 18   PWM
 * 21/27
 * 22
 * 23
 * 24
 * 25
 * 
 The BCM2835 has 54 GPIO pins.
      BCM2835 data sheet, Page 90 onwards.
      There are 6 control registers, each control the functions of a block
      of 10 pins.
      Each control register has 10 sets of 3 bits per GPIO pin:

      000 = GPIO Pin X is an input
      001 = GPIO Pin X is an output
      100 = GPIO Pin X takes alternate function 0
      101 = GPIO Pin X takes alternate function 1
      110 = GPIO Pin X takes alternate function 2
      111 = GPIO Pin X takes alternate function 3
      011 = GPIO Pin X takes alternate function 4
      010 = GPIO Pin X takes alternate function 5

 So the 3 bits for port X are:
      X / 10 + ((X % 10) * 3)

Digital direction configuration: [0..1] are input only due to pullups,
 * all other ports can be input or outputs

Analog: The type and resolution of the onboard ADC/DAC chips are set
by the module option variable daqgert_conf
0 = Factory Gertboard configuratin of MCP3002 ADC and MCP4802 ADC: 10bit in/8bit out
1 = MCP3202 ADC and MCP4822 DAC: 12bit in/12bit out 
2 = MCP3002 ADC and MCP4822 DAC: 10bit in/12bit out
3 = MCP3202 ADC and MCP4802 DAC: 12bit in/8bit out
The input  range is 0 to 1023/4095 for 0.0 to 3.3(Vdd) onboard devices or 2.048 volts/Vdd for PIC slaves 
The output range is 0 to 4095 for 0.0 to 2.048 onboard devices (output resolution depends on the device)

 *  PIC Slave Info:
 * 
 * bit 7 high for commands sent from the MASTER
 * bit 6 0 send lower or 1 send upper byte ADC result first
 * bits 3..0 port address
 * all zeros sent to the PIC slave will return 8 bits from the ADC buffer
 *
 * bit 7 low  for config data sent in CMD_DUMMY per uC type
 * bits 6 config bit code always 1
 * bit	5 0=ADC ref VDD, 1=ADC rec FVR=2.048
 * bit  4 0=10bit adc, 1=12bit adc
 * bits 3..0 number of ADC channels
 * 
 */

#include "../comedidev.h"
#include "comedi_fc.h"
#include "../comedidev.h"
#include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spi/spi.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/timer.h>
#include "8253.h"

/* Function stubs */
void (*pinMode) (int pin, int mode);
void (*digitalWrite) (int pin, int value);
void (*setPadDrive) (int group, int value);
int (*digitalRead) (int pin);
static int daqgert_spi_probe(struct comedi_device *);
static void daqgert_ai_clear_eoc(struct comedi_device *);
static int daqgert_ai_cancel(struct comedi_device *,
	struct comedi_subdevice *);
static void daqgert_handle_eoc(struct comedi_device *,
	struct comedi_subdevice *);
static void my_timer_callback(unsigned long);
static void daqgert_ai_set_chan_range(struct comedi_device *,
	unsigned int, char);
static unsigned int daqgert_ai_get_sample(struct comedi_device *,
	struct comedi_subdevice *);

/* analog chip types (type - 12 bits) */
#define MCP3002 2 /* 10 bit ADC */
#define MCP3202 0
#define MCP4802 4 /* 8 bit DAC output from 12 bit input data */
#define MCP4812 2
#define MCP4822 0
#define PICSL10 2
#define PICSL12 0
#define NUM_AI_CHAN 2
#define NUM_AO_CHAN 2

static int daqgert_conf = 0;
module_param(daqgert_conf, int, S_IRUGO);
static int pullups = 2;
module_param(pullups, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
static int gpiosafe = 1;
module_param(gpiosafe, int, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
static int dio_conf = 0;
module_param(dio_conf, int, S_IRUGO);
static int count = 0;
module_param(count, int, S_IRUGO);

/* kthread */
static struct timer_list my_timer;
static struct task_struct *daqgert_task;
static struct mutex daqgert_platform_lock;

struct comedi_control {
	u8 *tx_buff;
	u8 *rx_buff;
};

struct spi_param_type {
	uint16_t range : 1;
	uint16_t bits : 2;
	uint16_t link : 1;
	uint16_t pic18 : 2;
	uint16_t chan : 4;
	struct spi_device *spi;
	struct comedi_device *dev;
	int device_type;
	int multi_mode; /* combine transfers into one message */
	int multi_size; /* the number of needed values returned as data */
};
static struct spi_param_type spi_adc = {
	.device_type = MCP3002,
	.multi_mode = false
}, spi_dac = {
	.device_type = MCP4802,
	.multi_mode = false
};

struct daqgert_private {
	unsigned int RPisys_rev;
	uint16_t conv_delay_usecs, cmd_delay_usecs, ai_neverending;
	int chan, timer, run, spi_run, count, cmd_running, cmd_canceled;
	struct mutex drvdata_lock, cmd_lock;
	unsigned int val;
	unsigned int ai_scans; /*  length of scanlist */
	unsigned int ai_act_scan; /*  how many scans we finished */
};

static struct daqgert_private daqgert_info = {
	.chan = 0,
	.timer = 0,
	.run = 0,
	.spi_run = 0,
	.count = 0,
	.cmd_running = 0,
	.cmd_canceled = 0,
	.cmd_delay_usecs = 10,
	.conv_delay_usecs = 30,
	.ai_neverending = 1,
	.val = 0
};

#define CSnA    0       /* GPIO 8  Gertboard ADC */
#define CSnB    1       /* GPIO 7  Gertboard DAC */

#define SPI_BUFF_SIZE 8192
#define MAX_CHANLIST_LEN	256

/* PIC Slave commands */
#define CMD_ZERO        0b00000000
#define CMD_ADC_GO	0b10000000
#define CMD_PORT_GO	0b10100000	// send data LO_NIBBLE to port buffer
#define CMD_CHAR_GO	0b10110000	// send data LO_NIBBLE to TX buffer
#define CMD_ADC_DATA	0b11000000
#define CMD_PORT_DATA	0b11010000	// send data HI_NIBBLE to port buffer ->PORT and return input PORT data in received SPI data byte
#define CMD_CHAR_DATA	0b11100000	// send data HI_NIBBLE to TX buffer and return RX buffer in received SPI data byte
#define CMD_XXXX	0b11110000	//
#define CMD_CHAR_RX	0b00010000	// Get RX buffer
#define CMD_DUMMY_CFG	0b01000000	// stuff config data in SPI buffer
#define CMD_DEAD        0b11111111      // This is usually a bad response

#define WPI_MODE_PINS            0
#define WPI_MODE_GPIO            1
#define WPI_MODE_GPIO_SYS        2
#define WPI_MODE_PIFACE          3

#define PIN_SAFE_MASK   0b00000000000000000111111100000000

#define INPUT            0
#define OUTPUT           1
#define PWM_OUTPUT       2

#define LOW              0
#define HIGH             1

#define PUD_OFF          0
#define PUD_DOWN         1
#define PUD_UP           2

#ifndef TRUE
#define TRUE    (1==1)
#define FALSE   (1==2)
#endif

// Port function select bits
#define FSEL_INPT               0b000
#define FSEL_OUTP               0b001
#define FSEL_ALT0               0b100
#define FSEL_ALT0               0b100
#define FSEL_ALT1               0b101
#define FSEL_ALT2               0b110
#define FSEL_ALT3               0b111
#define FSEL_ALT4               0b011
#define FSEL_ALT5               0b010

/* driver hardware numbers */
#define NUM_DIO_CHAN  17
#define NUM_DIO_CHAN_REV2       17
#define NUM_DIO_CHAN_REV3       17
#define NUM_DIO_OUTPUTS 8
#define DIO_PINS_DEFAULT        0xff

// Timer
//      Word offsets

#define TIMER_LOAD      (0x400 >> 2)
#define TIMER_VALUE     (0x404 >> 2)
#define TIMER_CONTROL   (0x408 >> 2)
#define TIMER_IRQ_CLR   (0x40C >> 2)
#define TIMER_IRQ_RAW   (0x410 >> 2)
#define TIMER_IRQ_MASK  (0x414 >> 2)
#define TIMER_RELOAD    (0x418 >> 2)
#define TIMER_PRE_DIV   (0x41C >> 2)
#define TIMER_COUNTER   (0x420 >> 2)

/* Locals to hold pointers to the hardware */

static volatile uint32_t *gpio;

/* Global for the RPi board rev */
extern unsigned int system_rev; // from the kernel symbol table exports */
extern unsigned int system_serial_low;
extern unsigned int system_serial_high;

static const struct comedi_lrange daqgert_ai_range3_300 = {1,
	{
		RANGE(0, 3.300),
	}};
static const struct comedi_lrange daqgert_ai_range2_048 = {1,
	{
		RANGE(0, 2.048),
	}};

static const struct comedi_lrange daqgert_ao_range = {1,
	{
		RANGE(0, 2.048),
	}};

/* pin exclude list */
static int wpi_pin_safe(int pin)
{
	unsigned int pin_bit = (0x01 << pin);
	if (!gpiosafe) return TRUE;
	if (pin_bit & PIN_SAFE_MASK) return FALSE;
	return TRUE;
}

/*
 Doing it the Arduino way with lookup tables...
      Yes, it's probably more inefficient than all the bit-twidling, but it
      does tend to make it all a bit clearer. At least to me!

 pinToGpio:
      Take a Wiring pin (0 through X) and re-map it to the BCM_GPIO pin
      Cope for 2 different board revisions here
 */
static int *pinToGpio;
static int *physToGpio;

static int pinToGpioR1 [64] = {
	17, 18, 21, 22, 23, 24, 25, 4, // From the Original Wiki - GPIO 0 through 7:   wpi  0 -  7
	0, 1, // I2C  - SDA1, SCL1                            wpi  8 -  9
	8, 7, // SPI  - CE1, CE0                              wpi 10 - 11
	10, 9, 11, // SPI  - MOSI, MISO, SCLK                      wpi 12 - 14
	14, 15, // UART - Tx, Rx                                wpi 15 - 16

	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 31
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 63
};

// Revision 2:

static int pinToGpioR2 [64] = {
	17, 18, 27, 22, 23, 24, 25, 4, // From the Original Wiki - GPIO 0 through 7:   wpi  0 -  7
	2, 3, // I2C  - SDA0, SCL0                            wpi  8 -  9
	8, 7, // SPI  - CE1, CE0                              wpi 10 - 11
	10, 9, 11, // SPI  - MOSI, MISO, SCLK                      wpi 12 - 14
	14, 15, // UART - Tx, Rx                                wpi 15 - 16
	28, 29, 30, 31, // Rev 2: New GPIOs 8 though 11                 wpi 17 - 20
	5, 6, 13, 19, 26, // B+                                           wpi 21, 22, 23, 24, 25
	12, 16, 20, 21, // B+                                           wpi 26, 27, 28, 29
	0, 1, // B+                                           wpi 30, 31

	// Padding:

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 63
};

// physToGpio:
//      Take a physical pin (1 through 26) and re-map it to the BCM_GPIO pin
//      Cope for 2 different board revisions here.
//      Also add in the P5 connector, so the P5 pins are 3,4,5,6, so 53,54,55,56

static int *physToGpio;

static int physToGpioR1 [64] = {
	-1, // 0
	-1, -1, // 1, 2
	0, -1,
	1, -1,
	4, 14,
	-1, 15,
	17, 18,
	21, -1,
	22, 23,
	-1, 24,
	10, -1,
	9, 25,
	11, 8,
	-1, 7, // 25, 26

	-1, -1, -1, -1, -1, // ... 31
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 47
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, // ... 63
};

static int physToGpioR2 [64] = {
	-1, // 0
	-1, -1, // 1, 2
	2, -1,
	3, -1,
	4, 14,
	-1, 15,
	17, 18,
	27, -1,
	22, 23,
	-1, 24,
	10, -1,
	9, 25,
	11, 8,
	-1, 7, // 25, 26

	// B+

	0, 1,
	5, -1,
	6, 12,
	13, -1,
	19, 16,
	26, 20,
	-1, 21,

	// the P5 connector on the Rev 2 boards:

	-1, -1,
	-1, -1,
	-1, -1,
	-1, -1,
	-1, -1,
	28, 29,
	30, 31,
	-1, -1,
	-1, -1,
	-1, -1,
	-1, -1,
};

// gpioToGPFSEL:
//      Map a BCM_GPIO pin to it's Function Selection
//      control port. (GPFSEL 0-5)
//      Groups of 10 - 3 bits per Function - 30 bits per port

static uint8_t gpioToGPFSEL [] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};


// gpioToShift
//      Define the shift up for the 3 bits per pin in each GPFSEL port

static uint8_t gpioToShift [] = {
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
};


// gpioToGPSET:
//      (Word) offset to the GPIO Set registers for each GPIO pin

static uint8_t gpioToGPSET [] = {
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

// gpioToGPCLR:
//      (Word) offset to the GPIO Clear registers for each GPIO pin

static uint8_t gpioToGPCLR [] = {
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
};


// gpioToGPLEV:
//      (Word) offset to the GPIO Input level registers for each GPIO pin

static uint8_t gpioToGPLEV [] = {
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
};

// GPPUD:
//      GPIO Pin pull up/down register

#define GPPUD   37

// gpioToPUDCLK
//      (Word) offset to the Pull Up Down Clock regsiter

static uint8_t gpioToPUDCLK [] = {
	38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38, 38,
	39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39, 39,
};

/*
 * pullUpDownCtrl:
 *      Control the internal pull-up/down resistors on a GPIO pin
 *      The Arduino only has pull-ups and these are enabled by writing 1
 *      to a port when in input mode - this paradigm doesn't quite apply
 *      here though.
 *********************************************************************************
 */

static void pullUpDnControl(int pin, int pud)
{
	pin = pinToGpio [pin];

	*(gpio + GPPUD) = pud & 3;
	udelay(5);
	*(gpio + gpioToPUDCLK [pin]) = 1 << (pin & 31);
	udelay(5);

	*(gpio + GPPUD) = 0;
	udelay(5);
	*(gpio + gpioToPUDCLK [pin]) = 0;
	udelay(5);
}

/*
 * pinMode:
 *      Sets the mode of a pin to be input, output
 ************************************************************
 */

static void pinModeGpio(int pin, int mode)
{
	int fSel, shift;

	pin &= 63;

	fSel = gpioToGPFSEL [pin];
	shift = gpioToShift [pin];

	/**/ if (mode == INPUT) /* Sets bits to zero = input */
		*(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift));
	else if (mode == OUTPUT)
		*(gpio + fSel) = (*(gpio + fSel) & ~(7 << shift)) | (1 << shift);
}

static void pinModeWPi(int pin, int mode)
{
	pinModeGpio(pinToGpio [pin & 63], mode);
}

/* physPinToGpio:
 *      Translate a physical Pin number to native GPIO pin number.
 *      Provided for external support.
 *********************************************************************************
 */

static int physPinToGpio(int physPin)
{
	return physToGpio [physPin & 63];
}

/*
 * digitalWrite:
 *      Set an output bit
 *****************************************************************
 */

static void digitalWriteWPi(int pin, int value)
{
	pin = pinToGpio [pin & 63];

	if (value == LOW)
		*(gpio + gpioToGPCLR [pin]) = 1 << (pin & 31);
	else
		*(gpio + gpioToGPSET [pin]) = 1 << (pin & 31);
}

static void digitalWriteGpio(int pin, int value)
{
	pin &= 63;

	if (value == LOW)
		*(gpio + gpioToGPCLR [pin]) = 1 << (pin & 31);
	else
		*(gpio + gpioToGPSET [pin]) = 1 << (pin & 31);
}

/*
 * digitalRead:
 *      Read the value of a given Pin, returning HIGH or LOW
 *******************************************************************
 */

static int digitalReadWPi(int pin)
{
	pin = pinToGpio [pin & 63];

	if ((*(gpio + gpioToGPLEV [pin]) & (1 << (pin & 31))) != 0)
		return HIGH;
	else
		return LOW;
}

static int digitalReadGpio(int pin)
{
	pin &= 63;

	if ((*(gpio + gpioToGPLEV [pin]) & (1 << (pin & 31))) != 0)
		return HIGH;
	else
		return LOW;
}

/*
 * piBoardRev:
 *	Return a number representing the hardware revision of the board.
 *	Revision is currently 1,2 or 3. -1 is returned on error.
SRRR MMMM PPPP TTTT TTTT VVVV

S scheme (0=old, 1=new)
R RAM (0=256, 1=512, 2=1024)
M manufacturer (0='SONY',1='EGOMAN',2='EMBEST',3='UNKNOWN',4='EMBEST')
P processor (0=2835, 1=2836)
T type (0='A', 1='B', 2='A+', 3='B+', 4='Pi 2 B', 5='Alpha', 6='Compute Module')
V revision (0-15)

1010 0010 0001 0000 0100 0001
SRRR MMMM PPPP TTTT TTTT VVVV

S=1 new scheme
R=2 1024 MB
M=2 EMBEST
P=1 2836
T=4 Pi 2 B
V=1 1
 *********************************************************************
 */

static int piBoardRev(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;
	int r = -1, nscheme = 0;
	static int boardRev = -1;

	if (boardRev != -1)
		return boardRev;

	if (devpriv->RPisys_rev & 0x800000) {
		nscheme = 1;
		r = devpriv->RPisys_rev & 0xf;
		dev_info(dev->class_dev, "RPi Board new scheme Rev %x, Serial %08x%08x, New Rev %x\n",
			devpriv->RPisys_rev, system_serial_high, system_serial_low, r);
	} else {
		r = devpriv->RPisys_rev & 0xff;
		dev_info(dev->class_dev, "RPi Board old scheme Rev %x, Serial %08x%08x\n",
			devpriv->RPisys_rev, system_serial_high, system_serial_low);
	}

	if (nscheme) {
		switch (r) {
		case 1:
			boardRev = 3;
			break;
		default:
			boardRev = -1;
		}
	} else {
		switch (r) {
		case 2:
		case 3:
			boardRev = 1;
			break;
		case 0:
		case 1:
			boardRev = -1;
			break;
		default:
			boardRev = 2;
		}
	}

	dev_info(dev->class_dev, "Comedi Gpio board rev %u\n",
		boardRev);
	dio_conf = boardRev;
	return boardRev;
}

/*
 * wiringPiSetup:
 *	Must be called once at the start of your program execution.
 *
 * Default setup: Initialises the system into wiringPi Pin mode and uses the
 *	memory mapped hardware directly.
 ************************************************************************
 */

static int wiringPiSetup(struct comedi_device *dev)
{
	int boardRev;

	pinMode = pinModeWPi;
	digitalWrite = digitalWriteWPi;
	digitalRead = digitalReadWPi;

	if ((boardRev = piBoardRev(dev)) < 0)
		return -1;

	if (boardRev == 1) {
		pinToGpio = pinToGpioR1;
		physToGpio = physToGpioR1;
	} else {
		pinToGpio = pinToGpioR2;
		physToGpio = physToGpioR2;
	}
	return 0;
}

/*
 * wiringPiSetupGpio:
 *	Must be called once at the start of your program execution.
 *
 * GPIO setup: Initialises the system into GPIO Pin mode and uses the
 *	memory mapped hardware directly.
 *************************************************************************
 */

static int wiringPiSetupGpio(struct comedi_device *dev)
{
	int x;

	if ((x = wiringPiSetup(dev)) < 0)
		return x;

	pinMode = pinModeGpio;
	digitalWrite = digitalWriteGpio;
	digitalRead = digitalReadGpio;

	return 0;
}

struct daqgert_board {
	const char *name;
	int board_type;
	int n_aochan;
	unsigned int ai_ns_min;
};

/* A client must be connected with a valid comedi cmd for this not to segfault */
static int daqgert_thread_function(void *data)
{
	struct comedi_device *dev = (void*) data;
	struct comedi_subdevice *s = dev->read_subdev;
	struct daqgert_private *devpriv = dev->private;

	while (!kthread_should_stop()) {
		while (!devpriv->run) {
			if (devpriv->timer) {
				schedule();
			} else {
				msleep(1);
			}
			if (devpriv->timer && devpriv->run) {
				devpriv->spi_run = true;
			}
			if (kthread_should_stop()) return 0;
		}
		if (devpriv->cmd_running) {
			daqgert_handle_eoc(dev, s);
			usleep_range(50, 250);
		} else {
			msleep(1);
		}
		devpriv->spi_run = false;
		devpriv->count++;
	}
	/*do_exit(1);*/
	return 0;

}

static void daqgert_start_pacer(struct comedi_device *dev, bool load_timers)
{
	if (load_timers) {
		/* setup timer interval to msecs */
		mod_timer(&my_timer, jiffies + msecs_to_jiffies(10));
	}
}

static void daqgert_ai_set_chan_range(struct comedi_device *dev,
	unsigned int chanspec, char wait)
{
	struct daqgert_private *devpriv = dev->private;
	mutex_lock(&devpriv->drvdata_lock);
	devpriv->chan = CR_CHAN(chanspec);
	if (wait)
		udelay(5);
	mutex_unlock(&devpriv->drvdata_lock);
}

static unsigned int daqgert_ai_get_sample(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	int chan;
	unsigned int val;
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	struct spi_transfer t[1];
	struct spi_message m;

	mutex_lock(&devpriv->drvdata_lock);
	chan = CR_CHAN(devpriv->chan);
	/* Make SPI messages for the type of ADC are we talking to */
	/* The PIC Slave needs 8 bit transfers only */
	if (spi_data->pic18) { /*  PIC18 SPI slave device. NO MULTI_MODE ever */
		udelay(devpriv->cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ADC_GO + chan;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1); /* rx buffer has old data */
		udelay(devpriv->conv_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ZERO;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1); /* rx buffer has old data */
		udelay(devpriv->cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ADC_DATA;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1);
		val = pdata->rx_buff[0];
		udelay(devpriv->cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ZERO;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1);
		val += pdata->rx_buff[0] << 8;
	} else { /* Gertboard onboard ADC device */
		if (!spi_data->multi_mode) { /* for single channel command scans */
			pdata->tx_buff[2] = 0; // format the ADC data as a single transmission
			pdata->tx_buff[1] = 0;
			pdata->tx_buff[0] = 0b11010000 | ((chan & 0x01) << 5);
			memset(&t, 0, sizeof(t)); // clear the transfer array
			t[0].tx_buf = pdata->tx_buff;
			if (spi_data->device_type == MCP3002) { // 10 bit adc data
				t[0].len = 2;
			} else {
				t[0].len = 3;
			}
			t[0].rx_buf = pdata->rx_buff;
			spi_message_init_with_transfers(&m, &t[0], 1); // make the proper message with the transfer
			spi_sync(spi_data->spi, &m); // exchange SPI data
			/* ADC type code result munging */
			if (spi_data->device_type == MCP3002) { // 10 bit adc data
				val = ((pdata->rx_buff[0] << 7) | (pdata->rx_buff[1] >> 1)) & 0x3FF;
			} else { // 12 bit adc data
				val = (pdata->rx_buff[2]&0x80) >> 7;
				val += pdata->rx_buff[1] << 1;
				val += (pdata->rx_buff[0]&0x0f) << 9;
			}
		} else {
			val = 0;
			/* todo */
		}
	}
	mutex_unlock(&devpriv->drvdata_lock);
	return val & s->maxdata;
}

static bool daqgert_ai_next_chan(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;

	//    dev_info(dev->class_dev, "ai_next_chan\n");
	s->async->events |= COMEDI_CB_BLOCK;

	s->async->cur_chan++;
	if (s->async->cur_chan >= cmd->chanlist_len) {
		s->async->cur_chan = 0;
		s->async->events |= COMEDI_CB_EOS;
		//		dev_info(dev->class_dev, "CB_EOS\n");
	}

	if (cmd->stop_src == TRIG_COUNT) {
		if (s->async->cur_chan == 0) {
			devpriv->ai_act_scan++;
			if (!devpriv->ai_neverending) {
				/* all data sampled */
				if (devpriv->ai_act_scan >= devpriv->ai_scans) {
					daqgert_ai_cancel(dev, s);
					s->async->events |= COMEDI_CB_EOA;
					//		dev_info(dev->class_dev, "CB_EOA\n");
				}
			}
		}
	}
	cfc_handle_events(dev, s);
	return true;
}

static void daqgert_handle_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	unsigned int next_chan, val;

	val = daqgert_ai_get_sample(dev, s);
	comedi_buf_put(s, val);

	next_chan = s->async->cur_chan + 1;
	if (next_chan >= cmd->chanlist_len)
		next_chan = 0;
	if (cmd->chanlist[s->async->cur_chan] != cmd->chanlist[next_chan])
		daqgert_ai_set_chan_range(dev, cmd->chanlist[next_chan], 1);

	daqgert_ai_next_chan(dev, s);
}

#if 0

static void daqgert_ai_soft_trig(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;

	devpriv->timer = TRUE;
	daqgert_start_pacer(dev, TRUE);
}

static int daqgert_ai_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned long context)
{
	struct daqgert_private *devpriv = dev->private;
	if (devpriv->spi_run) return -EBUSY;
	return 0;

}
#endif

static int daqgert_ai_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct daqgert_private *devpriv = dev->private;
	int ret = -EBUSY;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ai_cmd\n");
	if (devpriv->cmd_running) {
		dev_info(dev->class_dev, "ai_cmd busy\n");
		goto ai_cmd_exit;
	}

	if (cmd->start_src != TRIG_NOW)
		ret = -EINVAL;
	if (cmd->scan_begin_src != TRIG_FOLLOW)
		ret = -EINVAL;
	if (cmd->convert_src != TRIG_TIMER)
		ret = -EINVAL;
	if (cmd->scan_end_src != TRIG_COUNT)
		ret = -EINVAL;
	if (cmd->scan_end_arg != cmd->chanlist_len)
		ret = -EINVAL;
	if (cmd->chanlist_len > MAX_CHANLIST_LEN)
		ret = -EINVAL;
	if (ret == -EINVAL) goto ai_cmd_exit;

	if (cmd->stop_src == TRIG_COUNT) {
		devpriv->ai_scans = cmd->stop_arg;
		devpriv->ai_neverending = 0;
	} else {
		devpriv->ai_scans = 0;
		devpriv->ai_neverending = 1;
	}

	devpriv->ai_act_scan = 0;
	s->async->cur_chan = 0;
	daqgert_ai_set_chan_range(dev, cmd->chanlist[s->async->cur_chan], 1);

	devpriv->run = false;
	devpriv->timer = true;
	daqgert_start_pacer(dev, TRUE);
	devpriv->cmd_running = true;
	devpriv->cmd_canceled = false;
	ret = 0;
ai_cmd_exit:
	mutex_unlock(&devpriv->cmd_lock);
	return ret;
}

static int daqgert_ai_poll(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	int num_bytes;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ai_poll\n");

	num_bytes = comedi_buf_n_bytes_ready(s);
	mutex_unlock(&devpriv->cmd_lock);
	return num_bytes;
}

/* For a single channel scan we can do a quasi-DMA transfer that's much faster */
static int daqgert_ai_cmdtest(struct comedi_device *dev,
	struct comedi_subdevice *s, struct comedi_cmd *cmd)
{
	const struct daqgert_board *board = dev->board_ptr;

	int err = 0;
	unsigned int flags, divisor1 = 0, divisor2 = 0;
	unsigned int arg;

	//	dev_info(dev->class_dev, "ai_cmdtest\n");
	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW);
	flags = TRIG_FOLLOW;
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, flags);

	flags = TRIG_TIMER;
	err |= cfc_check_trigger_src(&cmd->convert_src, flags);

	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_NONE);

	//	dev_info(dev->class_dev, "ai_cmdtest 1\n");
	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	//	dev_info(dev->class_dev, "ai_cmdtest 2\n");
	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);
	err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);

	if (cmd->convert_src == TRIG_TIMER)
		err |= cfc_check_trigger_arg_min(&cmd->convert_arg,
		board->ai_ns_min);
	else /* TRIG_EXT */
		err |= cfc_check_trigger_arg_is(&cmd->convert_arg, 0);

	err |= cfc_check_trigger_arg_min(&cmd->chanlist_len, 1);
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= cfc_check_trigger_arg_min(&cmd->stop_arg, 1);
	else /* TRIG_NONE */
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);

	//	dev_info(dev->class_dev, "ai_cmdtest 3\n");
	if (err)
		return 3;

	/* step 4: fix up any arguments */
	if (cmd->convert_src == TRIG_TIMER) {
		arg = cmd->convert_arg;
		i8253_cascade_ns_to_timer(500000,
			&divisor1,
			&divisor2,
			&arg, cmd->flags);
		err |= cfc_check_trigger_arg_is(&cmd->convert_arg, arg);
	}

	//	dev_info(dev->class_dev, "ai_cmdtest 4\n");
	if (err)
		return 4;

	//	dev_info(dev->class_dev, "ai_cmdtest PASS\n");
	return 0;
}

static void my_timer_callback(unsigned long data)
{
	struct comedi_device *dev = (void*) data;
	struct daqgert_private *devpriv = dev->private;

	if (!devpriv->run) {
		devpriv->run = true;
		devpriv->timer = true;
	}
	daqgert_start_pacer(dev, true);

}

static void daqgert_ai_clear_eoc(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;
	int count = 0;

	del_timer_sync(&my_timer);
	setup_timer(&my_timer, my_timer_callback, (unsigned long) dev);
	devpriv->run = false;
	devpriv->timer = false;
	do { // wait if needed to SPI to clear or timeout
		msleep(1);
	} while (devpriv->spi_run || (count++ < 100));

	devpriv->run = false; // just to be sure
	devpriv->timer = false;
}

static int daqgert_ai_cancel(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;

	mutex_lock(&devpriv->cmd_lock);
	daqgert_ai_clear_eoc(dev);
	dev_info(dev->class_dev, "ai cancel\n");
	count = devpriv->count;
	s->async->cur_chan = 0;
	s->async->inttrig = NULL;
	devpriv->cmd_canceled = false;
	devpriv->cmd_running = false;
	mutex_unlock(&devpriv->cmd_lock);
	return 0;
}

/* FIXME Slow brute forced IO bits, 5us reads from userland */

/* need to use (fix) state to optimize changes */
static int daqgert_dio_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, unsigned int *data)
{
	int pinWPi;
	unsigned int val = 0, mask = 0;

	/* s->state contains the GPIO bits */
	/* s->io_bits contains the GPIO direction */

	/* i/o testing with gpio pins  */
	/* We need to shift a single bit from state to set or clear the GPIO */
	for (pinWPi = 0; pinWPi < s->n_chan; pinWPi++) {
		mask = comedi_dio_update_state(s, data);
		if (wpi_pin_safe(pinWPi)) {
			/* Do nothing on SPI AUX pins */
			if (mask) {
				if (mask & 0xffffffff)
					digitalWriteWPi(pinWPi,
					(s->state & (0x01 << pinWPi)) >> pinWPi); /* output writes */
			}
			val = s->state & 0xffffffff;
			val |= (digitalReadWPi(pinWPi) << pinWPi); /* input reads shift */
		}
		data[1] = val;
	}

	return insn->n;
}

/* query or change DIO config */
static int daqgert_dio_insn_config(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, unsigned int *data)
{
	unsigned int wpi_pin = CR_CHAN(insn->chanspec), chan = 1 << wpi_pin;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		if (wpi_pin_safe(wpi_pin)) {
			s->io_bits |= chan;
			pinModeWPi(wpi_pin, OUTPUT);
		}
		break;
	case INSN_CONFIG_DIO_INPUT:
		if (wpi_pin_safe(wpi_pin)) {
			s->io_bits &= (~chan);
			pinModeWPi(wpi_pin, INPUT);
			pullUpDnControl(wpi_pin, pullups);
		}
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] = (s->io_bits & chan) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
	default:
		return -EINVAL;
	}
	dev_dbg(dev->class_dev, "%s: GPIO wpi-pins setting 0x%x\n",
		dev->board_name,
		(unsigned int) s->io_bits);
	return 1;

}

/* Talk to the ADC via the SPI */
static int daqgert_ai_rinsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, unsigned int *data)
{
	int ret = -EBUSY;
	int n;
	struct daqgert_private *devpriv = dev->private;

	mutex_lock(&devpriv->cmd_lock);
	if (devpriv->cmd_running)
		goto ai_read_exit;

	mutex_lock(&devpriv->drvdata_lock);
	devpriv->chan = CR_CHAN(insn->chanspec);
	mutex_unlock(&devpriv->drvdata_lock);
	/* convert n samples */
	for (n = 0; n < insn->n; n++) {
		data[n] = daqgert_ai_get_sample(dev, s);
	}
	ret = 0;
ai_read_exit:
	mutex_unlock(&devpriv->cmd_lock);
	return ret ? ret : insn->n;
}

/* write to the DAC via SPI and read the last value back */
static int daqgert_ao_winsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, unsigned int *data)
{
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	unsigned int chan = CR_CHAN(insn->chanspec);
	unsigned int n, junk, val = s->readback[chan];

	mutex_lock(&devpriv->cmd_lock);
	for (n = 0; n < insn->n; n++) {
		val = data[n];
		junk = val & 0xfff; /* strip to 12 bits */
		pdata->tx_buff[1] = junk & 0xff; /* load lsb SPI data into transfer buffer */
		pdata->tx_buff[0] = (0b00110000 | ((chan & 0x01) << 7) | (junk >> 8));
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 2, pdata->rx_buff, 2); /* Load DAC channel, send two bytes */
	}
	s->readback[chan] = val;
	mutex_unlock(&devpriv->cmd_lock);
	return insn->n;
}

static int daqgert_ai_config(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct spi_param_type *spi_data = s->private;
	/* Stuff here? */
	return spi_data->chan;
}

static int daqgert_ao_config(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct spi_param_type *spi_data = s->private;
	/* Stuff here? */
	return spi_data->chan;
}

static int daqgert_attach(struct comedi_device *dev, struct comedi_devconfig *it)
{
	const struct daqgert_board *thisboard = dev->board_ptr;
	struct comedi_subdevice *s;
	int ret, num_subdev = 1, i, d;
	int num_ai_chan, num_ao_chan, num_dio_chan = NUM_DIO_CHAN;
	struct daqgert_private *devpriv;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv)); /* auto free on exit */
	if (!devpriv)
		return -ENOMEM;

	mutex_init(&devpriv->cmd_lock);
	mutex_init(&devpriv->drvdata_lock);
	
	/* Board  operation data */
	devpriv->cmd_delay_usecs = 10;
	devpriv->conv_delay_usecs = 30;
	devpriv->ai_neverending = 1;

	/* Use the kernel system_rev EXPORT_SYMBOL */
	devpriv->RPisys_rev = system_rev; /* what board are we running on? */
	if (devpriv->RPisys_rev < 2) {
		dev_err(dev->class_dev, "Invalid RPi board revision! %u\n", devpriv->RPisys_rev);
		return -EINVAL;
	}

	gpio = ioremap(GPIO_BASE, SZ_16K); /* lets get access to the GPIO base */
	if (!gpio) {
		dev_err(dev->class_dev, "Invalid gpio io base address!\n");
		return -EINVAL;
	}
	dev->iobase = GPIO_BASE; /* filler */

	/* setup the pins in a static matter for now */
	/* PIN mode for all */
	dev_info(dev->class_dev, "GertBoard WiringPiSetup\n");
	wiringPiSetup(dev);
	for (i = 0; i < NUM_DIO_OUTPUTS; i++) { /* [0..7] OUTPUTS */
		pinModeWPi(i, OUTPUT);
	}
	dev_info(dev->class_dev, "GertBoard WPi pins set [0..7] to outputs\n");
	num_dio_chan = NUM_DIO_CHAN; /* Rev 1 board setup first */
	if (piBoardRev(dev) > 1) /* This a Rev 2 or higher board "I hope" */
		num_dio_chan = NUM_DIO_CHAN_REV2;
	if (piBoardRev(dev) > 2) /* This a Rev 3 or higher board "I hope" */
		num_dio_chan = NUM_DIO_CHAN_REV3;

	/* assume we have DON"T a gertboard */
	dev_info(dev->class_dev, "GertBoard Detection Started\n");
	num_subdev = 1;
	if (daqgert_spi_probe(dev)) num_subdev += 2;
	// add AI and AO channels */
	dev_info(dev->class_dev, "GertBoard Detection Completed\n");
	dev->board_name = thisboard->name;
	ret = comedi_alloc_subdevices(dev, num_subdev);
	if (ret)
		return ret;

	/* daq_gert dio */
	s = &dev->subdevices[0];
	s->type = COMEDI_SUBD_DIO;
	s->subdev_flags = SDF_READABLE | SDF_WRITABLE;
	s->n_chan = num_dio_chan;
	s->len_chanlist = num_dio_chan;
	s->range_table = &range_digital;
	s->maxdata = 1;
	s->insn_bits = daqgert_dio_insn_bits;
	s->insn_config = daqgert_dio_insn_config;
	s->state = 0;
	s->io_bits = DIO_PINS_DEFAULT; /* set output bits */
	d = s->io_bits;

	if (num_subdev > 1) { /* we have the SPI ADC DAC on board */
		/* daq_gert ai */
		s = &dev->subdevices[1];
		s->private = &spi_adc; /* SPI adc comedi state */
		num_ai_chan = daqgert_ai_config(dev, s); /* config SPI ports for ai use */
		s->type = COMEDI_SUBD_AI;
		/* we support single-ended (ground)  */
		s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_CMD_READ | SDF_COMMON;
		s->n_chan = num_ai_chan;
		s->len_chanlist = num_ai_chan;
		s->maxdata = (1 << (12 - spi_adc.device_type)) - 1;
		if (spi_adc.range) {
			s->range_table = &daqgert_ai_range2_048;
		} else {
			s->range_table = &daqgert_ai_range3_300;
		}
		s->insn_read = daqgert_ai_rinsn;
		s->do_cmdtest = daqgert_ai_cmdtest;
		s->do_cmd = daqgert_ai_cmd;
		s->poll = daqgert_ai_poll;
		s->cancel = daqgert_ai_cancel;
		dev->read_subdev = s;

		/* setup the timer to call my_timer_callback */
		setup_timer(&my_timer, my_timer_callback, (unsigned long) dev);

		/* daq-gert ao */
		s = &dev->subdevices[2];
		s->private = &spi_dac; /* SPI dac comedi state */
		num_ao_chan = daqgert_ao_config(dev, s); /* config SPI ports for ao use */
		s->type = COMEDI_SUBD_AO;
		/* we support single-ended (ground)  */
		s->subdev_flags = SDF_WRITABLE | SDF_GROUND;
		s->n_chan = num_ao_chan;
		s->len_chanlist = num_ao_chan;
		s->maxdata = (1 << 12) - 1; /* the actual analog resolution depends on the DAC chip 8,10,12 */
		s->range_table = &daqgert_ao_range;
		s->insn_write = daqgert_ao_winsn;
		s->insn_read = comedi_readback_insn_read;
		comedi_alloc_subdev_readback(s);
	}
	/* setup kthread */
	daqgert_task = kthread_run(&daqgert_thread_function, (void *) dev, "daq_gert");
	dev_info(dev->class_dev, "Daq_gert SPI i/o thread started\n");

	dev_info(dev->class_dev, "%s attached: GPIO iobase 0x%lx, ioremap 0x%lx, GPIO wpi-pins 0x%x\n",
		dev->driver->driver_name,
		dev->iobase,
		(long unsigned int) gpio,
		(unsigned int) d);

	return 0;
}

static void daqgert_detach(struct comedi_device *dev)
{
	mutex_lock(&daqgert_platform_lock);
	if (daqgert_task) kthread_stop(daqgert_task);
	daqgert_task = NULL;

	if (1) {

		/* remove kernel timer when unloading module */
		del_timer_sync(&my_timer);
	}

	iounmap(gpio);
	dev_info(dev->class_dev, "Daq_gert detached\n");
	mutex_unlock(&daqgert_platform_lock);
}

static const struct daqgert_board daqgert_boards[] = {
	{
		.name = "daq-gert",
		.board_type = 0,
		.n_aochan = 2,
		.ai_ns_min = 10000,
	},
	{
		.name = "daq_gert",
		.board_type = 0,
		.n_aochan = 2,
		.ai_ns_min = 10000,
	},
};

static struct comedi_driver daqgert_driver = {
	.driver_name = "daq_gert",
	.module = THIS_MODULE,
	.attach = daqgert_attach,
	.detach = daqgert_detach,
	.board_name = &daqgert_boards[0].name,
	.num_names = ARRAY_SIZE(daqgert_boards),
	.offset = sizeof(struct daqgert_board),
};

static struct spi_driver spidev_spi_driver;

/* called for each listed spigert device in the bcm270*.c file */
static int spidev_spi_probe(struct spi_device *spi)
{
	struct comedi_control *pdata;
	int ret;

	pdata = kzalloc(sizeof(struct comedi_control), GFP_KERNEL);
	if (!pdata) return -ENOMEM;

	/* alloc the spi transfer buffer structures */
	spi->dev.platform_data = pdata;
	/* these buffers are large to handle async SPI transfer scans in a hunk */
	pdata->tx_buff = kzalloc(SPI_BUFF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!pdata->tx_buff) {
		ret = -ENOMEM;
		goto kfree_exit;
	}
	pdata->rx_buff = kzalloc(SPI_BUFF_SIZE, GFP_KERNEL | GFP_DMA);
	if (!pdata->rx_buff) {
		ret = -ENOMEM;
		goto kfree_tx_exit;
	}

	if (spi->chip_select == CSnA) {
		/* get a copy of the slave device 0 */ /* we need a device to talk to the ADC */
		spi_adc.spi = spi;
		spi->max_speed_hz = 1000000;
	}
	if (spi->chip_select == CSnB) {
		/* get a copy of the slave device 1 */ /* we need a device to talk to the DAC */
		spi_dac.spi = spi;
		spi->max_speed_hz = 4000000;
	}
	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_3; /* mode 3 for ADC & DAC*/
	spi_setup(spi);
	dev_info(&spi->dev,
		"setup: cd %d: %d Hz, bpw %u, mode 0x%x\n",
		spi->chip_select, spi->max_speed_hz, spi->bits_per_word,
		spi->mode);

	/* Check for basic errors */
	ret = spi_w8r8(spi, 0); /* check for spi comm error */
	if (ret < 0) {
		dev_err(&spi->dev, "SPI not working\n");
		ret = -EIO;
		goto kfree_rx_exit;
	}
	if (!(spi->mode & SPI_NO_CS) &&
		(spi->chip_select > spi->master->num_chipselect)) {
		dev_err(&spi->dev,
			"setup: invalid chipselect %u (%u defined)\n",
			spi->chip_select, spi->master->num_chipselect);
		ret = -EINVAL;
		goto kfree_rx_exit;
	}
	return 0;

kfree_rx_exit:
	kfree(pdata->rx_buff);
kfree_tx_exit:
	kfree(pdata->tx_buff);
kfree_exit:
	kfree(pdata);

	return ret;
}

static int spidev_spi_remove(struct spi_device *spi)
{
	struct comedi_control *pdata = spi->dev.platform_data;

	mutex_lock(&daqgert_platform_lock);
	dev_info(&spi->dev, "releasing memory\n");
	if (pdata->rx_buff)
		kfree(pdata->rx_buff);
	dev_info(&spi->dev, "released rx\n");
	if (pdata->tx_buff)
		kfree(pdata->tx_buff);
	dev_info(&spi->dev, "released tx\n");
	if (pdata)
		kfree(pdata);
	dev_info(&spi->dev, "released pdata\n");
	mutex_unlock(&daqgert_platform_lock);

	return 0;
}

static struct spi_driver spidev_spi_driver = {
	.driver =
	{
		.name = "spigert",
		.owner = THIS_MODULE,
	},
	.probe = spidev_spi_probe,
	.remove = spidev_spi_remove,
};

/*
 * setup and probe the spi bus for devices
 */
static int daqgert_spi_probe(struct comedi_device *dev)
{
	int ret;

	dev_info(dev->class_dev, "SPI probe\n");
	if (!spi_adc.spi) {
		dev_info(dev->class_dev, "No SPI channel detected\n");
		spi_adc.chan = 0;
		spi_dac.chan = 0;
		return spi_adc.chan;
	}

	spi_adc.dev = dev;
	spi_dac.chan = NUM_AO_CHAN;

	switch (daqgert_conf) {
	case 1:
		spi_adc.device_type = MCP3202;
		spi_dac.device_type = MCP4822;
		break;
	case 2:
		spi_adc.device_type = MCP3002;
		spi_dac.device_type = MCP4822;
		break;
	case 3:
		spi_adc.device_type = MCP3202;
		spi_dac.device_type = MCP4802;
		break;
	default:
		spi_adc.device_type = MCP3002;
		spi_dac.device_type = MCP4802;
	}
	/* SPI data transfers, send a few dummys for config info */
	ret = spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	ret = spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	ret = spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	dev_info(dev->class_dev,
		"Gertboard ADC Board pre Detect Code %i, daqgert_conf option value %i\n",
		ret, daqgert_conf);
	if ((ret != 76) && (ret != 110)) { // PIC slave adc codes
		ret = spi_w8r8(spi_adc.spi, 0b01100000); /* check for channel 0 SE */
		if (1) { //FIXME need to add another probe test
			spi_adc.pic18 = 0; /* MCP3X02 mode */
			spi_adc.chan = NUM_AI_CHAN;
			spi_adc.range = 0; /* range 2.048 */
			dev_info(dev->class_dev,
				"Gertboard ADC chip Board Detected, %i Channels, Range code %i, Device code %i, PIC code %i, Detect Code %i\n",
				spi_adc.chan, spi_adc.range, spi_adc.device_type, spi_adc.pic18, ret);
			return spi_adc.chan;
		}
		spi_adc.pic18 = 0; /* SPI probes found nothing */
		dev_info(dev->class_dev, "No GERT Board ADC Found, GPIO pins only. Detect Code %i\n",
			ret);
		spi_adc.chan = 0;
		return spi_adc.chan;
	}
	if (ret) {
		spi_adc.pic18 = 1; /* PIC18 single-end mode 10 bits */
		spi_adc.device_type = PICSL10;
		spi_adc.chan = ret & 0x0f;
		spi_adc.range = (ret & 0b00100000) >> 5;
		spi_adc.bits = (ret & 0b00010000) >> 4;
		if (spi_adc.bits) {
			spi_adc.pic18 = 2; /* PIC24 mode 12 bits */
			spi_adc.device_type = PICSL12;
		}
		dev_info(dev->class_dev,
			"PIC spi slave ADC chip Board Detected, %i Channels, Range code %i, Device code %i, Bits code %i, PIC code %i, Detect Code %i\n",
			spi_adc.chan, spi_adc.range, spi_adc.device_type, spi_adc.bits, spi_adc.pic18, ret);
	} else {
		spi_adc.pic18 = 0; /* SPI probes found nothing */
		/* look for the gertboard SPI devices .pic18 code 1 */
		dev_info(dev->class_dev, "No GERT Board PIC Found, GPIO pins only. Detect Code %i\n",
			ret);
		spi_adc.chan = 0;

		return spi_adc.chan;
	}
	return spi_adc.chan;
}

static int __init daqgert_init(void)
{
	int ret;

	mutex_init(&daqgert_platform_lock);
	ret = spi_register_driver(&spidev_spi_driver);
	if (ret < 0)
		return ret;
	ret = comedi_driver_register(&daqgert_driver);
	if (ret < 0)
		return ret;

	return 0;
}
module_init(daqgert_init);

static void __exit daqgert_exit(void)
{
	comedi_driver_unregister(&daqgert_driver);
	spi_unregister_driver(&spidev_spi_driver);
}
module_exit(daqgert_exit);

MODULE_AUTHOR("Fred Brooks <spam@sma2.rain.com>");
MODULE_DESCRIPTION(
	"Comedi driver for RASPI GERTBOARD DIO/AI/AO");
MODULE_VERSION("0.0.18");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spigert");
