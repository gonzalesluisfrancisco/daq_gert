/*
 * Demo code for driver testing, a simple console display of data inputs and voltage
 *
 * This file may be freely modified, distributed, and combined with
 * other software, as long as proper attribution is given in the
 * source code.
 */

#include <stdlib.h>
#include <stdio.h>	/* for printf() */
#include <unistd.h>
#include <string.h>
#include <comedilib.h>
#include "bmc/bmc.h"
#include "bmc/daq.h"

struct bmcdata bmc; /* DIO buffer */

int main(int argc, char *argv[]) {
    int blink[2], flip[2] = {0, 0};
    if (init_daq(0.0, 25.0, FALSE) < 0) {
        printf("Missing Analog subdevice(s)\n");
        return -1;
    }
    if (init_dio() < 0) {
        printf("Missing Digital subdevice(s)\n");
        return -1;
    }
    set_dio_output(0);
    set_dio_output(1);
    put_dio_bit(0, 1);
    put_dio_bit(1, 1);
    get_data_sample();

    while (1) {


        printf("         \r");
        get_data_sample();
        printf(" %2.3fV %2.3fV %2.3fV %2.3fV %2.3fV %2.3fV %2.3fV %u %u %u %u %u %u raw %x, %x",
                bmc.pv_voltage, bmc.cc_voltage, bmc.input_voltage, bmc.b1_voltage, bmc.b2_voltage, bmc.system_voltage, bmc.logic_voltage,
                bmc.datain.D0, bmc.datain.D1, bmc.datain.D2, bmc.datain.D3, bmc.datain.D6, bmc.datain.D7,bmc.adc_sample[0],bmc.adc_sample[1]);
        usleep(4990);
        if ((bmc.datain.D6 == 0) || 1) {
            if (((blink[0]++) % 150) == 0) {
                flip[0] = !flip[0];
                set_dio_input(6); // wpi channel
            }
            printf(" Flip led 0 %x ", flip[0]);
            bmc.dataout.D0 = flip[0];
            set_dac_volts(0, bmc.cc_voltage);
        } else {
            set_dac_volts(0, 0.666);
            bmc.dataout.D0 = 0;
        }
        if ((bmc.datain.D7 == 0) || 1) {
            if (((blink[1]++) % 150) == 0) {
                flip[1] = !flip[1];
                set_dio_input(7); // wpi channel
            }
            printf(" Flip led 1 %x ", flip[1]);
            set_dac_volts(1, 0.333);
            bmc.dataout.D1 = flip[1];
        } else {
            set_dac_volts(1, 1.666);
            bmc.dataout.D1 = 0;
        }
    }
    return 0;
}


