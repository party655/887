
// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF        // Internal External Switchover bit (Internal/External Switchover mode is enabled)
#pragma config FCMEN = OFF       // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is enabled)
#pragma config LVP = OFF         // Low Voltage Programming Enable bit (RB3/PGM pin has PGM function, low voltage programming enabled)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

#include <xc.h>
#include <pic16f887.h>
#include "pin_manager_.h"
#include <stdint.h>
#include <stdbool.h>
#include "encoder.h"
#include "TM1637Display_.h"
#include "ds1302.h"
#include "biper.h"


#define _XTAL_FREQ          4000000
#define TEST_DELAY          1000
#define time                0

void initialization ()
{
    PORTB = 0;
    TRISB = 0xE;
    ANSELH = 0;
    PORTD = 0;
    TRISD = 0;
    INTCON = 0xE0;
    OPTION_REG = 0xC0;
}



   
// The amount of time (in milliseconds) between tests
unsigned char mainMenu, ChMenu, 

timeChannelVal_1, timeChannelVal_2, timeChannelVal_3, timeChannelVal_4, timeChannelVal_5, timeChannelVal_6,
timeChannelVal_7, timeChannelVal_8;

static int var;

const uint8_t DONE[] = {
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,           // d
	SEG_A | SEG_B | SEG_C | SEG_D | SEG_E | SEG_F,   // O
	SEG_C | SEG_E | SEG_G,                           // n
	SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
	};
const uint8_t CHnL[] = { 57, 118, 84, 56 };
const uint8_t dtLi[] = { 94, 120, 56, 120 };
const uint8_t dtHu[] = { 94, 120, 118, 28 };
const uint8_t Auto[] = { 119, 28, 120, 92 };
const uint8_t SEG_16[] = { 134, 88, 66, 91 };
const uint8_t SEG_17[] = { 99, 44, 27, 89 };
const uint8_t SEG_0[] = { 0, 0, 0, 0 };


typedef struct
{
    char keyFlag:1;
    char rightFlag:1;
    char leftFlag:1;
}flags; flags trig; 

void biper (int time_high, int time_low, int quantity)
{
    for(int i=0; i<quantity; i++) 
    {
    
    for(int i=0; i<time_high; i++) 
    {
    bip = 1;
    __delay_ms(1);
    }
    
    for(int i=0; i<time_low; i++) 
    {
    bip = 0;
    __delay_ms(1);
    }
    }
}

void detectKey()
{ 
    while(key) 
            {   __delay_ms(10);
                trig.keyFlag ^= 1;
                biper(20,1,1);
                while(key);
                break;
            } 
}

unsigned char changeVar (unsigned char var)
{
//    unsigned char var = 0;   
    
    while(s2_right)
        {                
            while(s1_left)
            {
            var++;
            biper(1,1,1);
            while(s1_left||s2_right);            
            }            
        }
               
        while(s1_left)
        {           
            while(s2_right&&var!=0)
            {          
            var--; 
            biper(1,1,1);
            while(s1_left||s2_right);
            }             
        }

//        while(key&&var!=0) 
//        {
//            var = 0;
////            showNumberDec(var,false,4,0);
//            biper(50,30,2);
//            while(key);
//        }
    return var;
    }


