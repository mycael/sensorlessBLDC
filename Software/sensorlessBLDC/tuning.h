/**********************************************************************
 *                                                                     *
 *                        Software License Agreement                   *
 *                                                                     *
 *    The software supplied herewith by Microchip Technology           *
 *    Incorporated (the "Company") for its dsPIC controller            *
 *    is intended and supplied to you, the Company's customer,         *
 *    for use solely and exclusively on Microchip dsPIC                *
 *    products. The software is owned by the Company and/or its        *
 *    supplier, and is protected under applicable copyright laws. All  *
 *    rights are reserved. Any use in violation of the foregoing       *
 *    restrictions may subject the user to criminal sanctions under    *
 *    applicable laws, as well as to civil liability for the breach of *
 *    the terms and conditions of this license.                        *
 *                                                                     *
 *    THIS SOFTWARE IS PROVIDED IN AN "AS IS" CONDITION.  NO           *
 *    WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING,    *
 *    BUT NOT LIMITED TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND    *
 *    FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE     *
 *    COMPANY SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL,  *
 *    INCIDENTAL OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.  *
 *                                                                     *
  **********************************************************************/
#include "dsp.h"

//------------------ external variables ----------------------
extern unsigned int lock1_demand;			// demand for first lock (duty cycle in %)
extern unsigned int lock2_demand;			// demand for second lock (duty cycle in %)
extern unsigned int lock1_duration;			// hold time for first lock
extern unsigned int lock2_duration;			// hold time for second lock
extern unsigned int MaxMotorSpeed;

extern unsigned int ramp_start_demand;		// demand for start of ramp (duty cycle in %)
extern unsigned int ramp_end_demand;		// demand for end of ramp (duty cycle in %)
extern unsigned int ramp_duration;			// total length (in time) or the ramp
extern unsigned int ramp_start_speed;		// ramp start speed in electrical revolutions
extern unsigned int ramp_end_speed;			// ramp end speed in electrical revolutions
extern unsigned int ramp_demand_rate;		// amount at which rate demand should be increased
extern unsigned int ramp_speed_rate;		// interval at with sectors speed should be increased  
extern unsigned long speed_constant;		// constant used to calculate RPM from TMR1 
extern unsigned int RPM_converter_constant; // constant used to convert potentiometer ADC result to RPM
extern unsigned int blanking_count_initial; // blanking count
extern unsigned int phase_advance_start;	// phase advance start speed (RPM)
extern unsigned int phase_advance_slope;	// phase advance slope in degrees per 1000RPM
extern tPID PIDStructure;					// PID data structure
extern volatile unsigned int CrossOverERPS; // The speed at with the algorithm transitions for low to high (and vice versa) speed mode
extern volatile unsigned int NumOfTimer1TimeOuts;  // Defines the number of Timer 1 timeouts that will sequentially occur before the motor shuts down
extern volatile unsigned int NoOfMotorPoles; // Number of motor poles
//------------------------ Function Declarations -------------
extern void GetParameters();



