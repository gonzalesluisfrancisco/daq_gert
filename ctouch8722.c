// PIC18F8722 Configuration Bit Settings

// 'C' source line config statements

#include <p18f8722.h>

// CONFIG1H
#pragma config OSC = HSPLL      // Oscillator Selection bits (HS oscillator, PLL enabled (Clock Frequency = 4 x FOSC1))
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Two-Speed Start-up disabled)

// CONFIG2L
#pragma config PWRT = OFF       // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 3         // Brown-out Voltage bits (Minimum setting)

// CONFIG2H
#pragma config WDT = ON         // Watchdog Timer (WDT enabled)
#pragma config WDTPS = 512     // Watchdog Timer Postscale Select bits (1:1024)

// CONFIG3L
#pragma config MODE = MC        // Processor Data Memory Mode Select bits (Microcontroller mode)
#pragma config ADDRBW = ADDR20BIT// Address Bus Width Select bits (20-bit Address Bus)
#pragma config DATABW = DATA16BIT// Data Bus Width Select bit (16-bit External Bus mode)
#pragma config WAIT = OFF       // External Bus Data Wait Enable bit (Wait selections are unavailable for table reads and table writes)

// CONFIG3H
#pragma config CCP2MX = PORTC   // CCP2 MUX bit (ECCP2 input/output is multiplexed with RC1)
#pragma config ECCPMX = PORTE   // ECCP MUX bit (ECCP1/3 (P1B/P1C/P3B/P3C) are multiplexed onto RE6, RE5, RE4 and RE3 respectively)
#pragma config LPT1OSC = OFF    // Low-Power Timer1 Oscillator Enable bit (Timer1 configured for higher power operation)
#pragma config MCLRE = ON       // MCLR Pin Enable bit (MCLR pin enabled; RG5 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP enabled)
#pragma config BBSIZ = BB2K     // Boot Block Size Select bits (1K word (2 Kbytes) Boot Block size)
#pragma config XINST = OFF       // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection bit Block 0 (Block 0 (000800, 001000 or 002000-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection bit Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection bit Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection bit Block 3 (Block 3 (00C000-00FFFFh) not code-protected)
#pragma config CP4 = OFF        // Code Protection bit Block 4 (Block 4 (010000-013FFFh) not code-protected)
#pragma config CP5 = OFF        // Code Protection bit Block 5 (Block 5 (014000-017FFFh) not code-protected)
#pragma config CP6 = OFF        // Code Protection bit Block 6 (Block 6 (01BFFF-018000h) not code-protected)
#pragma config CP7 = OFF        // Code Protection bit Block 7 (Block 7 (01C000-01FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot Block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection bit Block 0 (Block 0 (000800, 001000 or 002000-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection bit Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection bit Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection bit Block 3 (Block 3 (00C000-00FFFFh) not write-protected)
#pragma config WRT4 = OFF       // Write Protection bit Block 4 (Block 4 (010000-013FFFh) not write-protected)
#pragma config WRT5 = OFF       // Write Protection bit Block 5 (Block 5 (014000-017FFFh) not write-protected)
#pragma config WRT6 = OFF       // Write Protection bit Block 6 (Block 6 (01BFFF-018000h) not write-protected)
#pragma config WRT7 = OFF       // Write Protection bit Block 7 (Block 7 (01C000-01FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-007FFF, 000FFF or 001FFFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection bit Block 0 (Block 0 (000800, 001000 or 002000-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection bit Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection bit Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection bit Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR4 = OFF      // Table Read Protection bit Block 4 (Block 4 (010000-013FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR5 = OFF      // Table Read Protection bit Block 5 (Block 5 (014000-017FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR6 = OFF      // Table Read Protection bit Block 6 (Block 6 (018000-01BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR7 = OFF      // Table Read Protection bit Block 7 (Block 7 (01C000-01FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-007FFF, 000FFF or 001FFFh) not protected from table reads executed in other blocks)