void __interrupt() myIsr()
{
    if (INTCONbits.T0IF)
    {
        INTCONbits.T0IF = 0;
        di();
        
        detectKey();
        
        while(!trig.keyFlag)
        { 
            mainMenu = changeVar(mainMenu);
            if(mainMenu>5)mainMenu=1;
        
        switch(mainMenu)
            {
            case 0: setSegments(SEG_0,4,0);
                    break;
            case 1: setSegments(CHnL,4,0);
                    break;           
            case 2: setSegments(dtLi,4,0);
                    break;
            case 3: setSegments(dtHu,4,0);
                    break;
            case 4: setSegments(Auto,4,0);
                    break;           
            case 5: setSegments(DONE,4,0);
                    break;         
            } break;
        
        }
        
        while(trig.keyFlag&&(mainMenu==1))
        { 
            ChMenu = changeVar(ChMenu);
            if(ChMenu>7)ChMenu=0;
        
        switch(ChMenu)
            {
            case 0: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);                 
                    break;
            case 1: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;           
            case 2: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;
            case 3: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;
            case 4: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;           
            case 5: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;        
            case 6: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;        
            case 7: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;        
            case 8: setSegments(CHnL,2,0);
                    showNumberDec(ChMenu+1,false,2,2);
                    break;        
            } break;
        
        }
        
        while((!key)&&trig.keyFlag)
        {
        switch(ChMenu)
            {
            case 0: timeChannelVal_1 = changeVar(timeChannelVal_1);
                    if(timeChannelVal_1>60)timeChannelVal_1=0;
                    showNumberDec(timeChannelVal_1,false,2,2);                 
                    break;
            case 1: timeChannelVal_2 = changeVar(timeChannelVal_2);
                    if(timeChannelVal_2>60)timeChannelVal_2=0;
                    showNumberDec(timeChannelVal_1,false,2,2);
                    break;           
            case 2: timeChannelVal_3 = changeVar(timeChannelVal_3);
                    if(timeChannelVal_3>60)timeChannelVal_3=0;
                    showNumberDec(timeChannelVal_1,false,2,2);
                    break;
            case 3: timeChannelVal_4 = changeVar(timeChannelVal_4);
                    if(timeChannelVal_4>60)timeChannelVal_4=0;
                    showNumberDec(timeChannelVal_1,false,2,2);
                    break;
            case 4: timeChannelVal_5 = changeVar(timeChannelVal_5);
                    if(timeChannelVal_5>60)timeChannelVal_5=0;
                    showNumberDec(timeChannelVal_1,false,2,2);
                    break;           
            case 5: timeChannelVal_6 = changeVar(timeChannelVal_6);
                    if(timeChannelVal_6>60)timeChannelVal_6=0;
                    showNumberDec(timeChannelVal_6,false,2,2);
                    break;        
            case 6: timeChannelVal_7 = changeVar(timeChannelVal_7);
                    if(timeChannelVal_7>60)timeChannelVal_7=0;
                    showNumberDec(timeChannelVal_7,false,2,2);
                    break;        
            case 7: timeChannelVal_8 = changeVar(timeChannelVal_8);
                    if(timeChannelVal_8>60)timeChannelVal_8=0;
                    showNumberDec(timeChannelVal_1,false,2,2);
                    break;        
            } 
        
        
        }
        }
    
    
    }
        








void main(void)
{
    
     __delay_ms(1000);
    initialization();
    di();
    TM1637Display();
    setBrightness(0x0f, true);
    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
    
  
   
    while (1)
    {
        
       
        ei();
        __delay_ms(6000);
        
       
        
    }
}
   


