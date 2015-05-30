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
 * TODO:	Refactor sample put get code to reduce the amount of build up/down time
 * 
Driver: "experimental" daq_gert in progress ... for 4.+ kernels with DT
 * 
 * This driver requires a kernel patch to gain direct SPI access at the kernel.
 * 
 * git clone https://github.com/nsaspook/daq_gert.git
 * 
 * Test program executable: bmc_test_program
 * 
 * git clone https://github.com/raspberrypi/linux.git in /usr/src for the latest
 * linux kernel source tree
 * 
 * cd to the linux kernel source directory: /usr/src/linux etc...
 * copy the daq_gert.diff patch file from the daq_gert directory to here
 * copy RPI2.config, RPi.confg or RPI2.config_4.0.y the from the daq_gert directory to .config 
 * in the Linux source directory
 * 
 * patch the kernel source with the daq_gert.diff patch file
 * patch -p1 <daq_gert.diff
 * 
 *  make -j4 for a RPi 2
 *  select SPI_COMEDI=y in SPI MASTERS to enable the SPI side of the driver (SPI_SPIDEV must be deselected )
 *  select DAQ_GERT=m to select the Comedi protocol part of the driver
 *  make modules_install
 *  to recompile the Linux kernel with the Comedi SPI module link
 *  and to make the needed daq_gert module
 *  then copy the Image file to the /boot directory with a new kernel image name
 *  and modify the boot file to use that image
 *  after the reboot: daq_gert should auto-load to device /dev/comedi0
 *  dmesg should the the kernel module messages
 *  run the test program: bmc_test_program to see if it's working
 * 
Description: GERTBOARD daq_gert
Author: Fred Brooks <spam@sma2.rain.com>
 * 
Most of the actual GPIO setup code was copied from
 * 
WiringPI 
 *      https://projects.drogon.net/raspberry-pi/wiringpi/
 *
 * Driver for Broadcom BCM2708 SPI Controllers
 *
 Also many other Comedi drivers
 * 

Devices: [] GERTBOARD (daq_gert)
Status: inprogress (DIO 95%) (AI 90%) AO (95%) (My code cleanup 90%)
Updated: May 2015 12:07:20 +0000

The DAQ-GERT appears in Comedi as a  digital I/O subdevice (0) with
17 or 21 or 30 channels, 
a analog input subdevice (1) with 2 single-ended channels with onboard adc, OR
a analog input subdevice (1) with single-ended channels set by the SPI slave device
a analog output subdevice(2) with 2 channels with onboard dac
 * 
 * Caveats:
 * 

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
by the module option variable daqgert_conf in the /etc/modprobe.d directory
 * options daq_gert daqgert_conf=1
 * 
0 = Factory Gertboard configuratin of MCP3002 ADC and MCP4802 ADC: 10bit in/8bit out
1 = MCP3202 ADC and MCP4822 DAC: 12bit in/12bit out 
2 = MCP3002 ADC and MCP4822 DAC: 10bit in/12bit out
3 = MCP3202 ADC and MCP4802 DAC: 12bit in/8bit out
 * 
 * Module parameters are found in the /sys/modules/daq_gert/parameters directory
 * 
The input  range is 0 to 1023/4095 for 0.0 to 3.3(Vdd) onboard devices or 2.048 volts/Vdd for PIC slaves 
The output range is 0 to 4095 for 0.0 to 2.048 onboard devices (output resolution depends on the device)
 * In the async command mode transfers can be handled in HUNK mode by creating a SPI message
 * of many conversion sequences into one message, this allows for close to native driver wire-speed 
 * An optimized DMA driver is in the works
 * https://github.com/msperl/spi-bcm2835/wiki
 * (the current interrupt driven kernel driver is limited to a 12 to 64 byte FIFO and no DMA) HUNK_LEN data samples
 * into the Comedi read buffer with a special mix_mode for sampling both ADC devices in an alt sequence for
 * programs like xoscope at full speed (48828 ns per conversion over a 10 second period). 
 * The transfer array is currently static but can easily be made into
 * a config size parameter runtime value if needed with kmalloc for the required space

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

static int32_t daqgert_spi_probe(struct comedi_device *);
static void daqgert_ai_clear_eoc(struct comedi_device *);
static int32_t daqgert_ai_cancel(struct comedi_device *,
	struct comedi_subdevice *);
static int32_t daqgert_ao_cancel(struct comedi_device *,
	struct comedi_subdevice *);
static void daqgert_handle_ai_eoc(struct comedi_device *,
	struct comedi_subdevice *);
static void daqgert_handle_ao_eoc(struct comedi_device *,
	struct comedi_subdevice *);
static void my_timer_ai_callback(unsigned long);
static void daqgert_ai_set_chan_range(struct comedi_device *,
	uint32_t, char);
static uint32_t daqgert_ai_get_sample(struct comedi_device *,
	struct comedi_subdevice *);
static void daqgert_ao_put_sample(struct comedi_device *,
	struct comedi_subdevice *, uint32_t);
static void daqgert_handle_ai_hunk(struct comedi_device *,
	struct comedi_subdevice *);

#define HUNK_LEN 1000

/* analog chip types (type - 12 bits) */
static const uint32_t MCP3002 = 2; /* 10 bit ADC */
static const uint32_t MCP3202 = 0;
static const uint32_t MCP4802 = 4; /* 8 bit DAC output from 12 bit input data */
static const uint32_t MCP4812 = 2;
static const uint32_t MCP4822 = 0;
static const uint32_t PICSL10 = 2;
static const uint32_t PICSL12 = 0;

static const uint32_t SPI_BUFF_SIZE = 3072;
static const uint32_t MAX_CHANLIST_LEN = 256;
static const uint32_t CONV_SPEED = 5000; /* 10s of nsecs: the true rate is ~4883/5000 so we need a fixup,  two conversions per mix scan */
static const uint32_t CONV_SPEED_FIX = 1; /* usecs: round it up to ~50usecs total with this */
static const uint32_t CONV_SPEED_FIX_FAST = 16; /* used for the MCP3002 ADC */
static const uint32_t NS_TO_MS = 1000;
static const uint32_t MAX_BOARD_RATE = 1000000000;


static const uint8_t CSnA = 0; /* GPIO 8  Gertboard ADC */
static const uint8_t CSnB = 1; /* GPIO 7  Gertboard DAC */

/* PIC Slave commands */
static const uint8_t CMD_ZERO = 0b00000000;
static const uint8_t CMD_ADC_GO = 0b10000000;
static const uint8_t CMD_PORT_GO = 0b10100000; /* send data LO_NIBBLE to port buffer */
static const uint8_t CMD_CHAR_GO = 0b10110000; /* send data LO_NIBBLE to TX buffer */
static const uint8_t CMD_ADC_DATA = 0b11000000;
static const uint8_t CMD_PORT_DATA = 0b11010000; /* send data HI_NIBBLE to port buffer ->PORT and return input PORT data in received SPI data byte */
static const uint8_t CMD_CHAR_DATA = 0b11100000; /* send data HI_NIBBLE to TX buffer and return RX buffer in received SPI data byte */
static const uint8_t CMD_XXXX = 0b11110000; /* ??? */
static const uint8_t CMD_CHAR_RX = 0b00010000; /* Get RX buffer */
static const uint8_t CMD_DUMMY_CFG = 0b01000000; /* stuff config data in SPI buffer */
static const uint8_t CMD_DEAD = 0b11111111; /* This is usually a bad response */

static const uint32_t PIN_SAFE_MASK_WPI = 0b00000000000000000111111100000000;
static const uint32_t PIN_SAFE_MASK_GPIO1 = 0b00000000000000000000111110000011;
static const uint32_t PIN_SAFE_MASK_GPIO2 = 0b00000000000000000000111110001100;

static const uint32_t INPUT = 0;
static const uint32_t OUTPUT = 1;
static const uint32_t PWM_OUTPUT = 2;

static const uint32_t LOW = 0;
static const uint32_t HIGH = 1;

/* GPPUD:
 * GPIO Pin pull up/down register
 */
static const uint32_t GPPUD = 37;
static const uint32_t PUD_OFF = 0;
static const uint32_t PUD_DOWN = 1;
static const uint32_t PUD_UP = 2;

/* driver hardware numbers */
static const uint32_t NUM_DIO_CHAN = 17;
static const uint32_t NUM_DIO_CHAN_REV2 = 17;
static const uint32_t NUM_DIO_CHAN_REV3 = 17;
static const uint32_t NUM_DIO_OUTPUTS = 8;
static const uint32_t DIO_PINS_DEFAULT = 0xff;


/* found at /sys/modules/daq_gert/parameters */
static int32_t daqgert_conf = 0;
module_param(daqgert_conf, int, S_IRUGO);
static int32_t pullups = 2;
module_param(pullups, int, S_IRUGO);
static int32_t gpiosafe = 1;
module_param(gpiosafe, int, S_IRUGO);
static int32_t dio_conf = 0;
module_param(dio_conf, int, S_IRUGO);
static int32_t ai_count = 0;
module_param(ai_count, int, S_IRUGO);
static int32_t ao_count = 0;
module_param(ao_count, int, S_IRUGO);
static int32_t hunk_count = 0;
module_param(hunk_count, int, S_IRUGO);
static int32_t hunk_len = HUNK_LEN;
module_param(hunk_len, int, S_IRUGO);
static int32_t gert_autoload = 1;
module_param(gert_autoload, int, S_IRUGO);
static int32_t gert_type = 0;
module_param(gert_type, int, S_IRUGO);
static int32_t speed_test = 0;
module_param(speed_test, int, S_IRUGO);
static int32_t wiringpi = 1;
module_param(wiringpi, int, S_IRUGO);
static int32_t use_hunking = 1;
module_param(use_hunking, int, S_IRUGO);