/*
 * This program converts the rs-232 output from a ELO Carroll-Touch touch-screen controller
 * to a format that can be used with the Varian E220/E500 Implanter
 * The Carroll controller must be first programmed
 * USART1 is the host comm port
 * USART2 is the touch-screen comm port
 * A jumper between PORTD pin6 and +vcc enables debug mode. REMOVE when done.
 * PORTD		switch input
 * PORTJ		LED bar display
 * PORTH0		run flasher led onboard.
 * 2x16 LCD status panel and 8 led status lights.
 *
 * Fred Brooks, Microchip Inc , Aug 2009,2015
 * Gresham, Oregon
 *
 *
 * This application is designed for use with the
 * ET-BASE PIC8722 board and  device.
 * HOST RS-232  5-1     uC port1
 * Female       2-2-tx
 *              3-3-rx
 * LCD  RS-232  5-1     uC port2
 * Male         2-3-rx
 *              3-2-tx
 */

#include <usart.h>
#include <delays.h>
#include <string.h>
#include <stdlib.h>
#include <EEP.h>
#include <GenericTypeDefs.h>

#ifdef INTTYPES
#include <stdint.h>
#else
#define INTTYPES
/*unsigned types*/
typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t;
typedef unsigned long long uint64_t;
/*signed types*/
typedef signed char int8_t;
typedef signed int int16_t;
typedef signed long int32_t;
typedef signed long long int64_t;
#endif


void rx_handler(void);

#define	DO_CAP	FALSE			// save data for usarts 1&2, save to eeprom
#define	TS_TYPE	0			// 0 for old CRT type screens, 1 for newer LCD screens
#define SET_EMU	TRUE

#define BUF_SIZE 64
#define	CAP_SIZE 256
#define	CMD_SIZE 2
#define	CMD_OVERFLOW	CMD_SIZE*12
#define ELO_SIZE 12
#define ELO_SEQ 10
#define FALSE	0
#define TRUE	1
#define	BLINK_RATE	20000
#define JB	FALSE
#define AUTO_RESTART	FALSE
#define SINGLE_TOUCH	FALSE
#define GOOD_MAX	128		// max number of chars from TS without expected frames seen
#define MAX_CAM_TIME	5
#define MAX_CAM_TIMEOUT	30
#define MAX_CAM_TOUCH	5
#define CAM_RELAY	LATAbits.LATA1
#define CAM_RELAY_AUX	LATEbits.LATE1
#define CAM_RELAY_TIME	LATEbits.LATE2

typedef struct reporttype {
	uint8_t headder, status;
	uint16_t x_cord, y_cord, z_cord;
	uint8_t checksum;
} volatile reporttype;

volatile uint16_t CATCH = FALSE, i = 0, y = 0, LED_UP = TRUE, z = 0, TOUCH = FALSE, UNTOUCH = FALSE,
	CATCH46 = FALSE, CATCH37 = FALSE, TSTATUS = FALSE, cdelay = 900, NEEDSETUP = FALSE,
	CRTFIX_DEBUG = 1, DATA1 = FALSE, DATA2 = FALSE, speedup = 0, LEARN1 = FALSE,
	LEARN2 = FALSE, CORNER1 = FALSE, CORNER2 = FALSE, CAM = FALSE;
volatile uint8_t touch_good = 0, cam_time = 0, do_emu = SET_EMU, ACK = FALSE, INPACKET = FALSE;
volatile int32_t j = 0, alive_led = 0, touch_count = 0, resync_count = 0, rawint_count = 0, status_count = 0;

/*
 * Step #1  The data is allocated into its own section.
 */
#pragma idata bigdata

