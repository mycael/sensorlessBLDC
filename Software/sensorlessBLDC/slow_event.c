/**********************************************************************
* © 2005 Microchip Technology Inc.
*
* FileName:        slow_event.c
* Dependencies:    general.h, motor_isr.h, medium_event.h, hardware.h
* Processor:       dsPIC30F3010
* Compiler:        MPLAB® C30 v2.00.00 or higher
*
* SOFTWARE LICENSE AGREEMENT:
* Microchip Technology Inc. (“Microchip”) licenses this software to you
* solely for use with Microchip dsPIC® digital signal controller
* products. The software is owned by Microchip and is protected under
* applicable copyright laws.  All rights reserved.
*
* SOFTWARE IS PROVIDED “AS IS.”  MICROCHIP EXPRESSLY DISCLAIMS ANY
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
* Author        Date       Comments on this revision
*   R. Condit	 3/6/06     First revision
*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
* Notes:
*   The SlowEvent() routine is called once every 10ms 
**********************************************************************/
#include <stdio.h>
#include "general.h"
#include "Motor_isr.h"
#include "hardware.h"
#include "medium_event.h"

unsigned int VerySlowEventCounter = 0;

void SlowEvent(void)
{
    
volatile struct {
		unsigned Button_S2:1;
		unsigned Button_S3:1;

		unsigned :14;
} DebounceFlags;

	ControlFlags.SlowEventFlag = 0;

// Check button presses

	if(BUTTON_S2 && !DebounceFlags.Button_S2) 
	{
		ControlFlags.RunMotor = 0;
		DebounceFlags.Button_S2 = 1;

		RunMode = SENSORLESS_INIT;
	}

	if(BUTTON_S3)
	{
		RunMode = MOTOR_OFF;
		ControlFlags.RunMotor = 0;
		SensorlessStartState = 0;
		DebounceFlags.Button_S3 = 1;
	}

	if (DebounceFlags.Button_S2)
	{
		if (!BUTTON_S2) DebounceFlags.Button_S2 = 0;
	}


	if (DebounceFlags.Button_S3)
	{
		if (!BUTTON_S3) DebounceFlags.Button_S3 = 0;
	}
    
    /*
    if(++VerySlowEventCounter >= 50)
    {
        VerySlowEventCounter = 0;
        //sprintf(string, "Speed : %lu\r\n", Speed);
        //SendStr(string);
        sprintf(string, "PW %u\r\n", SpeedReference);
        SendStr(string);
        
    }
    */
    
	
}