struct daqgert_board {
	const char *name;
	int32_t board_type;
	int32_t n_aichan;
	int32_t n_aochan;
	uint32_t ai_ns_min;
	uint32_t ai_ns_min_calc;
	uint32_t ai_rate_min;
	uint32_t ao_ns_min;
	uint32_t ao_ns_min_calc;
	uint32_t ao_rate_min;
};

static const struct daqgert_board daqgert_boards[] = {
	{
		.name = "Gertboard",
		.board_type = 0,
		.n_aichan = 2,
		.n_aochan = 2,
		.ai_ns_min = 50000, /* values plus software overhead */
		.ai_ns_min_calc = 35000,
		.ai_rate_min = 20000,
		.ao_ns_min = 5000,
		.ao_ns_min_calc = 4500,
		.ao_rate_min = 10000,
	},
	{
		.name = "Fredboard",
		.board_type = 1,
		.n_aichan = 8,
		.n_aochan = 8,
		.ai_ns_min = 50000,
		.ai_ns_min_calc = 35000,
		.ai_rate_min = 20000,
		.ao_ns_min = 5000,
		.ao_ns_min_calc = 4500,
		.ao_rate_min = 10000,
	},
};

struct comedi_control {
	uint8_t *tx_buff;
	uint8_t *rx_buff;
	struct spi_transfer t[HUNK_LEN];
	uint32_t delay_usecs;
	uint32_t mix_delay_usecs;
	struct mutex daqgert_platform_lock;
};

struct spi_param_type {
	uint32_t range : 1;
	uint32_t bits : 2;
	uint32_t link : 1;
	uint32_t pic18 : 2;
	uint32_t chan : 4;
	struct spi_device *spi;
	int32_t device_type;
	int32_t hunk_size; /* the number of needed values returned as data */
	struct timer_list my_timer;
	struct task_struct *daqgert_task;
};

/* SPI devices for COMEDI to use in global scope */
static struct spi_param_type spi_adc, spi_dac;

struct daqgert_private {
	uint32_t RPisys_rev;
	uint32_t __iomem *timer_1mhz;
	int32_t *pinToGpio;
	int32_t *physToGpio;
	int32_t board_rev;
	int32_t num_subdev;
	uint32_t ai_rate_max, ao_rate_max, ao_timer, ao_counter;
	uint32_t ai_conv_delay_usecs, ai_conv_delay_10nsecs, ai_cmd_delay_usecs,
	ai_neverending, ao_neverending;
	int32_t ai_chan, ao_chan, timer, run, spi_ai_run, spi_ao_run, ai_count,
	ao_count, hunk_count, ai_cmd_running, ai_cmd_canceled, ao_cmd_running,
	ao_cmd_canceled;
	struct mutex drvdata_lock, cmd_lock;
	uint32_t val;
	uint16_t hunk : 1;
	uint16_t ai_hunk : 1;
	uint16_t ai_mix : 1;
	int32_t mix_chan;
	uint32_t last_hunk_run;
	int32_t next_hunk_buf;
	uint32_t runs_to_end; /* how many times we must switch buffers */
	uint32_t ai_scans; /*  length of scanlist */
	int32_t ai_scans_left; /*  number left to finish */
	uint32_t ao_scans; /*  length of scanlist */
	uint32_t ai_poll_ptr;
	struct spi_param_type *ai_spi;
	struct spi_param_type *ao_spi;
	void (*pinMode) (struct comedi_device *dev, int32_t pin, int32_t mode);
	void (*digitalWrite) (struct comedi_device *dev, int32_t pin, int32_t value);
	void(*setPadDrive) (struct comedi_device *dev, int32_t group, int32_t value);
	int32_t(*digitalRead) (struct comedi_device *dev, int32_t pin);
	int32_t timing_lockout;
};

/* Locals to hold pointers to the hardware */

/* Global for the RPi board rev */
extern uint32_t system_rev; /* from the kernel symbol table exports */
extern uint32_t system_serial_low;
extern uint32_t system_serial_high;

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
static int32_t wpi_pin_safe(struct comedi_device *dev, int32_t pin)
{
	struct daqgert_private *devpriv = dev->private;
	uint32_t pin_bit = (0x01 << pin);

	if (!gpiosafe) return true;
	if (wiringpi) {
		if (pin_bit & PIN_SAFE_MASK_WPI) return false;
	} else {
		if (devpriv->board_rev == 1) {
			if (pin_bit & PIN_SAFE_MASK_GPIO1) return false;
		} else {
			if (pin_bit & PIN_SAFE_MASK_GPIO2) return false;
		}
	}
	return true;
}

/*
 Doing it the Arduino way with lookup tables...
      Yes, it's probably more inefficient than all the bit-twidling, but it
      does tend to make it all a bit clearer. At least to me!

 pinToGpio:
      Take a Wiring pin (0 through X) and re-map it to the BCM_GPIO pin
      Cope for 2 different board revisions here
 */

static int32_t pinToGpioR1 [64] = {
	17, 18, 21, 22, 23, 24, 25, 4, /* From the Original Wiki - GPIO 0 through 7:   wpi  0 -  7 */
	0, 1, /* I2C  - SDA1, SCL1                            wpi  8 -  9 */
	8, 7, /* SPI  - CE1, CE0                              wpi 10 - 11 */
	10, 9, 11, /* SPI  - MOSI, MISO, SCLK                      wpi 12 - 14 */
	14, 15, /* UART - Tx, Rx                                wpi 15 - 16 */

	/* Padding: */

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* ... 31 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* ... 47 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, /* ... 63 */
};

/* Revision 2: */

static int32_t pinToGpioR2 [64] = {
	17, 18, 27, 22, 23, 24, 25, 4, /* From the Original Wiki - GPIO 0 through 7:   wpi  0 -  7 */
	2, 3, /* I2C  - SDA0, SCL0                            wpi  8 -  9 */
	8, 7, /* SPI  - CE1, CE0                              wpi 10 - 11 */
	10, 9, 11, /* SPI  - MOSI, MISO, SCLK                      wpi 12 - 14 */
	14, 15, /* UART - Tx, Rx                                wpi 15 - 16 */
	28, 29, 30, 31, /* Rev 2: New GPIOs 8 though 11                 wpi 17 - 20 */
	5, 6, 13, 19, 26, /* B+                                           wpi 21, 22, 23, 24, 25 */
	12, 16, 20, 21, /* B+                                           wpi 26, 27, 28, 29 */
	0, 1, /* B+                                           wpi 30, 31 */

	/* Padding: */

	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

/*	physToGpio:
	Take a physical pin (1 through 26) and re-map it to the BCM_GPIO pin
	Cope for 2 different board revisions here.
	Also add in the P5 connector, so the P5 pins are 3,4,5,6, so 53,54,55,56
 */

static int32_t physToGpioR1 [64] = {
	-1, /* 0 */
	-1, -1, /* 1, 2 */
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
	-1, 7, /* 25, 26 */

	-1, -1, -1, -1, -1, /* ... 31 */
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
};

static int32_t physToGpioR2 [64] = {
	-1, /* 0 */
	-1, -1, /* 1, 2 */
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
	-1, 7, /* 25, 26 */

	/* B+ */

	0, 1,
	5, -1,
	6, 12,
	13, -1,
	19, 16,
	26, 20,
	-1, 21,

	/* the P5 connector on the Rev 2 boards: */

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

/*
	gpioToGPFSEL:
	Map a BCM_GPIO pin to it's Function Selection
	control port. (GPFSEL 0-5)
	Groups of 10 - 3 bits per Function - 30 bits per port
 */

static uint8_t gpioToGPFSEL [] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
	3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
	5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
};

/*
	gpioToShift
	Define the shift up for the 3 bits per pin in each GPFSEL port
 */

static uint8_t gpioToShift [] = {
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
	0, 3, 6, 9, 12, 15, 18, 21, 24, 27,
};

/*
	gpioToGPSET:
	(Word) offset to the GPIO Set registers for each GPIO pin
 */

static uint8_t gpioToGPSET [] = {
	7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
	8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
};

/*
	gpioToGPCLR:
	(Word) offset to the GPIO Clear registers for each GPIO pin
 */

static uint8_t gpioToGPCLR [] = {
	10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10,
	11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11,
};

/*
	gpioToGPLEV:
	(Word) offset to the GPIO Input level registers for each GPIO pin
 */

static uint8_t gpioToGPLEV [] = {
	13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13,
	14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14, 14,
};

/*
	gpioToPUDCLK
	(Word) offset to the Pull Up Down Clock regsiter
 */

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

static void pullUpDnControl(struct comedi_device *dev, int32_t pin, int32_t pud)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t *pinToGpio = devpriv->pinToGpio;

	pin = pinToGpio [pin];
	iowrite32(pud & 3, (__iomem uint32_t*) dev->mmio + GPPUD);
	udelay(5);
	iowrite32(1 << (pin & 31), (__iomem uint32_t*) dev->mmio + gpioToPUDCLK [pin]);
	udelay(5);

	iowrite32(0, (__iomem uint32_t*) dev->mmio + GPPUD);
	udelay(5);
	iowrite32(0, (__iomem uint32_t*) dev->mmio + gpioToPUDCLK [pin]);
	udelay(5);
}

