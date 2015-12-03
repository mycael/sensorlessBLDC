#include "general.h"


void Init_IC(void)
{
    IC1CONbits.ICTMR = 1;   // Timer2
    IC1CONbits.ICI = 0;     // Interrupt all capture event
    
    IFS0bits.IC1IF = 0;     // Reset interrupt flag
    IEC0bits.IC1IE = 1;     // Enable IC1 interrupt
    
    IC1CONbits.ICM = 0b001;     // Interrupt on rising and falling edge
    PR2 = 0xFFFF;
    T2CON = 0;
    T2CONbits.TCKPS = 0b01;     // 1:8 prescaler
    T2CONbits.TON = 1;         // Enable timer2
}