

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../config/os_settings.h"

#include "drv_adc.h"
#include "../../mcc_generated_files/pwm.h"

void __attribute__ ( ( __interrupt__ , auto_psv) ) _ADCAN0Interrupt ( void )
{
    uint16_t valPOT;
    uint16_t PWMLimit;
    //Read the ADC value from the ADCBUF
    valPOT = ADCBUF0<<3;

    //Limit with Max 50% DC
    if(valPOT > (PG4PER >> 1)) PWMLimit = PG4PER >>1;
    else PWMLimit = valPOT;
    
    PWM_DutyCycleSet(4, PWMLimit);

    //clear the POT interrupt flag
    IFS5bits.ADCAN0IF = 0;
}