/*
 * pinMode:
 *      Sets the mode of a pin to be input, output
 ************************************************************
 */

static void pinModeGpio(struct comedi_device *dev, int32_t pin, int32_t mode)
{
	int32_t fSel, shift;

	pin &= 63;
	fSel = gpioToGPFSEL [pin];
	shift = gpioToShift [pin];

	if (mode == INPUT) /* Sets bits to zero = input */
		iowrite32(ioread32((__iomem uint32_t*) dev->mmio + fSel) & ~(7 << shift), (__iomem uint32_t*) dev->mmio + fSel);
	else
		if (mode == OUTPUT)
		iowrite32((ioread32((__iomem uint32_t*) dev->mmio + fSel) & ~(7 << shift)) | (1 << shift), (__iomem uint32_t*) dev->mmio + fSel);
}

static void pinModeWPi(struct comedi_device *dev, int32_t pin, int32_t mode)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t *pinToGpio = devpriv->pinToGpio;

	pinModeGpio(dev, pinToGpio [pin & 63], mode);
}

/*
 * digitalWrite:
 *      Set an output bit
 *****************************************************************
 */

static void digitalWriteWPi(struct comedi_device *dev, int32_t pin, int32_t value)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t *pinToGpio = devpriv->pinToGpio;

	pin = pinToGpio [pin & 63];
	if (value == LOW)
		iowrite32(1 << (pin & 31), (__iomem uint32_t*) dev->mmio + gpioToGPCLR [pin]);

	else
		iowrite32(1 << (pin & 31), (__iomem uint32_t*) dev->mmio + gpioToGPSET [pin]);
}

static void digitalWriteGpio(struct comedi_device *dev, int32_t pin, int32_t value)
{

	pin &= 63;
	if (value == LOW)
		iowrite32(1 << (pin & 31), (__iomem uint32_t*) dev->mmio + gpioToGPCLR [pin]);
	else
		iowrite32(1 << (pin & 31), (__iomem uint32_t*) dev->mmio + gpioToGPSET [pin]);
}

/*
 * digitalRead:
 *      Read the value of a given Pin, returning HIGH or LOW
 *******************************************************************
 */

static int32_t digitalReadWPi(struct comedi_device *dev, int32_t pin)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t *pinToGpio = devpriv->pinToGpio;

	pin = pinToGpio [pin & 63];
	if ((ioread32((__iomem uint32_t*) dev->mmio + gpioToGPLEV [pin]) & (1 << (pin & 31))) != 0)
		return HIGH;
	else
		return LOW;
}

static int32_t digitalReadGpio(struct comedi_device *dev, int32_t pin)
{

	pin &= 63;
	if ((ioread32((__iomem uint32_t*) dev->mmio + gpioToGPLEV [pin]) & (1 << (pin & 31))) != 0)
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

static int32_t piBoardRev(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t r = -1, nscheme = 0;
	static int32_t boardRev = -1;

	if (boardRev != -1) /* skip if already detected */
		return boardRev;

	if (devpriv->RPisys_rev & 0x80000000) dev_info(dev->class_dev, "over-volt bit set\n");
	if (devpriv->RPisys_rev & 0x800000) {
		nscheme = 1;
		r = devpriv->RPisys_rev & 0xf;
		dev_info(dev->class_dev, "RPi new scheme rev %x, serial %08x%08x, new rev %x\n",
			devpriv->RPisys_rev, system_serial_high, system_serial_low, r);
	} else {
		r = devpriv->RPisys_rev & 0xff;
		dev_info(dev->class_dev, "RPi old scheme rev %x, serial %08x%08x\n",
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

	dev_info(dev->class_dev, "driver gpio board rev %u\n",
		boardRev);
	dio_conf = boardRev; /* set module param */

	return boardRev;
}

/*
 * wiringPiSetup:
 *	Must be called once at the start of your program execution.
 *
 * Default setup: Initializes the system into wiringPi Pin mode and uses the
 *	memory mapped hardware directly.
 ************************************************************************
 */

static int32_t wiringPiSetup(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t boardRev;

	devpriv->pinMode = pinModeWPi;
	devpriv->digitalWrite = digitalWriteWPi;
	devpriv->digitalRead = digitalReadWPi;

	if ((boardRev = piBoardRev(dev)) < 0)
		return -1;

	/* set the comedi private data */
	if (boardRev == 1) {
		devpriv->pinToGpio = pinToGpioR1;
		devpriv->physToGpio = physToGpioR1;
	} else {

		devpriv->pinToGpio = pinToGpioR2;
		devpriv->physToGpio = physToGpioR2;
	}
	return 0;
}

/*
 * wiringPiSetupGpio:
 *	Must be called once at the start of your program execution.
 *
 * GPIO setup: Initializes the system into GPIO Pin mode and uses the
 *	memory mapped hardware directly.
 *************************************************************************
 */

static int32_t wiringPiSetupGpio(struct comedi_device *dev)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t x;

	if ((x = wiringPiSetup(dev)) < 0)
		return x;

	devpriv->pinMode = pinModeGpio;
	devpriv->digitalWrite = digitalWriteGpio;
	devpriv->digitalRead = digitalReadGpio;

	return 0;
}

/* A client must be connected with a valid comedi cmd 
 * and *data a pointer to that comedi structure
 * for this not to segfault 
 */
static int32_t daqgert_ai_thread_function(void *data)
{
	struct comedi_device *dev = (void*) data;
	struct comedi_subdevice *s = dev->read_subdev;
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;

	while (!kthread_should_stop()) {
		while (!devpriv->run) {
			if (devpriv->timer) {
				schedule();
			} else {
				msleep(1);
			}
			if (devpriv->timer && devpriv->run) {
				devpriv->spi_ai_run = true;
			}
			if (kthread_should_stop()) return 0;
		}
		if (devpriv->ai_cmd_running) {
			if (devpriv->ai_hunk) {
				daqgert_handle_ai_hunk(dev, s);
				devpriv->hunk_count++;
				hunk_count = devpriv->hunk_count;
			} else {
				daqgert_handle_ai_eoc(dev, s);
				devpriv->ai_count++;
				usleep_range(pdata->delay_usecs, pdata->delay_usecs + 1);
			}
		} else {
			devpriv->spi_ai_run = false;
			msleep(1);
		}
	}
	/*do_exit(1);*/
	return 0;

}

static int32_t daqgert_ao_thread_function(void *data)
{
	struct comedi_device *dev = (void*) data;
	struct comedi_subdevice *s = &dev->subdevices[2];
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;

	while (!kthread_should_stop()) {
		if (devpriv->ao_cmd_running) {
			devpriv->spi_ao_run = true;
			daqgert_handle_ao_eoc(dev, s);
			devpriv->ao_count++;
			usleep_range(pdata->delay_usecs, pdata->delay_usecs + 1);
		} else {
			devpriv->spi_ao_run = false;
			msleep(1);
		}
	}
	/*do_exit(1);*/
	return 0;

}

static void daqgert_ai_start_pacer(struct comedi_device *dev, bool load_timers)
{
	struct daqgert_private *devpriv = dev->private;

	if (load_timers) {

		/* setup timer interval to msecs */
		mod_timer(&devpriv->ai_spi->my_timer, jiffies + msecs_to_jiffies(10));
	}
}

static void daqgert_ai_set_chan_range(struct comedi_device *dev,
	uint32_t chanspec, char wait)
{
	struct daqgert_private *devpriv = dev->private;
	mutex_lock(&devpriv->drvdata_lock);
	devpriv->ai_chan = CR_CHAN(chanspec);

	if (wait)
		udelay(1);
	mutex_unlock(&devpriv->drvdata_lock);
}

static void daqgert_ao_set_chan_range(struct comedi_device *dev,
	uint32_t chanspec, char wait)
{
	struct daqgert_private *devpriv = dev->private;
	mutex_lock(&devpriv->drvdata_lock);
	devpriv->ao_chan = CR_CHAN(chanspec);

	if (wait)
		udelay(1);
	mutex_unlock(&devpriv->drvdata_lock);
}

static void daqgert_ao_put_sample(struct comedi_device *dev,
	struct comedi_subdevice *s, uint32_t val)
{
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	uint32_t val_tmp, chan;

	mutex_lock(&devpriv->drvdata_lock);
	chan = CR_CHAN(devpriv->ao_chan);
	val_tmp = val & 0xfff; /* strip to 12 bits */
	pdata->tx_buff[1] = val_tmp & 0xff; /* load lsb SPI data into transfer buffer */
	pdata->tx_buff[0] = (0b00110000 | ((chan & 0x01) << 7) | (val_tmp >> 8));
	spi_write_then_read(spi_data->spi, pdata->tx_buff, 2, pdata->rx_buff, 2); /* Load DAC channel, send two bytes */
	s->readback[chan] = val;
	devpriv->spi_ao_run = false;
	mutex_unlock(&devpriv->drvdata_lock);
}

static uint32_t daqgert_ai_get_sample(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	struct spi_message m;
	int32_t chan;
	uint32_t val;

	mutex_lock(&devpriv->drvdata_lock);
	chan = CR_CHAN(devpriv->ai_chan);
	/* Make SPI messages for the type of ADC are we talking to */
	/* The PIC Slave needs 8 bit transfers only */
	if (spi_data->pic18) { /*  PIC18 SPI slave device. NO MULTI_MODE ever */
		udelay(devpriv->ai_cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ADC_GO + chan;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1); /* rx buffer has old data */
		udelay(devpriv->ai_conv_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ZERO;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1); /* rx buffer has old data */
		udelay(devpriv->ai_cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ADC_DATA;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1);
		val = pdata->rx_buff[0];
		udelay(devpriv->ai_cmd_delay_usecs); /* ADC conversion delay */
		pdata->tx_buff[0] = CMD_ZERO;
		spi_write_then_read(spi_data->spi, pdata->tx_buff, 1, pdata->rx_buff, 1);
		val += pdata->rx_buff[0] << 8;
	} else { /* Gertboard onboard ADC device */
		if (devpriv->ai_hunk) { /* for single channel command scans with pre-formatted tx_buffer & transfer array */
			spi_message_init_with_transfers(&m, &pdata->t[0], hunk_len); /* make the proper message with the transfers */
		} else {
			pdata->tx_buff[0] = 0b11010000 | ((chan & 0x01) << 5);
			spi_message_init_with_transfers(&m, &pdata->t[0], 1); /* make the proper message with the transfer */
		}
		spi_bus_lock(spi_data->spi->master);
		spi_sync_locked(spi_data->spi, &m); /* exchange SPI data */
		spi_bus_unlock(spi_data->spi->master);
		/* ADC type code result munging */
		if (devpriv->ai_hunk) { /* for single channel command scans */
			val = 0; /* data in the buffers will be sent to comedi buffers later */
		} else {
			if (spi_data->device_type == MCP3002) {
				val = ((pdata->rx_buff[0] << 7) | (pdata->rx_buff[1] >> 1)) & 0x3FF;
			} else {
				val = (pdata->rx_buff[2]&0x80) >> 7;
				val += pdata->rx_buff[1] << 1;
				val += (pdata->rx_buff[0]&0x0f) << 9;
			}
		}
	}
	devpriv->spi_ai_run = false;
	mutex_unlock(&devpriv->drvdata_lock);
	return val & s->maxdata;
}

/* start chan set in ai_cmd */
static void daqgert_handle_ai_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	uint32_t next_chan, val;
	uint32_t chan = s->async->cur_chan;

	val = daqgert_ai_get_sample(dev, s);
	comedi_buf_write_samples(s, &val, 1);

	next_chan = s->async->cur_chan;
	if (cmd->chanlist[chan] != cmd->chanlist[next_chan])
		daqgert_ai_set_chan_range(dev, cmd->chanlist[next_chan], 1);

	if (cmd->stop_src == TRIG_COUNT &&
		s->async->scans_done >= cmd->stop_arg) {
		if (!devpriv->ai_neverending) {
			daqgert_ai_cancel(dev, s);
			s->async->events |= COMEDI_CB_EOA;
		}
	}
}

static void daqgert_ao_next_chan(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;

	if (cmd->stop_src == TRIG_COUNT &&
		s->async->scans_done >= cmd->stop_arg) {
		if (!devpriv->ao_neverending) {
			/* all data sampled */
			daqgert_ao_cancel(dev, s);
			s->async->events |= COMEDI_CB_EOA;
		}
	}
}

/* start chan set in ao_cmd */
static void daqgert_handle_ao_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	uint32_t next_chan, val;
	uint16_t sampl_val;
	uint32_t chan = s->async->cur_chan;

	if (!comedi_buf_read_samples(s, &sampl_val, 1)) {
		dev_err(dev->class_dev, "buffer underflow\n");
		s->async->events |= COMEDI_CB_OVERFLOW;
		return;
	}

	/* possible munge of data */
	val = sampl_val;
	daqgert_ao_put_sample(dev, s, val);

	next_chan = s->async->cur_chan;
	if (cmd->chanlist[chan] != cmd->chanlist[next_chan])
		daqgert_ao_set_chan_range(dev, cmd->chanlist[next_chan], 1);

	daqgert_ao_next_chan(dev, s);
}

