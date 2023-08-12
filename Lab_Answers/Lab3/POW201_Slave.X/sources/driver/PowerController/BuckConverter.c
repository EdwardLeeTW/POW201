/* ********************************************************************************
 * 3p3z Controller Configuration Code Example
 * ********************************************************************************
 *
 * The following code example covers all user-defined configurations of the
 * control object 'VCOMP' required to run the recently configured controller.
 *
 * Please note:
 * This is no executable code and serves only as template for the integration into
 * a proprietary firmware project. You can copy & paste this entire code example
 * to your code project by clicking the 'Copy to Clipboard' above or pick specific
 * section by selecting a text section and pressing <CTRL>+'C'.
 *
 * Once code has been copied to user code, each setting has to be configured by
 * replacing the placeholders <[data type]> in each code line by your individual
 * configuration values.
 *
 * This code example only lists configurations of features which have been selected
 * in this tool. Enabling/disabling code generation options may also change the
 * contents of this code example. Unused options are EXCLUDED from this code example.
 *
 * ********************************************************************************/

#define __BuckConverter

// Standard Include Files
#include <stdint.h>                               // include standard integer data types
#include <stdbool.h>                              // include standard boolean data types
#include <stddef.h>                               // include standard definition data types

// 3p3z Controller Include Files
#include "VCOMP.h"                                // include 'VCOMP' controller header file
#include "BuckConverter.h"
#include "../../../mcc_generated_files/pin_manager.h"

volatile uint16_t VCOMP_ControlObject_Initialize(void)
{
    volatile uint16_t retval = 0;                 // Auxiliary variable for function call verification (initially set to ZERO = false)
    Buck_Vref = 0;
    /* Controller Input and Output Ports Configuration */

     // Configure Controller Primary Input Port
    VCOMP.Ports.Source.ptrAddress = &ADCBUF1;   // Pointer to primary feedback source (e.g. ADC buffer register or variable)
    VCOMP.Ports.Source.Offset = 0;              // Primary feedback signal offset
    VCOMP.Ports.Source.NormScaler = 0;          // Primary feedback normalization factor bit-shift scaler
    VCOMP.Ports.Source.NormFactor = 0x7FFF;     // Primary feedback normalization factor fractional

    // Configure Controller Primary Output Port
    VCOMP.Ports.Target.ptrAddress = &PG7DC;     // Pointer to primary output target (e.g. SFR register or variable)
    VCOMP.Ports.Target.Offset = 0;              // Primary output offset value
    VCOMP.Ports.Target.NormScaler = 0;          // Primary output normalization factor bit-shift scaler
    VCOMP.Ports.Target.NormFactor = 0x7FFF;     // Primary output normalization factor fractional
 
    // Configure Control Reference Port
    VCOMP.Ports.ptrControlReference = &Buck_Vref; // Pointer to control reference (user-variable)
  
    /* Controller Output Limits Configuration */

    // Primary Control Output Limit Configuration
    VCOMP.Limits.MinOutput = VCOMP_MIN_CLAMP;        // Minimum control output value
    VCOMP.Limits.MaxOutput = VCOMP_MAX_CLAMP;        // Maximum control output value
    
    /* Advanced Parameter Configuration */

    // Initialize User Data Space Buffer Variables
    VCOMP.Advanced.usrParam0 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam1 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam2 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam3 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam4 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam5 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam6 = 0;                 // No additional advanced control options used
    VCOMP.Advanced.usrParam7 = 0;                 // No additional advanced control options used

    /* Controller Status Word Configuration */

    VCOMP.status.bits.enabled = false;            // Keep controller disabled

    // Call Assembler Control Library Initialization Function
    retval = VCOMP_Initialize(&VCOMP);            // Initialize controller data arrays and number scalers

    return(retval);

}

/* 3p3z Control Loop Interrupt Service Routine for Controller Object 'VCOMP'
 * **************************************************************************************************
 * This code example of a interrupt service routine uses the tailored name label '_VCOMP'.
 * The Assembler library code sequences of controller data objects generated by PS-DCLD are optimized
 * for being called by a PWM interrupt for minimum response time. However, in some applications
 * it might be desired to call the control loop from other interrupt sources.
 * Using custom labels for interrupt routines allows using generic interrupt service routine
 * function calls in code, which can be mapped to specific interrupt sources by adding a #define
 * pre-compiler directive declaration to your code, like the following example:
 *
 *    #define _VCOMP_Interrupt    _PWM1Interrupt  // Define label for interrupt service routine function call
 *    #define _VCOMP_ISRIF        _PWM1IF         // Define label for interrupt flag bit
 *
 * **************************************************************************************************/

void __attribute__ ( ( __interrupt__ , auto_psv) ) _ADCAN1Interrupt ( void )
{
    //LED2_SetHigh();
    
    //VCOMP_Update(&VCOMP);               // Call control loop
    VCOMP_PTermUpdate(&VCOMP);        // Call P-Term control loop
    
    //LED2_SetLow();
    IFS5bits.ADCAN1IF = 0;             // Clear the interrupt flag
}


//**********************************************************************************
// Download latest version of this tool here:
//      https://www.microchip.com/powersmart
//**********************************************************************************

// Simple Softstart
void Buck_Softstart(void)
{
    VCOMP.status.bits.enabled = true;           // Enable controller   
    if(Buck_Vref < VCOMP_VREF)
    {
        Buck_Vref+=10;
    }
    else
    {
        Buck_Vref = VCOMP_VREF;
    }
}