//            if(key) 
//            {   __delay_ms(10);
//                trig.keyFlag = 1;
//            } else trig.keyFlag = 0;
//                        
//                        while(trig.keyFlag)
//                        {
//                            ChMenu = 1;
//                            ChMenu = changeVar(ChMenu);
//                            if(ChMenu>12) ChMenu = 12;
//                            switch(ChMenu)
//                            {
//                                case 1: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 2: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 3: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 4: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 5: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 6: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 7: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 8: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 9: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 10: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 11: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                                case 12: setSegments(CHnL,2,0);
//                                        showNumberDec(ChMenu,false,2,0);
//                                        if(key){__delay_ms(10); trig.keyFlag=0;}
//                            }
//                        }
    
    
    
    
//    int k;
//    uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
//    uint8_t Fuck[] = { 113, 62, 57, 114 };
//    uint8_t You[] = { 0, 110, 63, 62, };
//    uint8_t ____[] = { 8, 8, 8, 8, };
//    
//    while(1)
//    {
//        
//        setBrightness(7,true);
//        setSegments(Fuck,4,0);
//         __delay_ms(TEST_DELAY);
//        setBrightness(7, false);
//        setSegments(Fuck,4,0);
//     
//        
//        for(k = 0; k < 4; k++)
//        {
//            setBrightness(7,true);
//        setSegments(____,0,k);
//         __delay_ms(300);
//        }
//        
//        setBrightness(7,true);
//        setSegments(You,4,0);
//         __delay_ms(TEST_DELAY);
//        setBrightness(7, false);
//        setSegments(You,4,0);
//        __delay_ms(TEST_DELAY);
//
//    }
//    
//    while(0)
//    {
//        for(k = 0; k < 4; k++) {
//       setBrightness(7, false);  // Turn off
//       setSegments(Fuck,4,0);
//       __delay_ms(TEST_DELAY);
//       setBrightness(7, true); // Turn on
//       setSegments(You,4,0);
//       __delay_ms(TEST_DELAY);  
//     }
//    }
//     // All segments on
//     setSegments(data,4,0); // 8888
//     __delay_ms(TEST_DELAY);
//     
//     // Selectively set different digits
//     data[0] = 0b01001001;
//     data[1] = encodeDigit(1);
//     data[2] = encodeDigit(2);
//     data[3] = encodeDigit(3);
//
//     for(k = 3; k >= 0; k--) {
//       setSegments(data, 1, k); // loop dashes and nums one digit at time
//       __delay_ms(TEST_DELAY); 
//       }
//
//     setSegments(data+2, 2, 2); // dash dash 2 3
//     __delay_ms(TEST_DELAY);
//
//     setSegments(data+2, 2, 1); // dash 2 2 3
//     __delay_ms(TEST_DELAY);
//
//     setSegments(data+1, 3, 1); //  dash 1 2 3 
//     __delay_ms(TEST_DELAY);
//     
//     
//     
//     // Show decimal numbers with/without leading zeros
//     // 0 7  42 217 1092 5467 0000 0007 0042 0217 1092 5467 
//     bool lz = false;
//     for (uint8_t z = 0; z < 2; z++) {
//       for(k = 0; k < 10000; k += k*4 + 7) {
//           showNumberDec(k, lz, 4, 0);
//           __delay_ms(TEST_DELAY);
//       }
//       lz = true;
//     }
//     
//    
//     
//      // turn on central dots
//     for(k = 0; k < 4; k++)
//       data[k] = 0;
//     setSegments(data,4,0); //0000
//       for(k=0; k <= 4; k++) {
//           showNumberDecEx(0, (0x80 >> k), true,4,0);
//           __delay_ms(TEST_DELAY);
//       }
//     
//  
//      // Show decimal number whose length is smaller than 4
//     showNumberDec(153, false, 3, 1); // 0153
//     __delay_ms(TEST_DELAY);
//     showNumberDec(22, false, 2, 2);  // 0122
//     __delay_ms(TEST_DELAY);
//     showNumberDec(0, true, 1, 3);    // 0120
//     __delay_ms(TEST_DELAY);
//     showNumberDec(0, true, 1, 2);   // 0120
//     __delay_ms(TEST_DELAY);
//     showNumberDec(0, true, 1, 1);  // 0100
//     __delay_ms(TEST_DELAY);
//     showNumberDec(0, true, 1, 0);  // 0000
//     __delay_ms(TEST_DELAY);
//     
//     // Brightness Test
//     for(k = 0; k < 4; k++)
//       data[k] = 0xff;
//     
//     for(k = 0; k < 7; k++) {
//       setBrightness(k, true);
//       setSegments(data,4,0); // 8888
//       __delay_ms(TEST_DELAY);
//     }
//      
//     // On/Off test
//     for(k = 0; k < 4; k++) {
//       setBrightness(7, false);  // Turn off
//       setSegments(data,4,0);
//       __delay_ms(TEST_DELAY);
//       setBrightness(7, true); // Turn on
//       setSegments(data,4,0);
//       __delay_ms(TEST_DELAY);  
//     }
//      
//     // Show Done!
//     setSegments(SEG_DONE,4,0);
//
//     while(1);
//       
//    