#if 0

static void daqgert_ai_soft_trig(struct comedi_device *dev)
{

	struct daqgert_private *devpriv = dev->private;

	devpriv->timer = true;
	daqgert_ai_start_pacer(dev, true);
}

static int32_t daqgert_ai_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn,
	unsigned long context)
{
	struct daqgert_private *devpriv = dev->private;
	if (devpriv->spi_ai_run) return -EBUSY;

	return 0;

}
#endif

static void transfer_from_hunk_buf(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint8_t *ptr,
	uint32_t bufptr, uint32_t len, uint32_t offset,
	bool mix_mode)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct spi_param_type *spi_data = s->private;
	uint32_t i;
	uint32_t val;


	for (i = 0; i < len; i++) { /* count up from zero to access the buffer */
		if (spi_data->device_type == MCP3002) {
			val = ((ptr[0 + bufptr] << 7) | (ptr[1 + bufptr] >> 1)) & 0x3FF;
		} else {
			val = (ptr[2 + bufptr]&0x80) >> 7;
			val += ptr[1 + bufptr] << 1;
			val += (ptr[0 + bufptr]&0x0f) << 9;
		}
		bufptr += offset;

		if (mix_mode) {
			if (i & 0x01) { /* use a even/odd mix of adc devices */
				s->async->cur_chan = 1;
				comedi_buf_write_samples(s, &val, 1);
				s->async->cur_chan = 0;
				s->async->events |= COMEDI_CB_EOS;
			} else {
				s->async->cur_chan = 0;
				comedi_buf_write_samples(s, &val, 1);
			}
		} else {
			s->async->cur_chan = 0;
			comedi_buf_write_samples(s, &val, 1);
			s->async->events |= COMEDI_CB_EOS;
		}
		comedi_handle_events(dev, s);
	}
	if (cmd->stop_src == TRIG_COUNT &&
		s->async->scans_done >= cmd->stop_arg) {
		daqgert_ai_cancel(dev, s);
		s->async->scans_done = cmd->stop_arg;
		s->async->events |= COMEDI_CB_EOA;
		comedi_handle_events(dev, s);
	}

}

static void transfer_to_hunk_buf(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint8_t *ptr,
	uint32_t bufptr, uint32_t hunk_len, uint32_t offset,
	bool mix_mode)
{
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	uint32_t i, len, delay_usecs = pdata->delay_usecs;
	uint32_t chan;
	uint8_t *tx_buff, *rx_buff;

	chan = devpriv->ai_chan;
	memset(&pdata->t, 0, sizeof(pdata->t));
	if (spi_data->device_type == MCP3002) {
		len = 2;
	} else {
		len = 3;
	}
	tx_buff = pdata->tx_buff;
	rx_buff = pdata->rx_buff;

	if (hunk_len > hunk_len) {
		dev_err(dev->class_dev, "scan transfer too large %i>%i\n",
			hunk_len, hunk_len);
		hunk_len = hunk_len;
	}

	for (i = 0; i < hunk_len; i++) {
		/* format the tx_buffer */
		if (mix_mode) {
			if (i & 0x01) { /* use a even/odd mix of adc devices */
				chan = devpriv->mix_chan;
				delay_usecs = pdata->mix_delay_usecs;
			} else {
				chan = devpriv->ai_chan;
				delay_usecs = 0;
			}
		}
		ptr[bufptr] = 0b11010000 | ((chan & 0x01) << 5); /* set the channel and config data */
		bufptr += offset;
		/* format the transfer array */
		pdata->t[i].cs_change = 1;
		pdata->t[i].len = len;
		pdata->t[i].tx_buf = tx_buff;
		pdata->t[i].rx_buf = rx_buff;
		pdata->t[i].delay_usecs = delay_usecs;
		tx_buff += len; /* move the buffer pointers to the next transfer slot in the buffer memory */
		rx_buff += len;
	}
}

static void daqgert_handle_ai_hunk(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	int32_t len, offset, bufptr;
	uint8_t *ptr;

	daqgert_ai_get_sample(dev, s); /* get the data from the ADC via SPI */
	ptr = (uint8_t *) pdata->rx_buff;
	bufptr = 0;

	len = devpriv->ai_scans;
	if (cmd->stop_src == TRIG_COUNT) {
		if (devpriv->ai_scans_left > hunk_len) {
			devpriv->ai_scans_left -= hunk_len;
			len = hunk_len;
		} else {
			len = devpriv->ai_scans_left;
			devpriv->ai_scans_left = 0;
		}
	}

	if (spi_data->device_type == MCP3002) {
		offset = 2;
	} else {
		offset = 3;
	}
	devpriv->ai_count += len;
	transfer_from_hunk_buf(dev, s, ptr, bufptr, len, offset, true);
	if (cmd->stop_src == TRIG_COUNT)
		dev_info(dev->class_dev, "From hunk %i %i\n", s->async->scans_done, cmd->stop_arg);
	devpriv->next_hunk_buf++;

	if (devpriv->next_hunk_buf > 1) devpriv->next_hunk_buf = 0;
}

static void daqgert_ai_setup_hunk(struct comedi_device *dev,
	struct comedi_subdevice *s, bool mix_mode)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	uint32_t len, offset, bufptr;
	uint8_t *ptr;

	len = devpriv->ai_scans;
	if (cmd->stop_src == TRIG_COUNT) { /* optimize small samples */
		if (devpriv->ai_scans > hunk_len) {
			len = hunk_len;
		} else {
			len = devpriv->ai_scans;
		}
	}

	if (cmd->stop_src == TRIG_NONE) { /* for future double buffer */
		devpriv->runs_to_end = true;
	} else {
		devpriv->runs_to_end = false;
	}

	devpriv->next_hunk_buf = 0;
	ptr = (uint8_t *) pdata->tx_buff;
	bufptr = 0;
	if (spi_data->device_type == MCP3002) {
		offset = 2;
	} else {
		offset = 3;
	}
	/* load the message for the ADC conversions in to the tx buffer */
	transfer_to_hunk_buf(dev, s, ptr, bufptr, len, offset, mix_mode);
}