//				***
//				E0.94		Clean up the code and comments
//				E0.95		Add startup delay for remote service terminals
//				E0.96		status reporting and monitor testing
//				E0.97		port bit testing
//				E0.98		fix cylon led roll.
//				E0.99		debug 1,2 Screen size code results changed with portD bit 0,1
//				E1.00
//				E1.01		debug 8 on single/tracking touch modes portD bit 7
//				E1.02		debug 7 on flash LCD while processing. portD bit 6
//				E1.03		code fixes/updates
//				E1.04		add delay in status/touched host send routines
//				E1.05		add interlocks for touch input from screen
//				E1.06		add WDT counter test switch input and checks for valid ts inputs.
//				E1.07		screen connect restart code via WDT timeout.
//				E1.08		Learn touchs and set with special touch sequence.
//				E1.09		Code for 2 special touchs and remove the delay switch. bit3 learn1, bit2 learn2
//				E1.10		External output to led/relay on PORTE, RE0,RE7 mirrors HA0 led
//				E1.11		add JB define for switch board missing.
//				E1.12-13	fix LCD display
//				E1.14		fix rs-232 flags
//				E1.15		Small coding  cleanups
//				E1.16		remove WDT calls in ISR, check for proper comms with the touch screen and controller.
//				E1.17		auto init touchscreen code.
//				E1.18		Code for new LCD screens and debug capture.
//				E1.19		recode ISR to remove library define functions
//				E1.20		VGA/CAM switcher code.
//				E1.21		Timed camera for left press, software smells
//				E1.22		Support for SmartSet commands on newer touch panels
//				***

char lcdstr[18] = "CRTFIX E1.22FB |", lcdstatus[18] = "D", lcdstatus_touched[18] = "Touched",
	tmp_str[18] = "   ", debugstr[18] = "DEBUG jmpr#6   |", bootstr1[18] = "Power Up        ",
	bootstr2[18] = "Status: OK     ";
volatile uint8_t elobuf[BUF_SIZE], ssbuf[BUF_SIZE], spinchr, commchr = ' ';

volatile struct reporttype *ssreport = (reporttype *) ssbuf;

uint8_t elocodes_m[ELO_SIZE] = {// 5 char, soft-reset,touch scanning off, report transfer on, (0x26) tracking mode, report transfer on, clear touch buffer, touch scanning on
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x3c, 0x2b, 0x44, 0x26, 0x44, 0x3d, 0x2a
}; // initial touch config codes, tracking
uint8_t elocodes_s[ELO_SIZE] = {// same as above ex (0x25) enter point mode
	0x0d, 0x0d, 0x0d, 0x0d, 0x0d, 0x3c, 0x2b, 0x44, 0x25, 0x44, 0x3d, 0x2a
}; // initial touch config codes, single

