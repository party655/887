/* 
 * File:   pin_manager_.h
 * Author: 111
 *
 * Created on 5 ??????? 2021 ?., 11:37
 */

#include <xc.h>
#include <pic16f887.h>

#define INPUT   1
#define OUTPUT  0

#define HIGH    1
#define LOW     0

#define ANALOG      1
#define DIGITAL     0

#define PULL_UP_ENABLED      1
#define PULL_UP_DISABLED     0



// get/set pinDIO aliases
#define pinDIO_TRIS                 TRISBbits.TRISB4

#define pinDIO_PORT                 PORTBbits.RB4


#define pinDIO_GetValue()           PORTBbits.RB4
#define pinDIO_SetDigitalInput()    do { TRISBbits.TRISB4 = 1; } while(0)
#define pinDIO_SetDigitalOutput()   do { TRISBbits.TRISB4 = 0; } while(0)


// get/set pinClk aliases
#define pinClk_TRIS                 TRISBbits.TRISB5

#define pinClk_PORT                 PORTBbits.RB5

#define pinClk_GetValue()           PORTBbits.RB5
#define pinClk_SetDigitalInput()    do { TRISBbits.TRISB5 = 1; } while(0)
#define pinClk_SetDigitalOutput()   do { TRISBbits.TRISB5 = 0; } while(0)