static void daqgert_ai_setup_eoc(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	uint32_t len;

	memset(&pdata->t, 0, sizeof(pdata->t));
	if (spi_data->device_type == MCP3002) {
		len = 2;
	} else {
		len = 3;
	}

	/* format the tx_buffer */
	pdata->tx_buff[2] = 0; /* format the ADC data as a single transmission into the buffer */
	pdata->tx_buff[1] = 0;

	/* format the transfer array */
	pdata->t[0].cs_change = 0;
	pdata->t[0].len = len;
	pdata->t[0].tx_buf = pdata->tx_buff;
	pdata->t[0].rx_buf = pdata->rx_buff;
}

static int32_t daqgert_ai_inttrig(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint32_t trig_num)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int32_t ret = 0;

	if (trig_num != cmd->start_arg)
		return -EINVAL;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ai inttrig\n");

	if (!devpriv->ai_cmd_running) {
		devpriv->run = false;
		devpriv->timer = true;
		daqgert_ai_start_pacer(dev, true);
		devpriv->ai_cmd_running = true;
		devpriv->ai_cmd_canceled = false;
		s->async->inttrig = NULL;
	} else {
		ret = -EBUSY;
	}

	mutex_unlock(&devpriv->cmd_lock);
	return ret;
}

static int32_t daqgert_ao_inttrig(struct comedi_device *dev,
	struct comedi_subdevice *s,
	uint32_t trig_num)
{
	struct daqgert_private *devpriv = dev->private;
	struct comedi_cmd *cmd = &s->async->cmd;
	int32_t ret = 0;

	if (trig_num != cmd->start_arg)
		return -EINVAL;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ao inttrig\n");

	if (!devpriv->ao_cmd_running) {
		devpriv->ao_cmd_running = true;
		devpriv->ao_cmd_canceled = false;
		s->async->inttrig = NULL;
	} else {
		ret = -EBUSY;
	}

	mutex_unlock(&devpriv->cmd_lock);
	return ret;
}

static int32_t daqgert_ao_cmd(struct comedi_device *dev, struct comedi_subdevice *s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct daqgert_private *devpriv = dev->private;
	int ret = 0;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ao_cmd\n");
	if (devpriv->ao_cmd_running) {
		dev_info(dev->class_dev, "ao_cmd busy\n");
		ret = -EBUSY;
		goto ao_cmd_exit;
	}

	devpriv->hunk = use_hunking;
	/* for possible hunking of AO */
	if (cmd->stop_src == TRIG_COUNT) {
		devpriv->ao_scans = cmd->chanlist_len * cmd->stop_arg;
		devpriv->ao_neverending = false;
	} else {
		devpriv->ao_scans = hunk_len;
		devpriv->ao_neverending = true;
	}

	/* 1ms */
	/* timing of the scan: we set all channels at once */
	devpriv->ao_timer = cmd->scan_begin_arg / 1000000;
	if (devpriv->ao_timer < 1) {
		ret = -EINVAL;
		goto ao_cmd_exit;
	}
	devpriv->ao_counter = devpriv->ao_timer;
	s->async->cur_chan = 0;
	daqgert_ao_set_chan_range(dev, cmd->chanlist[s->async->cur_chan], 1);

	if (cmd->start_src == TRIG_NOW) {
		s->async->inttrig = NULL;
		/* enable this acquisition operation */
		devpriv->ao_cmd_running = true;
		devpriv->ao_cmd_canceled = false;
	} else {
		/* TRIG_INT */
		/* wait for an internal signal */
		s->async->inttrig = daqgert_ao_inttrig;
	}

	devpriv->timing_lockout++;
ao_cmd_exit:
	mutex_unlock(&devpriv->cmd_lock);
	return ret;
}

static int32_t daqgert_ai_cmd(struct comedi_device *dev, struct comedi_subdevice * s)
{
	struct comedi_cmd *cmd = &s->async->cmd;
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	int32_t ret = 0, i;

	mutex_lock(&devpriv->cmd_lock);
	dev_info(dev->class_dev, "ai_cmd\n");
	if (devpriv->ai_cmd_running) {
		dev_info(dev->class_dev, "ai_cmd busy\n");
		ret = -EBUSY;
		goto ai_cmd_exit;
	}

	devpriv->hunk = use_hunking;
	if (cmd->stop_src == TRIG_COUNT) {
		devpriv->ai_scans = cmd->chanlist_len * cmd->stop_arg;
		devpriv->ai_neverending = false;
	} else {
		devpriv->ai_scans = hunk_len;
		devpriv->ai_neverending = true;
	}
	devpriv->ai_scans_left = devpriv->ai_scans; /* a count down */

	if (devpriv->hunk && !spi_data->pic18) { /* check if we can use HUNK transfer */
		devpriv->ai_hunk = true;
		devpriv->ai_mix = false;
		devpriv->mix_chan = CR_CHAN(cmd->chanlist[0]);
		for (i = 1; i < cmd->chanlist_len; i++) {
			if (cmd->chanlist[0] != cmd->chanlist[i]) {
				/* we can't use HUNK :-( */
				devpriv->ai_hunk = false;
				dev_info(dev->class_dev, "hunk ai mode transfers disabled\n");
				break;
			}
		}
		/* check for the special mix_mode case */
		if (cmd->chanlist_len == 2 && (cmd->chanlist[0] != cmd->chanlist[1])) {
			devpriv->ai_hunk = true;
			devpriv->ai_mix = true;
			devpriv->mix_chan = CR_CHAN(cmd->chanlist[1]);
			dev_info(dev->class_dev, "hunk ai mix_mode transfers enabled\n");
		}
	} else {
		devpriv->ai_hunk = false;
	}

	s->async->cur_chan = 0;
	daqgert_ai_set_chan_range(dev, cmd->chanlist[s->async->cur_chan], 1);

	/* don't we want wake up every scan? */
	if (cmd->flags & CMDF_WAKE_EOS) {
		/* HUNK is useless for this situation */
		if (cmd->chanlist_len == 1)
			devpriv->ai_hunk = false;
	}
	if (devpriv->timing_lockout) {
		devpriv->ai_hunk = false;
		dev_info(dev->class_dev, "all hunk ai mode transfers disabled from timing lockout\n");
	}

	if (devpriv->ai_hunk) /* run batch conversions in background */
		daqgert_ai_setup_hunk(dev, s, true);
	else
		daqgert_ai_setup_eoc(dev, s);

	if (cmd->start_src == TRIG_NOW) {
		s->async->inttrig = NULL;
		/* enable this acquisition operation */
		devpriv->run = false;
		devpriv->timer = true;
		daqgert_ai_start_pacer(dev, true);
		devpriv->ai_cmd_running = true;
		devpriv->ai_cmd_canceled = false;
	} else {
		/* TRIG_INT */
		/* don't enable the acquisition operation */
		/* wait for an internal signal */
		s->async->inttrig = daqgert_ai_inttrig;
	}

	devpriv->timing_lockout++;
ai_cmd_exit:
	mutex_unlock(&devpriv->cmd_lock);
	return ret;
}

/* get close to a good sample spacing for one second, test_mode is to see what the max sample rate is */
static int32_t daqgert_ao_delay_rate(struct comedi_device *dev, int32_t rate, int32_t device_type, bool test_mode)
{
	const struct daqgert_board *board = dev->board_ptr;
	struct daqgert_private *devpriv = dev->private;
	int32_t spacing_usecs = 0, sample_freq, total_sample_time, delay_time;
	if (test_mode) {
		dev_info(dev->class_dev, "ao speed testing: rate %i, spacing usecs %i\n", rate, spacing_usecs);
		return spacing_usecs;
	}

	if (rate <= 0) rate = board->ao_rate_min;
	if (rate > devpriv->ao_rate_max) rate = devpriv->ao_rate_max;
	sample_freq = devpriv->ao_rate_max / rate;
	total_sample_time = board->ao_ns_min * sample_freq; /* ns time needed for all samples in one second */
	delay_time = devpriv->ao_rate_max - total_sample_time; /* what's left */
	if (delay_time >= sample_freq) { /* something */
		spacing_usecs = (delay_time / sample_freq) / NS_TO_MS;
		if (spacing_usecs < 0) spacing_usecs = 0;
	} else { /* or nothing */
		spacing_usecs = 0;
	}
	dev_info(dev->class_dev, "ao rate %i, spacing usecs %i\n", rate, spacing_usecs);
	return spacing_usecs;
}

static int32_t daqgert_ao_cmdtest(struct comedi_device *dev,
	struct comedi_subdevice *s, struct comedi_cmd *cmd)
{
	const struct daqgert_board *board = dev->board_ptr;
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;
	int32_t i, err = 0;
	uint32_t flags;
	uint32_t tmp_timer;

