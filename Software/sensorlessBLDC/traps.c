#include "general.h"
#include "hardware.h"
#include <libpic30.h>


void __attribute__((__interrupt__)) _OscillatorFail(void);
void __attribute__((__interrupt__)) _AddressError(void);
void __attribute__((__interrupt__)) _StackError(void);
void __attribute__((__interrupt__)) _MathError(void);
void __attribute__((__interrupt__)) _AltOscillatorFail(void);
void __attribute__((__interrupt__)) _AltAddressError(void);
void __attribute__((__interrupt__)) _AltStackError(void);
void __attribute__((__interrupt__)) _AltMathError(void);

/* Primary Exception Vector handlers:
   These routines are used if INTCON2bits.ALTIVT = 0.
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application. */

void __attribute__((__interrupt__, no_auto_psv)) _OscillatorFail(void)
{
    //int i;
    //OVDCON = 0;
    INTCON1bits.OSCFAIL = 0;        //Clear the trap flag
    /*
    while (1)
    {
        LED = 0;
        for(i = 0; i < 3; i++)__delay_ms(50);
        LED = 1;
        for(i = 0; i < 3; i++)__delay_ms(50);
    }
     * */
}

void __attribute__((__interrupt__, no_auto_psv)) _AddressError(void)
{
    int i;
    OVDCON = 0;
    INTCON1bits.ADDRERR = 0;        //Clear the trap flag
    while (1)
    {
        LED = 0;
        for(i = 0; i < 3; i++)__delay_ms(50);
        LED = 1;
        for(i = 0; i < 3; i++)__delay_ms(50);
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _StackError(void)
{
    int i;
    OVDCON = 0;
    INTCON1bits.STKERR = 0;         //Clear the trap flag
    while (1)
    {
        LED = 0;
        for(i = 0; i < 3; i++)__delay_ms(50);
        LED = 1;
        for(i = 0; i < 3; i++)__delay_ms(50);
    }
}

void __attribute__((__interrupt__, no_auto_psv)) _MathError(void)
{
    int i;
    OVDCON = 0;
    INTCON1bits.MATHERR = 0;        //Clear the trap flag
    while (1)
    {
        LED = 0;
        for(i = 0; i < 3; i++)__delay_ms(50);
        LED = 1;
        for(i = 0; i < 3; i++)__delay_ms(50);
    }
}

/* Alternate Exception Vector handlers:
   These routines are used if INTCON2bits.ALTIVT = 1.
   All trap service routines in this file simply ensure that device
   continuously executes code within the trap service routine. Users
   may modify the basic framework provided here to suit to the needs
   of their application. */

void __attribute__((__interrupt__, no_auto_psv)) _AltOscillatorFail(void)
{
    OVDCON = 0;
        INTCON1bits.OSCFAIL = 0;
        while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltAddressError(void)
{
    OVDCON = 0;
        INTCON1bits.ADDRERR = 0;
        while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltStackError(void)
{
    OVDCON = 0;
        INTCON1bits.STKERR = 0;
        while (1);
}

void __attribute__((__interrupt__, no_auto_psv)) _AltMathError(void)
{
    OVDCON = 0;
        INTCON1bits.MATHERR = 0;
        while (1);
}

