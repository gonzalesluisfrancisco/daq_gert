daq_gert
========
Comedi driver for RPi ai, ao gpio for the  gertboard daq_gert.c
Driver: "experimental" daq_gert in progress ... for 4.+ kernels

The DAQ-GERT appears in Comedi as a  digital I/O subdevice (0) with
17 or 21 or 30 channels, 
a analog input subdevice (1) with 2 single-ended channels with onboard adc, OR
a analog input subdevice (1) with single-ended channels set by the SPI slave device
a analog output subdevice(2) with 2 channels with onboard dac

It's mainly a test driver for a modded version of xoscope in COMEDI mode

Current Status:
Most of the DIO basics work but not totally correctly and speed
on ADC samples is limited to about 20,000 S/sec on a RPi2


Notes:
This driver requires a kernel patch to gain direct SPI access at the kernel. 

 * git clone https://github.com/raspberrypi/linux.git in /usr/src for the latest
 * linux kernel source tree
 * 
 * cd to the linux kernel source directory: /usr/src/linux etc...
 * copy the daq_gert.diff patch file from the daq_gert directory to here
 * copy RPI2.config or RPi.confg the from the daq_gert directory to .config in the Linux source directory
 * 
 * patch the kernel source with the daq_gert.diff patch file
 * patch -p1 <daq_gert.diff
 * 
 *  make -j4 for a RPi 2
 *  select SPI_COMEDI=y in SPI MASTERS to enable the SPI side of the driver (SPI_SPIDEV must be deselected )
 *  select DAQ_GERT=m to select the Comedi protocol part of the driver
 *  make modules_install
 *  to recompile the Linux kernel with the Comedi SPI link and to make the needed daq_gert module
 *  then copy the Image file to the /boot directory with a new kernel image name
 *  and modify the boot file to use that image
 *  after the reboot: daq_gert should auto-load to device /dev/comedi0
 *  if the legacy option is set in /etc/modprobe.d/comedi.conf the new device will be created after those
 *  dmesg should see the kernel module messages from daq_gert
 *  run the test program: bmc_test_program to see if it's working
 * 
 * Module parameters are found in the /sys/modules/daq_gert/parameters directory
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


Comments:
Things are looking pretty good for the MCP3X02 ADC driver section of the code. 
I can get a 50usec per sample avg over a 1 second burst period using xoscope 
with 25usecs for wire-speed and ~25 for CPU overhead without DMA. 
Without DMA the output data stream is not totally continuous at full speed as I 
have to stop the SPI transfer to process and copy data but that can be improved  
with a double-buffer on the SPI side by using another thread and core. 
With a driver request for less than full speed sample rates the software inserts 
calculated delays between samples (or groups of samples during a two channel scan) 
to adjust the sample to the correct time-splice. I'm thinking about having the 
option to always run a full speed and then integrate the extra samples 
(costing more CPU cycles) into one per the requested time-splice. 
(runs at 20,000 S/sec, requests for 1000 S/sec, driver integrates every 
20 samples to 1 for output to the data consumer) 

*  PIC Slave Info:
Currently on runs in a slow polled mode with the PIC18 but the plan is to offload 
commands to it with a PIC24 version with 12bit samples at much higher speeds 
and auto sequencing.