	if (!devpriv)
		return -EFAULT;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);

	/*
	 * all conversion events happen simultaneously with
	 * a rate of 1kHz/n
	 */
	flags = TRIG_NOW;
	err |= cfc_check_trigger_src(&cmd->convert_src, flags);

	/* start a new scan (output at once) with a timer */
	flags = TRIG_TIMER;
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, flags);


	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_COUNT | TRIG_NONE);

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

	err |= cfc_check_trigger_is_unique(cmd->start_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_FOLLOW) /* internal trigger */
		err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);


	if (cmd->scan_begin_src == TRIG_TIMER) {
		i = 1;
		/* find a power of 2 for the number of channels */
		while (i < (cmd->chanlist_len))
			i = i * 2;
		tmp_timer = (board->ao_ns_min_calc / 2) * i; /* 2 is num of channels */
		err |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg, tmp_timer); /* fastest */
		/* now calc the real sampling rate with all the
		 * rounding errors */
		tmp_timer = ((uint32_t) (cmd->scan_begin_arg / board->ao_ns_min)) * board->ao_ns_min;
		pdata->delay_usecs = daqgert_ao_delay_rate(dev, tmp_timer, spi_data->device_type, speed_test);
		err |= cfc_check_trigger_arg_max(&cmd->scan_begin_arg, devpriv->ao_rate_max); /* slowest */
	} else {
		pdata->delay_usecs = 0;
	}

	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);

	if (cmd->stop_src == TRIG_COUNT)
		err |= cfc_check_trigger_arg_min(&cmd->stop_arg, 1);
	else /* TRIG_NONE */
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);

	if (err)
		return 3;

	/* step 4: fix up any arguments */

	if (err)
		return 4;

	return 0;
}

static int32_t daqgert_ai_poll(struct comedi_device *dev, struct comedi_subdevice * s)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t num_bytes;

	if (!devpriv->ai_hunk)
		return 0; /* poll is valid only for HUNK transfer */

	dev_info(dev->class_dev, "ai_poll\n");
	num_bytes = comedi_buf_n_bytes_ready(s);
	return num_bytes;
}

/* get close to a good sample spacing for one second, test_mode is to see what the max sample rate is */
static int32_t daqgert_ai_delay_rate(struct comedi_device *dev, int32_t rate, int32_t device_type, bool test_mode)
{
	const struct daqgert_board *board = dev->board_ptr;
	struct daqgert_private *devpriv = dev->private;
	int32_t spacing_usecs = 0, sample_freq, total_sample_time, delay_time;

	if (test_mode) {
		dev_info(dev->class_dev, "ai speed testing: rate %i, spacing usecs %i\n", rate, spacing_usecs);
		return spacing_usecs;
	}

	if (rate <= 0) rate = board->ai_rate_min;
	if (rate > devpriv->ai_rate_max) rate = devpriv->ai_rate_max;
	sample_freq = devpriv->ai_rate_max / rate;
	total_sample_time = board->ai_ns_min * sample_freq; /* time needed for all samples */
	delay_time = devpriv->ai_rate_max - total_sample_time;
	if (delay_time >= sample_freq) {
		spacing_usecs = (delay_time / sample_freq) / NS_TO_MS;
		if (spacing_usecs < 0) spacing_usecs = 0;
	} else {
		spacing_usecs = 0;
	}
	spacing_usecs += CONV_SPEED_FIX;
	if (device_type == MCP3002) spacing_usecs += CONV_SPEED_FIX_FAST;
	dev_info(dev->class_dev, "ai rate %i, spacing usecs %i\n", rate, spacing_usecs);
	return spacing_usecs;
}

/* For some scans we can do a quasi-DMA-like transfer that's much faster */
static int32_t daqgert_ai_cmdtest(struct comedi_device *dev,
	struct comedi_subdevice *s, struct comedi_cmd * cmd)
{
	const struct daqgert_board *board = dev->board_ptr;
	struct daqgert_private *devpriv = dev->private;
	struct spi_param_type *spi_data = s->private;
	struct spi_device *spi = spi_data->spi;
	struct comedi_control *pdata = spi->dev.platform_data;

	int32_t i, err = 0;
	uint32_t divisor1 = 0, divisor2 = 0;
	uint32_t arg;
	uint32_t tmp_timer;

	/* Step 1 : check if triggers are trivially valid */

	err |= cfc_check_trigger_src(&cmd->start_src, TRIG_NOW | TRIG_INT);
	err |= cfc_check_trigger_src(&cmd->scan_begin_src, TRIG_FOLLOW | TRIG_TIMER);
	//	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_TIMER | TRIG_NOW);
	err |= cfc_check_trigger_src(&cmd->convert_src, TRIG_TIMER);
	err |= cfc_check_trigger_src(&cmd->scan_end_src, TRIG_COUNT);
	err |= cfc_check_trigger_src(&cmd->stop_src, TRIG_NONE | TRIG_COUNT);

	if (err)
		return 1;

	/* Step 2a : make sure trigger sources are unique */

	err |= cfc_check_trigger_is_unique(cmd->start_src);
	err |= cfc_check_trigger_is_unique(cmd->stop_src);

	/* Step 2b : and mutually compatible */

	if (err)
		return 2;

	/* Step 3: check if arguments are trivially valid */

	err |= cfc_check_trigger_arg_is(&cmd->start_arg, 0);

	if (cmd->scan_begin_src == TRIG_FOLLOW) /* internal trigger */
		err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, 0);

	if (cmd->scan_begin_src == TRIG_TIMER) {
		i = 1;
		/* find a power of 2 for the number of channels */
		while (i < (cmd->chanlist_len))
			i = i * 2;
		err |= cfc_check_trigger_arg_min(&cmd->scan_begin_arg, board->ai_ns_min_calc / 2 * i); /* 2 is num of channels */
		/* now calc the real sampling rate with all the
		 * rounding errors */
		tmp_timer = ((uint32_t) (cmd->scan_begin_arg / board->ai_ns_min)) * board->ai_ns_min;
		pdata->delay_usecs = daqgert_ai_delay_rate(dev, tmp_timer, spi_data->device_type, speed_test);
		pdata->mix_delay_usecs = pdata->delay_usecs < 2; /* double delay with zero for the first scan chan */
		err |= cfc_check_trigger_arg_is(&cmd->scan_begin_arg, tmp_timer);
	}

	if (cmd->convert_src == TRIG_TIMER)
		err |= cfc_check_trigger_arg_min(&cmd->convert_arg,
		board->ai_ns_min);

	err |= cfc_check_trigger_arg_min(&cmd->chanlist_len, 1);
	err |= cfc_check_trigger_arg_is(&cmd->scan_end_arg, cmd->chanlist_len);


	if (cmd->stop_src == TRIG_COUNT)
		err |= cfc_check_trigger_arg_min(&cmd->stop_arg, 1);
	else /* TRIG_NONE */
		err |= cfc_check_trigger_arg_is(&cmd->stop_arg, 0);

	if (err)
		return 3;

	/* step 4: fix up any arguments */
	if (cmd->convert_src == TRIG_NOW) {
		pdata->delay_usecs = 0;
		pdata->mix_delay_usecs = pdata->delay_usecs < 2; /* double delay with zero for the first scan chan */
	}

	if (cmd->convert_src == TRIG_TIMER) {
		arg = cmd->convert_arg;
		i8253_cascade_ns_to_timer(devpriv->ai_conv_delay_10nsecs,
			&divisor1,
			&divisor2,
			&arg, cmd->flags);
		pdata->delay_usecs = daqgert_ai_delay_rate(dev, arg, spi_data->device_type, speed_test);
		pdata->mix_delay_usecs = pdata->delay_usecs < 2; /* double delay with zero for the first scan chan */
		err |= cfc_check_trigger_arg_is(&cmd->convert_arg, arg);
	}

	if (err)
		return 4;

	return 0;
}

static void my_timer_ai_callback(unsigned long data)
{
	struct comedi_device *dev = (void*) data;
	struct daqgert_private *devpriv = dev->private;
	static uint32_t time_marks = 0;

	if (!devpriv->run) {
		devpriv->run = true;
		devpriv->timer = true;
	}
	daqgert_ai_start_pacer(dev, true);
	if (speed_test) {

		if (!(time_marks++ % 1000))
			dev_info(dev->class_dev, "speed testing %i: count %i, hunk %i, length %i\n",
			time_marks, ai_count, hunk_count, hunk_len);
	}
}

static void daqgert_ai_clear_eoc(struct comedi_device * dev)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t count = 500;

	del_timer_sync(&devpriv->ai_spi->my_timer);
	setup_timer(&devpriv->ai_spi->my_timer, my_timer_ai_callback, (unsigned long) dev);
	devpriv->run = false;
	devpriv->timer = false;
	do { /* wait if needed to SPI to clear or timeout */
		schedule(); /* force a context switch */
		msleep(1);
	} while (devpriv->spi_ai_run && (count--));

	devpriv->run = false;
	devpriv->timer = false;
}

static int32_t daqgert_ai_cancel(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct daqgert_private *devpriv = dev->private;

	if (!devpriv->ai_cmd_running)
		return 0;

	daqgert_ai_clear_eoc(dev);
	dev_info(dev->class_dev, "ai cancel\n");
	ai_count = devpriv->ai_count;
	hunk_count = devpriv->hunk_count;
	devpriv->ai_hunk = false;
	s->async->cur_chan = 0;
	s->async->inttrig = NULL;
	devpriv->ai_cmd_canceled = true;
	devpriv->ai_cmd_running = false;
	devpriv->timing_lockout--;
	if (devpriv->timing_lockout < 0) devpriv->timing_lockout = 0;
	return 0;
}

