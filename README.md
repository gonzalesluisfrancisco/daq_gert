daq_gert
========
Comedi driver for RPi ai, ao gpio for the  gertboard daq_gert.c
Driver: "experimental" daq_gert in progress ... for 4.+ kernels with DT
on the RPi2 with the bcm2835 SPI master with async Comedi commands and triggers
The single processor version for the RPi has only the sync analog sample capability
**********
Device-tree operation is now working:  Use overlay rpi-spigert-overlay.dtb
to load the device module in /boot/config.txt: 
dtparam=spi=on
dtoverlay=rpi-spigert-overlay.dtb
**********

The DAQ-GERT appears in Comedi as a  digital I/O subdevice (0) with
17 or 21 or 30 channels, 
a analog input subdevice (1) with 2 single-ended channels with onboard adc, OR
a analog input subdevice (1) with single-ended channels set by the SPI slave device
and a analog output subdevice(2) with 2 channels with onboard dac

It's mainly a test driver for a modded version of xoscope in COMEDI mode

Current Status:
Most of the DIO basics work but not totally correctly and speed
on ADC samples is limited to about 20,000 S/sec on a RPi2


Notes:
This driver currently requires a kernel patch to gain direct SPI access at the kernel level
with an optional cs_change_usecs patch to spi.c and spi.h 
needed to reduce delays in cs_change=true transfers
comment out #define CS_CHANGE_USECS in the daq_gert.c source if you don't use this patch

 * git clone https://github.com/raspberrypi/linux.git in /usr/src for the latest
 * linux kernel source tree
 * 
 * cd to the linux kernel source directory: /usr/src/linux etc...
 * copy the daq_gert.diff patch file from the daq_gert directory to the source
 * directory 
 * copy RPi2.config_4.0.y from the daq_gert directory to .config in the Linux source directory
 * or
 * copy RPi.config_4.0.y from the daq_gert directory to .config in the Linux source directory
 * 
 * patch the kernel source with the daq_gert.diff patch file
 * patch -p1 <daq_gert.diff
 * 
 *  make menuconfig or xconfig
 *  select SPI_COMEDI=m in SPI MASTERS to enable the SPI side of the driver 
 *  select DAQ_GERT=m to select the Comedi protocol part of the driver in the 
 *  staging daq comedi misc drivers section.
 *  make -j4 for a RPi 2 to compile a new kernel and driver in much less time
 *  use the instructions here to build the new kernel, modules, device-trees and overlays.
 *  https://www.raspberrypi.org/documentation/linux/kernel/building.md
 *
 *  after the reboot: daq_gert should auto-load to device /dev/comedi0*
 *  if the legacy option is set in /etc/modprobe.d/comedi.conf the new device will be created after those
 *  dmesg should see the kernel module messages from daq_gert
 *  run the test program: bmc_test_program to see if it's working
 * 
 * 
The input  range is 0 to 1023/4095 for 0.0 to 3.3(Vdd) onboard devices or 2.048 volts/Vdd for PIC slaves 
The output range is 0 to 4095 for 0.0 to 2.048 onboard devices (output resolution depends on the device)
 * In the async command mode transfers can be handled in HUNK mode by creating a SPI message
 * of many conversion sequences into one message, this allows for close to native driver wire-speed 

Analog: The type and resolution of the onboard ADC/DAC chips are set
by the module option variable daqgert_conf in the /etc/modprobe.d directory

 * options daq_gert daqgert_conf=1 gert_autoload=1
 * 
daqgert_conf options:
0 = Factory Gertboard configuratin of MCP3002 ADC and MCP4802 ADC: 10bit in/8bit out
1 = MCP3202 ADC and MCP4822 DAC: 12bit in/12bit out 
2 = MCP3002 ADC and MCP4822 DAC: 10bit in/12bit out
3 = MCP3202 ADC and MCP4802 DAC: 12bit in/8bit out

gert_autolocal options:
0 = don't autoload (mainly for testing)
1 = load and configure daq_gert on boot (default)

All modules parameters can read in  /sys/module/daq_gert/parameters


Comments:
Things are looking pretty good for the MCP3X02 ADC driver section of the code. 
I can get a 30usec per sample avg over a 1 second burst period using xoscope 
with 26usecs for wire-speed and ~5 for CPU overhead without DMA for 12bit reads. 
Without DMA the output data stream is not totally continuous at full speed as I 
have to stop the SPI transfer to process and copy data.
With a driver request for less than full speed sample rates the software inserts 
calculated delays between samples (or groups of samples during a two channel scan) 
to adjust the sample to the correct time-splice.

*  PIC Slave Info:
Currently on runs in a slow polled mode with the PIC18 but the plan is to offload 
commands to it with a PIC24 version with 12bit samples at much higher speeds 
and auto sequencing.
