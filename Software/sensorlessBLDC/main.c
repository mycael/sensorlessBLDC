/**********************************************************************
* Project Name :   sensorlessBLDC
* FileName:        main.c
* Dependencies:    general.h
*                  init_PWM.h
*                  init_ADC.h
*                  IIR_Filter.h
*                  BEMF_filter.h
*                  Motor_isr.h
*                  slow_event.h
*                  medium_event.h
*                  dsp.h
* Processor:       dsPIC30F3011
* Compiler:        MPLAB® XC16
*
* SOFTWARE LICENSE AGREEMENT:
* This software is based on a Microchip software. The Microchip Licence
* is applicable. As this software is distributed in a non commercial use
* it can bee freely modify or redistributed (see section :
* About Us » Legal Information » Copyright Usage Guidelines, on the
* Microchip website).
* 
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  
* MICROCHIP , PAST AND CURRENT AUTHORS EXPRESSLY DISCLAIM ANY
* WARRANTY OF ANY KIND, WHETHER EXPRESS OR IMPLIED, INCLUDING BUT NOT
* LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE, OR NON-INFRINGEMENT. IN NO EVENT SHALL MICROCHIP
* BE LIABLE FOR ANY INCIDENTAL, SPECIAL, INDIRECT OR CONSEQUENTIAL
* DAMAGES, LOST PROFITS OR LOST DATA, HARM TO YOUR EQUIPMENT, COST OF
* PROCUREMENT OF SUBSTITUTE GOODS, TECHNOLOGY OR SERVICES, ANY CLAIMS
* BY THIRD PARTIES (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF),
* ANY CLAIMS FOR INDEMNITY OR CONTRIBUTION, OR OTHER SIMILAR COSTS.
*
* REVISION HISTORY:
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Author            Date      Comments on this revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Reston Condit      Jan 7, 2007   Intial version
* Michael Guarisco   Nov, 2015     Modified version for dsPIC30F3011 and XC16
* Michael Guarisco   Dec 1, 2015       
**********************************************************************/

#include "general.h"
#include "hardware.h"
#include <libpic30.h>
#include "init_PWM.h"
#include "init_ADC.h"
#include "IIR_Filter.h"
#include "BEMF_filter.h"
#include "Motor_isr.h"
//#include "slow_event.h"
#include "medium_event.h"
#include "dsp.h"
#include "init_IC.h"



// Setup Configuration bits
// FOSC
#pragma config FOSFPR = XT_PLL16        // Oscillator (XT w/PLL 16x)
#pragma config FCKSMEN = CSW_FSCM_OFF   // Clock Switching and Monitor (Sw Disabled, Mon Disabled)

// FWDT
#pragma config FWPSB = WDTPSB_16        // WDT Prescaler B (1:16)
#pragma config FWPSA = WDTPSA_512       // WDT Prescaler A (1:512)
#pragma config WDT = WDT_OFF            // Watchdog Timer (Disabled)

// FBORPOR
#pragma config FPWRT = PWRT_64          // POR Timer Value (64ms)
#pragma config BODENV = BORV20          // Brown Out Voltage (4.5V)
#pragma config BOREN = PBOR_ON          // PBOR Enable (Enabled)
#pragma config LPOL = PWMxL_ACT_HI      // Low-side PWM Output Polarity (Active High)
#pragma config HPOL = PWMxH_ACT_HI      // High-side PWM Output Polarity (Active High)
#pragma config PWMPIN = RST_IOPIN       // PWM Output Pin Reset (Control with PORT/TRIS regs)
#pragma config MCLRE = MCLR_DIS          // Master Clear Enable (Enabled)

// FGS
#pragma config GWRP = GWRP_OFF          // General Code Segment Write Protect (Disabled)
#pragma config GCP = CODE_PROT_OFF      // General Segment Code Protection (Disabled)

// FICD
#pragma config ICS = ICS_PGD            // Comm Channel Select (Use PGC/EMUC and PGD/EMUD)



int main ( void )
{
    //char string[20];
    int i, j;
	ControlFlags.EnablePotentiometer = 1;   // enable speed/demand potentiometer VR1
	#ifdef PID_SPEED_LOOP
		ControlFlags.SpeedControlEnable = 1; // enable speed control loop
	#endif

    TRISD = 0x0001; // RD0 input for input capture
    TRISC = 0x0000;
  	LATC = 0;
  	LATD = 0;
  	TRISF = 0xFFFF;  
    RCON &= 0b0011111100100000;
    for(j=0; j<5; j++)
    {
        LED = 0;
        for(i = 0; i < 3; i++)__delay_ms(50);
        LED = 1;
        for(i = 0; i < 3; i++)__delay_ms(50);
    }
    LED = 0;
    
 	// Setup Timer1 for 1:8 prescale.  Timer1 is used to measure
 	// the period between zero crossings.  Enable Timer 1.
 	T1CON = 0x8010;
 	
 	// Timer4 and Timer 5 will be the commutation timers.  They
	//  have the same prescaler (1:8) as Timer 1.  Turn on Timer 4
	//  but not Timer 5  
 	T4CON = 0x0010;			// 1:8 prescaler
	TMR4 = 0;				// clear Timer 4
	IFS1bits.T4IF = 0;		// Disable the Timer 2 interrupt
	IEC1bits.T4IE = 0;
	T4CONbits.TON = 1; 		// Turn on Timer 2

	T5CON = 0x0010;			// 1:8 prescaler
	TMR5 = 0;				// clear Timer 3
	IFS1bits.T5IF = 0;		// Disable the Timer 3 interrupt
	IEC1bits.T5IE = 0;


 	Init_PWM();				// initialize the PWM module
 	Init_ADC();				// initialize the ADC module
    Init_IC();
 	 	 	 	
	// Call the IIRTransposeFilterInit routine to zero out the state variables for each
	//  of the three phases
	IIRTransposeFilterInit( &BEMF_phaseA_Filter );
	IIRTransposeFilterInit( &BEMF_phaseB_Filter );
	IIRTransposeFilterInit( &BEMF_phaseC_Filter );

	// initialize the motor state variable to turn the motor off
	RunMode = MOTOR_OFF;
    
 	while(1)
 	{
		if(ControlFlags.MediumEventFlag)
			MediumEvent();				// execute start-up ramp or speed control loop
		//if(ControlFlags.SlowEventFlag)
		//	SlowEvent();				// check buttons and update LCD
	}
}