static int32_t daqgert_ao_cancel(struct comedi_device *dev,
	struct comedi_subdevice *s)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t count = 500;

	if (!devpriv->ao_cmd_running)
		return 0;

	dev_info(dev->class_dev, "ao cancel\n");
	ao_count = devpriv->ao_count;
	s->async->cur_chan = 0;
	devpriv->ao_cmd_running = false;
	do { /* wait if needed to SPI to clear or timeout */
		schedule(); /* force a context switch to stop the AO thread */
		msleep(1);
	} while (devpriv->spi_ao_run && (count--));

	s->async->inttrig = NULL;
	devpriv->timing_lockout--;
	if (devpriv->timing_lockout < 0) devpriv->timing_lockout = 0;
	devpriv->ao_cmd_canceled = true;
	return 0;
}

/* FIXME Slow brute forced IO bits, 5us reads from userland */

/* need to use (fix) state to optimize changes */
static int32_t daqgert_dio_insn_bits(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, uint32_t *data)
{
	struct daqgert_private *devpriv = dev->private;
	int32_t pinWPi;
	uint32_t val = 0, mask = 0;

	/* s->state contains the GPIO bits */
	/* s->io_bits contains the GPIO direction */

	/* i/o testing with gpio pins  */
	/* We need to shift a single bit from state to set or clear the GPIO */
	for (pinWPi = 0; pinWPi < s->n_chan; pinWPi++) {
		mask = comedi_dio_update_state(s, data);
		if (wpi_pin_safe(dev, pinWPi)) {
			/* Do nothing on SPI AUX pins */
			if (mask) {
				if (mask & 0xffffffff)
					devpriv->digitalWrite(dev, pinWPi,
					(s->state & (0x01 << pinWPi)) >> pinWPi); /* output writes */
			}
			val = s->state & 0xffffffff;
			val |= (devpriv->digitalRead(dev, pinWPi) << pinWPi); /* input reads shift */
		}
		data[1] = val;
	}

	return insn->n;
}

/* query or change DIO config */
static int32_t daqgert_dio_insn_config(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, uint32_t *data)
{
	struct daqgert_private *devpriv = dev->private;
	uint32_t wpi_pin = CR_CHAN(insn->chanspec), chan = 1 << wpi_pin;

	switch (data[0]) {
	case INSN_CONFIG_DIO_OUTPUT:
		if (wpi_pin_safe(dev, wpi_pin)) {
			s->io_bits |= chan;
			devpriv->pinMode(dev, wpi_pin, OUTPUT);
		}
		break;
	case INSN_CONFIG_DIO_INPUT:
		if (wpi_pin_safe(dev, wpi_pin)) {
			s->io_bits &= (~chan);
			devpriv->pinMode(dev, wpi_pin, INPUT);
			pullUpDnControl(dev, wpi_pin, pullups);
		}
		break;
	case INSN_CONFIG_DIO_QUERY:
		data[1] = (s->io_bits & chan) ? COMEDI_OUTPUT : COMEDI_INPUT;
		return insn->n;
	default:
		return -EINVAL;
	}
	dev_dbg(dev->class_dev, "%s: gpio pins setting 0x%x\n",
		dev->board_name,
		(uint32_t) s->io_bits);

	return insn->n;

}