uint8_t elocodes_e0[ELO_SIZE] = {// emulate setup E271-2200 TOUCH BINARY
	0x55, 'Q', 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e1[ELO_SIZE] = {
	0x55, 'Q', 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e2[ELO_SIZE] = {
	0x55, 'M', 0b00000011, 0b01001000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e3[ELO_SIZE] = {
	0x55, 'S', 'X', 0x00, 0x00, 0x00, 0x71, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e4[ELO_SIZE] = {
	0x55, 'S', 'Y', 0x00, 0x00, 0x00, 0x59, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e5[ELO_SIZE] = {
	0x55, 'a', 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e6[ELO_SIZE] = {
	0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
uint8_t elocodes_e7[ELO_SIZE] = {// dummy packet
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

volatile uint16_t tchar, uchar, debug_port = 0, restart_delay = 0, touch_saved = 0, touch_sent = 0, touch_corner1 = 0,
	touch_corner_timed = 0, corner_skip = 0;

#pragma idata

#pragma idata sddata
volatile uint8_t host_rec[CAP_SIZE] = "H";
volatile uint8_t scrn_rec[CAP_SIZE] = "S";
#pragma idata 

volatile uint8_t host_write = FALSE, scrn_write = FALSE;

#pragma code rx_interrupt = 0x8

void rx_int(void)
{
	_asm goto rx_handler _endasm
}
#pragma code

#pragma interrupt rx_handler

void rx_handler(void)
{
	uint8_t c;
	static uint16_t scrn_ptr = 0, host_ptr = 0, do_cap = DO_CAP, junk;
	static uint8_t sum = 0;

	if (INTCONbits.RBIF) {
		junk = PORTB;
		INTCONbits.RBIF = 0;
	}
	rawint_count++; // debug counters
	if (!do_cap) {
		LATJbits.LATJ7 = 1; //  led off before checking serial ports
		LATJbits.LATJ0 = 0;
	}
	commchr = '*';

	/* Get the character received from the USARTs */

	if (PIR3bits.RC2IF) { // is data from touchscreen
		LATEbits.LATE4 = !LATEbits.LATE4;
		if (CAM && (cam_time > MAX_CAM_TIME)) {
			CAM_RELAY_TIME = 0;
			CAM_RELAY_AUX = 0; // clear video switch
			CAM_RELAY = 0; // clear video switch
			CAM = FALSE;
		}
		if (RCSTA2bits.OERR) {
			RCSTA2bits.CREN = 0; //	clear overrun
			RCSTA2bits.CREN = 1; // re-enable
		}
		c = RCREG2; // read data from touchscreen
		if (do_cap) {
			if (scrn_ptr < CAP_SIZE) {
				scrn_rec[scrn_ptr] = RCREG2; // read data from touchscreen
				while (!TXSTA1bits.TRMT) {
				}; // wait until the usart is clear
				TXREG1 = scrn_rec[scrn_ptr];
				scrn_ptr++;
				LATJbits.LATJ2 = !LATJbits.LATJ2; // flash  led
			} else {
				scrn_write = TRUE;
				LATJbits.LATJ4 = !LATJbits.LATJ4; // flash  led
			}
		} else {
			if (do_emu) {
				if (i == 0 && c == 'U') { // look for start of packet
					INPACKET = TRUE;
					sum = 0xAA + 'U';
				}

				touch_good++; // chars received before a status report
				LATEbits.LATE0 = 1; // flash external led
				LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				DATA2 = TRUE; // usart is connected to data
				if (TOUCH || ACK) {
					ssbuf[i++] = c;
					sum += c;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
					if (i == ELO_SEQ) { // end of report
						if (sum == c) { //checksum match
						}
						i = 0;
						INPACKET = FALSE;
						restart_delay = 0;
						if (TOUCH) {
							CATCH = TRUE;
							LATF = ssreport->x_cord;
						}
						TOUCH = FALSE; // stop buffering touchscreen data.
						ACK = FALSE;
						sum = 0;
					};
				};
				if (c == 'T' && (!CATCH) && (!ACK)) { // looks like a touch report
					TOUCH = TRUE;
					TSTATUS = TRUE;
					restart_delay = 0;
					CATCH = FALSE;
					i = 1;
					ssbuf[i++] = c;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				};
				if (c == 'A' && (!ACK) && (!TOUCH)) { // looks like a status report
					ACK = TRUE;
					TSTATUS = TRUE;
					restart_delay = 0;
					LATJbits.LATJ6 = 0; // led 6 touch-screen connected
					speedup = -10000;
					i = 1;
					ssbuf[i++] = c;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				};
				if (i > (BUF_SIZE - 2)) {
					i = 0; // stop buffer-overflow
					TOUCH = FALSE;
					CATCH = FALSE;
					ACK = FALSE;
				};
				if (touch_good > GOOD_MAX) { // check for max count and no host to get touch data
					LATEbits.LATE0 = 1; // LED off
					LATEbits.LATE7 = LATEbits.LATE0;
					touch_good = 0;
				}
			} else {
				touch_good++; // chars received before a status report
				LATEbits.LATE0 = 1; // flash external led
				LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				DATA2 = TRUE; // usart is connected to data
				if (TOUCH) {
					elobuf[i++] = c;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
					if (c == 0xFF && TOUCH) { // end of report
						CATCH = TRUE;
						restart_delay = 0;
						TOUCH = FALSE; // stop buffering touchscreen data.
					};
				};
				if (c == 0xFE && (!CATCH)) { // looks like a touch report
					TOUCH = TRUE;
					TSTATUS = TRUE;
					restart_delay = 0;
					CATCH = FALSE;
					i = 0;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				};
				if (c == 0xF5) { // looks like a status report
					TSTATUS = TRUE;
					restart_delay = 0;
					LATJbits.LATJ6 = 0; // led 6 touch-screen connected
					speedup = -10000;
					LATEbits.LATE0 = 1; // flash external led
					LATEbits.LATE7 = LATEbits.LATE0; // flash external led
				};
				if (i > (BUF_SIZE - 2)) {
					i = 0; // stop buffer-overflow
					TOUCH = FALSE;
					CATCH = FALSE;
				};
				if (touch_good > GOOD_MAX) { // check for max count and no host to get touch data
					LATEbits.LATE0 = 1; // LED off
					LATEbits.LATE7 = LATEbits.LATE0;
					while (TRUE) { // lockup for reboot
						touch_good++;
					};
				}
			}
		}
	};


	if (PIR1bits.RC1IF) { // is data from host
		LATEbits.LATE3 = !LATEbits.LATE3;
		if (RCSTA1bits.OERR) {
			RCSTA1bits.CREN = 0; //	clear overrun
			RCSTA1bits.CREN = 1; // re-enable
		}
		if (do_cap) {
			if (host_ptr < CAP_SIZE) {
				host_rec[host_ptr] = RCREG1; // read data from touchscreen
				while (!TXSTA2bits.TRMT) {
				}; // wait until the usart is clear
				TXREG2 = host_rec[host_ptr];
				host_ptr++;
				LATJbits.LATJ1 = !LATJbits.LATJ1; // flash  led
			} else {
				tchar = RCREG1; // read from host
				host_write = TRUE;
				LATJbits.LATJ3 = !LATJbits.LATJ3; // flash  led
			}
		} else {
			tchar = RCREG1; // read from host
			DATA1 = TRUE; // usart is connected to data
			if ((tchar == (uint8_t) 0x46)) { // send one report to host
				CATCH46 = TRUE;
				touch_good = 0;
			}
			if ((tchar == (uint8_t) 0x37)) { // start of touch scan read
				CATCH37 = TRUE;
				touch_good = 0;
			}
			if ((tchar == (uint8_t) 0x3C)) { // touch reset from host
				NEEDSETUP = FALSE;
			}
		};
	}
}

void write_touch_eeprom(uint8_t data, uint8_t count, uint16_t addr, uint16_t offset)
{
	//  eeprom data array: 0=0x57 checksum, 1=length of array 2=start of array data
	Busy_eep();
	Write_b_eep(0 + offset, 0x57); //	write checksum 0x57 at byte 0 of the offset

	Busy_eep();
	Write_b_eep(1 + offset, count); // length of data

	Busy_eep();
	Write_b_eep(addr + 2 + offset, data); //  data
}

uint8_t read_touch_eeprom(uint16_t addr, uint16_t offset)
{
	Busy_eep();
	return Read_b_eep(addr + offset);
}

void wdtdelay(uint16_t delay)
{
	uint16_t dcount;

	for (dcount = 0; dcount <= delay; dcount++) { // delay a bit							// all leds on
		ClrWdt(); // reset the WDT timer
	};
}

void touch_cam(void)
{

	//	check for corner presses
	if (CATCH) {
		if ((elobuf[0] <= (uint8_t) 0x06) && (elobuf[1] >= (uint8_t) 0x5a)) { // check for left bottom corner
			touch_corner1++;
			touch_corner_timed = TRUE;
		};

		if ((elobuf[0] >= (uint8_t) 0x72) && (elobuf[1] >= (uint8_t) 0x5a)) { // check for right bottom corner
			touch_corner1++;
		};
	};

	if (touch_corner1 >= MAX_CAM_TOUCH) { // we have several corner presses 
		CAM = TRUE;
		cam_time = 0;
		CAM_RELAY_TIME = 1;
		touch_corner1 = 0;
		CAM_RELAY_AUX = 1; // set secondary VGA/CAM switch
		CAM_RELAY = 1; // set primary VGA/CAM switch
		elobuf[0] = 0;
		elobuf[1] = 0;
	};
}

void elocmdout(uint8_t *elostr)
{
	LATJbits.LATJ5 = !LATJbits.LATJ5; // touch screen commands led
	while (Busy2USART()) {
	}; // wait until the usart is clear
	putc2USART(elostr[0]);
	wdtdelay(30000);
	while (Busy2USART()) {
	}; // wait until the usart is clear
}

void eloSScmdout(uint8_t *elostr)
{
	LATJbits.LATJ5 = !LATJbits.LATJ5; // touch screen commands led
	while (Busy2USART()) {
	}; // wait until the usart is clear
	putc2USART(elostr[0]);
	while (Busy2USART()) {
	}; // wait until the usart is clear
}

void elopacketout(uint8_t *strptr, uint8_t strcount)
{
	uint8_t i;
	for (i = 0; i < strcount; i++) {
		eloSScmdout(&strptr[i]);
	};
}

void setup_lcd(void)
{
	uint16_t code_count;
	uint8_t single_t = SINGLE_TOUCH;

	if (do_emu) {
		elopacketout(elocodes_e7, ELO_SEQ); // dummy packet
		elopacketout(elocodes_e0, ELO_SEQ);
		elopacketout(elocodes_e2, ELO_SEQ);
		elopacketout(elocodes_e3, ELO_SEQ);
		elopacketout(elocodes_e4, ELO_SEQ);
		elopacketout(elocodes_e1, ELO_SEQ);
	} else {

		if (TS_TYPE == 1) single_t = FALSE;
		for (code_count = 0; code_count < ELO_SIZE; code_count++) {
			if (single_t) {
				elocmdout(&elocodes_s[code_count]);
			} else {
				elocmdout(&elocodes_m[code_count]);
			}
		};
	}

	NEEDSETUP = FALSE;
}

void putc1(uint16_t c)
{
	while (Busy1USART()) {
	}; // wait until the usart is clear
	putc1USART(c);
}

void putc2(uint16_t c)
{
	while (Busy2USART()) {
	}; // wait until the usart is clear
	putc2USART(c);
}

void start_delay(void)
{
	wdtdelay(50000);
}

uint16_t Test_Screen(void)
{
	while (Busy2USART()) {
	}; // wait until the usart is clear
	if (do_emu) return TRUE;
	putc2(0x46);
	wdtdelay(30000);
	if (DATA2) {
		setup_lcd(); // send lcd touch controller setup codes
		return TRUE;
	} else {
		setup_lcd(); // send lcd touch controller setup codes
		return FALSE;
	}
}

void main(void)
{
	uint16_t spinner = 0, eep_ptr;
	uint8_t scaled_char;
	float rez_scale_h = 1.0, rez_parm_h, rez_scale_v = 1.0, rez_parm_v;

	/* Configure all LAT B,E,H,J pins for output */
	TRISJ = 0;
	TRISH = 0;
	TRISE = 0;
	TRISF = 0;
	TRISB = 0;
	TRISA = 0;
	TRISD = 0xff;
	PORTH = 0;
	LATE = 0;
	CAM_RELAY_TIME = 0;
	CAM_RELAY = 0;
	touch_count = 0;
	CAM = 0;
	INTCON = 0;
	INTCON2 = 0;
	INTCON3 = 0;
	if (read_touch_eeprom(0, 0) == (uint8_t) 0x57) CORNER1 = TRUE;
	if (read_touch_eeprom(0, 512) == (uint8_t) 0x57) CORNER2 = TRUE;

	strcpypgm2ram(bootstr1, "Booting Program    ");
	start_delay(); // wait for touch-screen to powerup and be ready
	strcpypgm2ram(bootstr1, "Power up delay     ");
	start_delay(); // wait for touch-screen to powerup and be ready

	/*
	 * Open the USART configured as0
	 * 8N1, 9600 baud, in receive INT mode
	 */
	Open1USART(USART_TX_INT_OFF &
		USART_RX_INT_ON &
		USART_ASYNCH_MODE &
		USART_EIGHT_BIT &
		USART_CONT_RX &
		USART_BRGH_LOW, 64); // 40mhz osc HS		9600 baud, USART_BRGH_LOW and 64, for 9600 baud @ 40MHz   (0.16% ERROR IN RATE)
	// USART_BRGH_HIGH and 64, for 9600 baud @ 10MHz  (0.16% ERROR IN RATE)

	Open2USART(USART_TX_INT_OFF &
		USART_RX_INT_ON &
		USART_ASYNCH_MODE &
		USART_EIGHT_BIT &
		USART_CONT_RX &
		USART_BRGH_LOW, 64); // 40mhz osc HS		9600 baud, USART_BRGH_LOW and 64, for 9600 baud @ 40MHz   (0.16% ERROR IN RATE)
	// USART_BRGH_HIGH and 64, for 9600 baud @ 10MHz  (0.16% ERROR IN RATE)

	while (DataRdy1USART()) { // dump 1 rx data`
		z = Read1USART();
	};
	while (DataRdy2USART()) { // dump 2 rx data
		z = Read2USART();
	};
	/* Disable interrupt priority */
	RCONbits.IPEN = 0;

	PIE1bits.RC1IE = 1; // com1 int unmask
	PIE3bits.RC2IE = 1; // com2 int unmask
	INTCONbits.RBIE = 0; // enable PORTB interrupts 1=enable
	//INTCON2bits.RBIP = 1; // Set the PORTB interrupt-on-change as a high priority interrupt
	/* Enable all interrupts */
	INTCONbits.GIE = 1; // global int enable
	INTCONbits.PEIE = 1; // enable all unmasked int

	if (!DO_CAP) {
		strcpypgm2ram(bootstr1, "Configure Screen   ");
		start_delay();
		setup_lcd(); // send lcd touch controller setup codes
	}

	LATJ = 0xff; // set leds to off at powerup/reset
	DATA1 = FALSE; // reset COMM flags.
	DATA2 = FALSE; // reset touch COMM flag
	// leds from outputs to ground via resistor.

	while (DO_CAP) {
		ClrWdt();
		LATJbits.LATJ6 = !LATJbits.LATJ6; // flash  led								// reset the WDT timer
		if (host_write || scrn_write) {
			eep_ptr = 0;
			while (eep_ptr <= 255) {
				LATJbits.LATJ0 = !LATJbits.LATJ0; // flash  led
				INTCONbits.GIE = 0; // global int enable
				INTCONbits.PEIE = 0; // enable all unmasked int
				Busy_eep();
				Write_b_eep(eep_ptr, host_rec[eep_ptr]); //  data
				ClrWdt(); // reset the WDT timer
				Busy_eep();
				Write_b_eep(eep_ptr + CAP_SIZE, scrn_rec[eep_ptr]); //  data
				Busy_eep();
				INTCONbits.GIE = 1; // global int enable
				INTCONbits.PEIE = 1; // enable all unmasked int
				ClrWdt(); // reset the WDT timer
				eep_ptr++;
			}
			INTCONbits.GIE = 0; // global int enable
			INTCONbits.PEIE = 0; // enable all unmasked int
			while (DO_CAP) {
				LATJ = 0x00; // all on
				ClrWdt(); // reset the WDT timer
			}
		}
	}

	if (Test_Screen()) {
		if (TSTATUS) {
			strcpypgm2ram(bootstr1, "Status:OK TS    ");
		} else {
			strcpypgm2ram(bootstr1, "Status:COM2 Data");
		}
	} else {
		strcpypgm2ram(bootstr1, "Status:No Screen");
	}


	/* Loop forever */
	while (TRUE) {

		if (j++ >= (BLINK_RATE + speedup)) { // delay a bit ok
			if (spinner++ > (uint16_t) 2) {
				spinchr = '|';
				spinner = 0;
			} else {
				spinchr = '-';
			}
			LATJbits.LATJ1 = 1;
			LATJbits.LATJ2 = 1;
			LATJbits.LATJ3 = 1;
			if (alive_led == 2) LATJbits.LATJ1 = 0; // roll leds cylon style
			if (alive_led == 4) LATJbits.LATJ2 = 0;
			if (alive_led == 8) LATJbits.LATJ3 = 0;
			LATHbits.LATH0 = !LATHbits.LATH0; // flash onboard led
			if (cam_time > MAX_CAM_TIMEOUT) {
				CAM_RELAY_TIME = 0;
				if (touch_corner_timed) {
					touch_corner_timed = FALSE;
					CAM_RELAY_TIME = 0;
					CAM_RELAY_AUX = 0; // clear video switch
					CAM_RELAY = 0; // clear video switch
					CAM = FALSE;
				}
			}
			cam_time++;
			LATEbits.LATE0 = !LATEbits.LATE0; // flash external led
			LATEbits.LATE7 = LATEbits.LATE0; // flash external led

			/*		For the auto-restart switch						*/
			if (AUTO_RESTART) { // enable auto-restarts
				if ((restart_delay++ >= (uint16_t) 60) && (!TSTATUS)) { // try and reinit lcd after delay
					start_delay();
					setup_lcd(); // send lcd touch controller setup codes
					start_delay();
					while (TRUE) {
					}; // lockup WDT counter to restart
				} else {
					if ((restart_delay >= (uint16_t) 150) && (TSTATUS)) { // after delay restart TS status.
						TSTATUS = FALSE; // lost comms while connected
						restart_delay = 0;
					};
				};
			};



			if (LED_UP && (alive_led != 0)) {
				alive_led = alive_led * 2;
			} else {
				if (alive_led != 0) alive_led = alive_led / 2;
			}
			if (alive_led < 2) {
				alive_led = 2;
				LED_UP = TRUE;
			} else {
				if (alive_led > 8) {
					alive_led = 8;
					LED_UP = FALSE;
				}
			}
			j = 0;
		}

		LATJbits.LATJ4 = !LATJbits.LATJ4; // toggle bits program run led
		touch_cam(); // always check the cam touch

		if (CATCH46) { // flag to send report to host
			LATJbits.LATJ0 = 1; // flash status led

			if (CATCH) { // send the buffered touch report
				Delay10KTCYx(75); // 75 ms
				putc1(0xFE); // send position report header
				if (do_emu) {
					rez_parm_h = ((float) (ssreport->x_cord)) * rez_scale_h;
					scaled_char = ((uint16_t) (rez_parm_h));
					putc1(scaled_char); // send h scaled touch coord
					rez_parm_v = ((float) (ssreport->y_cord)) * rez_scale_v;
					scaled_char = ((uint16_t) (rez_parm_v));
					putc1(scaled_char); // send v scaled touch coord
				} else {
					rez_parm_h = ((float) (elobuf[0])) * rez_scale_h;
					scaled_char = ((uint16_t) (rez_parm_h));
					putc1(scaled_char); // send h scaled touch coord
					rez_parm_v = ((float) (elobuf[1])) * rez_scale_v;
					scaled_char = ((uint16_t) (rez_parm_v));
					putc1(scaled_char); // send v scaled touch coord
					i = 0;
				}
				putc1(0xFF); // send end of report
				touch_count++;
				CATCH = FALSE;
				CATCH46 = FALSE;
			} else { // just send status
				Delay10KTCYx(65); // 65 ms
				putc1(0xF5); // send status report
				putc1(0xFF); // end of report
				status_count++;
				CATCH46 = FALSE;
			};
		};

		if (CATCH37) { // send screen size codes
			LATJbits.LATJ7 = 0; // off blink for rez codes sent
			Delay10KTCYx(75); // 75 ms
			rez_scale_h = 1.0; // LCD touch screen real H/V rez
			rez_scale_v = 1.0;
			if (do_emu) {
				elopacketout(elocodes_e5, ELO_SEQ); // send a ACk query
			} else {
				putc2(0x3D); // send clear buffer to touch
			}
			putc1(0xF4); // send status report
			if (TS_TYPE == 0) { // CRT type screens
				putc1(0x77); // touch parm
				putc1(0x5f); // touch parm
			}
			if (TS_TYPE == 1) { // new LCD type screens
				putc1(0x71); // touch parm
				putc1(0x59); // touch parm
			}
			putc1(0xFF); // end of report
			resync_count++;
			CATCH37 = FALSE;
		};

		if (TOUCH) {
			// do nothing now.
		};

		if (NEEDSETUP) setup_lcd(); // send lcdsetup codes to screen

		/*	check for port errors	*/
		if (RCSTA1bits.OERR == (uint8_t) 1) {
			LATJ = 0xFF; // all leds off with error
			LATE != (uint16_t) 0x02; // overrun clear error and reenable receiver 1
			RCSTA1bits.CREN = 0;
			RCSTA1bits.CREN = 1;
		}
		if (RCSTA2bits.OERR == (uint8_t) 1) {
			LATJ = 0xFF; // all leds off with error
			LATE != (uint8_t) 0x08; // overrun clear error and reenable receiver 2
			RCSTA2bits.CREN = 0;
			RCSTA2bits.CREN = 1;
		}
		ClrWdt(); // reset the WDT timer
	};
}