/* Talk to the ADC via the SPI */
static int32_t daqgert_ai_rinsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, uint32_t *data)
{
	int32_t ret = -EBUSY;
	int32_t n;
	struct daqgert_private *devpriv = dev->private;

	mutex_lock(&devpriv->cmd_lock);
	if (devpriv->ai_cmd_running)
		goto ai_read_exit;

	mutex_lock(&devpriv->drvdata_lock);
	devpriv->ai_hunk = false;
	devpriv->ai_chan = CR_CHAN(insn->chanspec);
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
static int32_t daqgert_ao_winsn(struct comedi_device *dev,
	struct comedi_subdevice *s,
	struct comedi_insn *insn, uint32_t *data)
{
	uint32_t chan = CR_CHAN(insn->chanspec);
	uint32_t n, val = s->readback[chan];

	daqgert_ao_set_chan_range(dev, insn->chanspec, 1);
	for (n = 0; n < insn->n; n++) {
		val = data[n];
		daqgert_ao_put_sample(dev, s, val);
	}
	return insn->n;
}

static int32_t daqgert_ai_config(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct spi_param_type *spi_data = s->private;

	/* Stuff here? */
	return spi_data->chan;
}

static int32_t daqgert_ao_config(struct comedi_device *dev,
	struct comedi_subdevice * s)
{
	struct spi_param_type *spi_data = s->private;

	/* Stuff here? */
	return spi_data->chan;
}

static int32_t daqgert_auto_attach(struct comedi_device *dev, unsigned long context)
{
	const struct daqgert_board *thisboard = &daqgert_boards[gert_type];
	struct comedi_subdevice *s;
	int32_t ret, i;
	int32_t num_ai_chan, num_ao_chan, num_dio_chan = NUM_DIO_CHAN;
	struct daqgert_private *devpriv;

	devpriv = comedi_alloc_devpriv(dev, sizeof(*devpriv)); /* auto free on exit */
	if (!devpriv)
		return -ENOMEM;

	mutex_init(&devpriv->cmd_lock);
	mutex_init(&devpriv->drvdata_lock);

	/* Board  operation data */
	devpriv->ai_cmd_delay_usecs = 10; /* PIC slave delays */
	devpriv->ai_conv_delay_usecs = 30;
	devpriv->ai_neverending = true;
	devpriv->hunk = use_hunking;
	devpriv->ai_mix = false;
	devpriv->ai_spi = &spi_adc;
	devpriv->ao_spi = &spi_dac;
	devpriv->ai_conv_delay_10nsecs = CONV_SPEED;
	devpriv->timing_lockout = 0;

	dev->board_ptr = thisboard;
	dev->board_name = thisboard->name;

	devpriv->ai_rate_max = MAX_BOARD_RATE; /* lowest samples per second */
	devpriv->ao_rate_max = MAX_BOARD_RATE;

	/* Use the kernel system_rev EXPORT_SYMBOL */
	devpriv->RPisys_rev = system_rev; /* what board are we running on? */
	if (devpriv->RPisys_rev < 2) {
		dev_err(dev->class_dev, "invalid RPi board revision! %u\n", devpriv->RPisys_rev);
		return -EINVAL;
	}

	dev->iobase = GPIO_BASE; /* bcm iobase */
	/* dev->mmio is a void pointer with 8bit pointer indexing, 
	 * we need 32bit indexing so mmio is casted to a (__iomem uint32_t*) 
	 * pointer for GPIO R/W operations 
	 */
	dev->mmio = ioremap(dev->iobase, SZ_16K); /* lets get access to the GPIO base */
	if (!dev->mmio) {
		dev_err(dev->class_dev, "invalid gpio io base address!\n");
		return -EINVAL;
	}

	devpriv->timer_1mhz = ioremap(ST_BASE, 8); /* lets get access to the timer base */
	if (!devpriv->timer_1mhz) {
		dev_err(dev->class_dev, "invalid 1mhz timer base address!\n");
		return -EINVAL;
	}

	/* setup the pins in a static matter for now */
	/* PIN mode for all */
	if (wiringpi) {
		dev_info(dev->class_dev, "%s WiringPiSetup\n", thisboard->name);
		if (wiringPiSetup(dev) < 0) {
			dev_err(dev->class_dev, "board gpio detection failed!\n");
			return -EINVAL;
		}
	} else {
		dev_info(dev->class_dev, "%s GpioPiSetup\n", thisboard->name);
		if (wiringPiSetupGpio(dev) < 0) {
			dev_err(dev->class_dev, "board gpio detection failed!\n");
			return -EINVAL;
		}
	}

	devpriv->board_rev = piBoardRev(dev);
	switch (devpriv->board_rev) {
	case 2:
		num_dio_chan = NUM_DIO_CHAN_REV2; /* This a Rev 2 board "I hope" */
		break;
	case 3:
		num_dio_chan = NUM_DIO_CHAN_REV3; /* This a Rev 3 board "I hope" */
		break;
	default:
		num_dio_chan = NUM_DIO_CHAN; /* Rev 1 board setup */
	}

	if (wiringpi) {
		for (i = 0; i < NUM_DIO_OUTPUTS; i++) { /* [0..7] OUTPUTS */
			devpriv->pinMode(dev, i, OUTPUT);
		}
		dev_info(dev->class_dev, "%s WPi pins set [0..7] to outputs\n", thisboard->name);
	}

	/* assume we have DON"T have a Gertboard */
	dev_info(dev->class_dev, "%s detection started\n", thisboard->name);
	devpriv->num_subdev = 1;
	if (daqgert_spi_probe(dev)) devpriv->num_subdev += 2;
	/* add AI and AO channels */
	dev_info(dev->class_dev, "%s detection completed\n", thisboard->name);
	ret = comedi_alloc_subdevices(dev, devpriv->num_subdev);
	if (ret) {
		dev_err(dev->class_dev, "alloc subdevice(s) failed!\n");
		return ret;
	}
	dev_info(dev->class_dev, "%i subdevices\n", dev->n_subdevices);
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

	if (devpriv->num_subdev > 1) { /* we have the SPI ADC DAC on board */
		/* daq_gert ai */
		if (devpriv->hunk) dev_info(dev->class_dev, "hunk ai transfers enabled, length: %i\n", hunk_len);
		s = &dev->subdevices[1];
		s->private = devpriv->ai_spi; /* SPI adc comedi state */
		num_ai_chan = daqgert_ai_config(dev, s); /* config SPI ports for ai use */
		s->type = COMEDI_SUBD_AI;
		/* we support single-ended (ground)  */
		s->subdev_flags = SDF_READABLE | SDF_GROUND | SDF_CMD_READ | SDF_COMMON;
		s->n_chan = num_ai_chan;
		s->len_chanlist = num_ai_chan;
		s->maxdata = (1 << (12 - devpriv->ai_spi->device_type)) - 1;
		if (devpriv->ai_spi->range) {
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

		/* setup the timer to call my_timer_ai_callback */
		setup_timer(&devpriv->ai_spi->my_timer, my_timer_ai_callback, (unsigned long) dev);

		/* daq-gert ao */
		s = &dev->subdevices[2];
		s->private = devpriv->ao_spi; /* SPI dac comedi state */
		num_ao_chan = daqgert_ao_config(dev, s); /* config SPI ports for ao use */
		s->type = COMEDI_SUBD_AO;
		/* we support single-ended (ground)  */
		s->subdev_flags = SDF_WRITABLE | SDF_GROUND | SDF_CMD_WRITE;
		s->n_chan = num_ao_chan;
		s->len_chanlist = num_ao_chan;
		s->maxdata = (1 << 12) - 1; /* the actual analog resolution depends on the DAC chip 8,10,12 */
		s->range_table = &daqgert_ao_range;
		s->insn_write = daqgert_ao_winsn;
		s->insn_read = comedi_readback_insn_read;
		s->do_cmdtest = daqgert_ao_cmdtest;
		s->do_cmd = daqgert_ao_cmd;
		s->cancel = daqgert_ao_cancel;
		ret = comedi_alloc_subdev_readback(s);
		if (ret) {
			dev_err(dev->class_dev, "alloc subdevice readback failed!\n");
			return ret;
		}
	}
	/* setup kthreads */
	if (devpriv->hunk) {
		devpriv->ai_spi->daqgert_task = kthread_run(&daqgert_ai_thread_function, (void *) dev, "daqgert_h_a");
		devpriv->ao_spi->daqgert_task = kthread_run(&daqgert_ao_thread_function, (void *) dev, "daqgert_h_d");
	} else {
		devpriv->ai_spi->daqgert_task = kthread_run(&daqgert_ai_thread_function, (void *) dev, "daqgert_a");
		devpriv->ao_spi->daqgert_task = kthread_run(&daqgert_ao_thread_function, (void *) dev, "daqgert_d");
	}
	dev_info(dev->class_dev, "daq_gert SPI i/o threads started\n");

	dev_info(dev->class_dev, "%s attached: gpio iobase 0x%lx, ioremaps 0x%lx  0x%lx, io pins 0x%x, 1Mhz timer value 0x%x:0x%x\n",
		dev->driver->driver_name,
		dev->iobase,
		(long unsigned int) dev->mmio,
		(long unsigned int) devpriv->timer_1mhz,
		(uint32_t) s->io_bits,
		(uint32_t) ioread32(devpriv->timer_1mhz + 2),
		(uint32_t) ioread32(devpriv->timer_1mhz + 1));

	return 0;
}

static void daqgert_detach(struct comedi_device * dev)
{
	struct daqgert_private *devpriv = dev->private;

	if (devpriv->ao_spi->daqgert_task) kthread_stop(devpriv->ao_spi->daqgert_task);
	devpriv->ao_spi->daqgert_task = NULL;

	if (devpriv->ai_spi->daqgert_task) kthread_stop(devpriv->ai_spi->daqgert_task);
	devpriv->ai_spi->daqgert_task = NULL;

	if (1) {
		/* remove kernel timer when unloading module */
		del_timer_sync(&devpriv->ai_spi->my_timer);
	}

	iounmap(devpriv->timer_1mhz);
	iounmap(dev->mmio);
	dev_info(dev->class_dev, "daq_gert detached\n");
}

static struct comedi_driver daqgert_driver = {
	.driver_name = "daq_gert",
	.module = THIS_MODULE,
	.auto_attach = daqgert_auto_attach,
	.detach = daqgert_detach,
	.board_name = &daqgert_boards[0].name,
	.num_names = ARRAY_SIZE(daqgert_boards),
	.offset = sizeof(struct daqgert_board),
};

static struct spi_driver spidev_spi_driver;

/* called for each listed spigert device in the bcm270*.c file */
static int32_t spidev_spi_probe(struct spi_device * spi)
{
	struct comedi_control *pdata;
	int32_t ret;

	pdata = kzalloc(sizeof(struct comedi_control), GFP_KERNEL);
	if (!pdata) return -ENOMEM;
	mutex_init(&pdata->daqgert_platform_lock);

	/* allocate the SPI transfer buffer structures */
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
		/* get a copy of the slave device 0 to share with comedi */ /* we need a device to talk to the ADC */
		spi_adc.spi = spi;
		spi->max_speed_hz = 1000000;
	}
	if (spi->chip_select == CSnB) {
		/* get a copy of the slave device 1 to share with comedi */ /* we need a device to talk to the DAC */
		spi_dac.spi = spi;
		spi->max_speed_hz = 8000000;
	}
	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_3; /* mode 3 for ADC & DAC*/
	spi_setup(spi);
	dev_info(&spi->dev,
		"setup: cd %d: %d Hz, bpw %u, mode 0x%x\n",
		spi->chip_select, spi->max_speed_hz, spi->bits_per_word,
		spi->mode);

	/* speed adjustments */
	pdata->delay_usecs = 0; /* delay between any single conversion */
	pdata->mix_delay_usecs = 0; /* delay for alt mix command conversions */

	/* Check for basic errors */
	ret = spi_w8r8(spi, 0); /* check for spi comm error */
	if (ret < 0) {
		dev_err(&spi->dev, "spi comm error\n");
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

static int32_t spidev_spi_remove(struct spi_device * spi)
{
	struct comedi_control *pdata = spi->dev.platform_data;

	if (pdata->rx_buff)
		kfree(pdata->rx_buff);
	if (pdata->tx_buff)
		kfree(pdata->tx_buff);
	if (pdata)
		kfree(pdata);
	dev_info(&spi->dev, "released\n");

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
 * setup and probe the spi bus for devices, save the data to the global spi variables
 */
static int32_t daqgert_spi_probe(struct comedi_device * dev)
{
	int32_t ret;
	const struct daqgert_board *thisboard = dev->board_ptr;

	dev_info(dev->class_dev, "spi probe\n");
	if (!spi_adc.spi) {
		dev_err(dev->class_dev, "no spi channel detected\n");
		spi_adc.chan = 0;
		spi_dac.chan = 0;
		return spi_adc.chan;
	}

	spi_dac.chan = thisboard->n_aochan;

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
	/* SPI data transfers, send a few dummies for config info */
	spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	ret = spi_w8r8(spi_adc.spi, CMD_DUMMY_CFG);
	dev_info(dev->class_dev,
		"%s adc board pre detect code %i, daqgert_conf option value %i\n",
		thisboard->name, ret, daqgert_conf);
	if ((ret != 76) && (ret != 110)) { /* PIC slave adc codes */
		ret = spi_w8r8(spi_adc.spi, 0b01100000); /* check for channel 0 SE */
		if (1) { /* FIXME need to add another probe test */
			spi_adc.pic18 = 0; /* MCP3X02 mode */
			spi_adc.chan = thisboard->n_aichan;
			spi_adc.range = 0; /* range 2.048 */
			dev_info(dev->class_dev,
				"%s adc chip board detected, %i channels, range code %i, device code %i, PIC code %i, detect code %i\n",
				thisboard->name, spi_adc.chan, spi_adc.range, spi_adc.device_type, spi_adc.pic18, ret);
			return spi_adc.chan;
		}
		spi_adc.pic18 = 0; /* SPI probes found nothing */
		dev_info(dev->class_dev, "no %s adc found, gpio pins only. detect code %i\n",
			thisboard->name, ret);
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
			"PIC spi slave adc chip board detected, %i channels, range code %i, device code %i, bits code %i, PIC code %i, detect Code %i\n",
			spi_adc.chan, spi_adc.range, spi_adc.device_type, spi_adc.bits, spi_adc.pic18, ret);
	} else {
		spi_adc.pic18 = 0; /* SPI probes found nothing */
		/* look for the gertboard SPI devices .pic18 code 1 */
		dev_info(dev->class_dev, "no %s PIC found, gpio pins only. Detect code %i\n",
			thisboard->name, ret);
		spi_adc.chan = 0;

		return spi_adc.chan;
	}
	return spi_adc.chan;
}

static int32_t __init daqgert_init(void)
{
	int32_t ret;

	ret = spi_register_driver(&spidev_spi_driver);
	if (ret < 0)
		return ret;
	ret = comedi_driver_register(&daqgert_driver);
	if (ret < 0)
		return ret;
	if (!spi_adc.spi || !spi_dac.spi) return -ENODEV;
	if (gert_autoload)
		ret = comedi_auto_config(&spi_adc.spi->master->dev, &daqgert_driver, 0);
	if (ret < 0)
		return ret;

	return 0;
}
module_init(daqgert_init);

static void __exit daqgert_exit(void)
{
	comedi_auto_unconfig(&spi_adc.spi->master->dev);
	comedi_driver_unregister(&daqgert_driver);
	spi_unregister_driver(&spidev_spi_driver);
}
module_exit(daqgert_exit);

MODULE_AUTHOR("Fred Brooks <spam@sma2.rain.com>");
MODULE_DESCRIPTION("RPi DIO/AI/AO Driver");
MODULE_VERSION("0.0.27");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:spigert");
