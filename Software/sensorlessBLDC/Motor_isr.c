#include "general.h"
#include "hardware.h"
#include "IIR_Filter.h"
#include "BEMF_filter.h"
#include "Hall_States.h"
#include "tuning.h"
#include "Motor_isr.h"
#include "snapshot.h"


// ----------------------Function prototypes ---------------------------
void CheckZeroCrossing(void);
unsigned int ThirtyDegreeTimeAverage(void);

// ----------------- Global Variable Declarations ----------------------
unsigned int pot;		    // stores the potentiometer value
unsigned int HalltoSector[8] = {-1,2,4,3,0,1,5,-1};
unsigned int SectortoState[6] = {STATE0,STATE1,STATE2,STATE3,STATE4,STATE5};
unsigned int HallState;		// Hall effect sensor state
int Sector = 0;				// Sector (0 - 5)
int OldSector;				// Stores old sector

int vpha, vphb, vphc, vbus;  // stores the ADC result for phase A, B, and C and the bus voltage measurement

unsigned int Timer1TimeoutCntr = 0;
unsigned int BlankingCount = 3;
unsigned int phase_advance = 0;
unsigned int SpeedPtr;
unsigned int RunMode = 0;
unsigned int SensorlessStartState = 0;
unsigned int MediumEventCounter = 0;
unsigned int SlowEventCounter = 0;
long accumulator_c;
int signal_average;
unsigned long Speed = 0;
int vpha_filtered_sample;
int vphb_filtered_sample;
int vphc_filtered_sample;
unsigned int NextSectorState;
unsigned int SixtyDegreeTime[6];
unsigned int OneEightyDegreeTime[16];

volatile struct ControlFlags ControlFlags;

unsigned int pos_ptr = 0;

unsigned int SpeedReference = 0;
unsigned int MotorOffCounter = 0;
unsigned int MotorOnCounter = 0;
unsigned long AccConsumption = 0;



/*---------------------------------------------------------------------
  Function Name: ADCInterrupt
  Description:   ADC Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void __attribute__((__interrupt__, no_auto_psv)) _ADCInterrupt( void )   // occurs at a rate of 81.920 kHz
{
	int i;
	/* reset ADC interrupt flag */
	IFS0bits.ADIF = 0; 

	// Read the potentiometer and phase voltages.
	//pot = (pot + POTBUF) >> 1;

	if (BlankingCount)				// if the blanking count hasn't expired, feed the Back EMF
	{								// filters the last filtered Back EMF sample (rather than the unfiltered sample.)
		BlankingCount--;
		vpha = vpha_filtered_sample;
		vphb = vphb_filtered_sample;
		vphc = vphc_filtered_sample;

//		vpha = (3*vpha_filtered_sample + VPHABUF)>>2;  // This three lines can be used an a alternative to the 
//		vphb = (3*vphb_filtered_sample + VPHBBUF)>>2;  //  previous three lines
//		vphc = (3*vphc_filtered_sample + VPHCBUF)>>2;
	}
	else
	{
		vpha = VPHABUF;
		vphb = VPHBBUF;
		vphc = VPHCBUF;
	}

	// Get the bus voltage and do a little averaging
	vbus =  (vbus + VBUSBUF) >> 1;
    
    // Get current sensor value
    //AccConsumption += VCURRENTBUF;

	switch (RunMode)
	{
		case MOTOR_OFF:
			OVDCON = 0;
			break;
		case HALL_SENSOR_MODE:
			// Read the hall sensors.  This is only done If running sensored.
			// Read Hall sensors to get position.
			do
			{
				HallState = (PORTD >> 8) & 0x0007;
				Sector = HalltoSector[HallState];
				i++;
			} while((Sector == -1) && (i<4));
	
			if(Sector != OldSector)
			{
				Commutate(Sector);
				TMR4 = 0;
				IFS1bits.T4IF = 0;
			}
		case SENSORLESS_START:
		case SENSORLESS_RUNNING	:
			CheckZeroCrossing();
			break;
		default:
			if (RunMode > (NO_OF_RUN_MODES - 1))  RunMode = 0;
			break;
	}
}

/*---------------------------------------------------------------------
  Function Name: T1Interrupt
  Description:   T1 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void __attribute__((__interrupt__, no_auto_psv)) _T1Interrupt( void ) 
{
	IFS0bits.T1IF = 0;
	if (Timer1TimeoutCntr++ >= NumOfTimer1TimeOuts)  // When Timer 1 overflows the algorithm is lost
	{
		IEC0bits.T1IE = 0;		// turn off Timer 1 and stop the motor
		RunMode = MOTOR_OFF;
	}
}

/*---------------------------------------------------------------------
  Function Name: T4Interrupt
  Description:   T4 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void __attribute__((__interrupt__, no_auto_psv)) _T4Interrupt( void )  // TMR 2 is never turned on in Hall Mode
{
	TMR4 = 0;			   			// clear TMR4
	IFS1bits.T4IF = 0;

	if (RunMode == SENSORLESS_RUNNING)	
	{
		Sector++;						// Increment Sector (there are 6 total)
		if(Sector > 5) Sector = 0;
		Commutate(Sector);				// Change the PWM output for next sector
	}
}

/*---------------------------------------------------------------------
  Function Name: T5Interrupt
  Description:   T5 Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/
void __attribute__((__interrupt__, no_auto_psv)) _T5Interrupt( void )  // TMR 3 is never turned on in Hall Mode
{
	T5CONbits.TON = 0;     			// turn off TMR5
	TMR5 = 0;
	TMR4 = 0;		
	IFS1bits.T4IF = 0;	   		
	IFS1bits.T5IF = 0;
	IEC1bits.T5IE = 0;
	Timer1TimeoutCntr = 0;			// A commutation has occured so the Timer 1 timeout counter can be reset

	if (RunMode == SENSORLESS_RUNNING)	
	{
		Sector = NextSectorState;
		Commutate(Sector);				// Force commutation
	}
}

/*---------------------------------------------------------------------
  Function Name: PWMInterrupt
  Description:   PWM Interrupt Handler
  Inputs:        None
  Returns:       None
-----------------------------------------------------------------------*/

void __attribute__((__interrupt__, no_auto_psv)) _PWMInterrupt( void )  // Occurs every 50us or at a rate of 20kHz
{
	IFS2bits.PWMIF = 0;             // Clear the PWM interrupt flag
    /*
	if (++SlowEventCounter >= 200)	// Fire Slow event every 10ms
	{
		SlowEventCounter = 0;
		ControlFlags.SlowEventFlag = 1;
	}
     * */
	
	if (++MediumEventCounter >= 20)  // Fire Medium event every 1ms
	{
		MediumEventCounter = 0;
		ControlFlags.MediumEventFlag = 1;
	}
	if (RunMode == SENSORLESS_START)
	{
		MediumEventCounter += 9;  // Fire Medium event every 100us in SENSORLESS_START mode
	}

}

/*---------------------------------------------------------------------
  Function Name: CheckZeroCrossing
  Description:   This function filters ADC samples of each motor phase
				  and looks at the filtered phase signals in order to 
				  detect a zero cross event
  Inputs:        None (global variables including vpha, vphb, vphc, and vbus)
  Returns:       None
-----------------------------------------------------------------------*/
void CheckZeroCrossing(void)
{
	static unsigned int ZeroCrossState = 0;
	unsigned int ThirtyDegreeTime;
	unsigned int ThreeSixtyDegreeTime;
	unsigned int phase_delay;
	static int vbus_offset = 0;
	
	
	if (ZeroCrossState < 6)  // If Low Speed Mode
	{
		BEMF_phaseA_Filter.pCoefs = &BEMF_filterCoefs_49152Hz;	
		BlockIIRTransposeFilter( &BEMF_phaseB_Filter, &vphb, &vphb_filtered_sample, 1 );
		BlockIIRTransposeFilter( &BEMF_phaseC_Filter, &vphc, &vphc_filtered_sample, 1 );
	}
	else
		BEMF_phaseA_Filter.pCoefs = &BEMF_filterCoefs_81940Hz;
	BlockIIRTransposeFilter( &BEMF_phaseA_Filter, &vpha, &vpha_filtered_sample, 1 );  // Get a filtered sample

	// finds the center voltage of the phase signal even under different loads
	signal_average = vbus/2 + vbus_offset;
	accumulator_c += vpha_filtered_sample - signal_average;
	vbus_offset = accumulator_c >> 13;

	#ifdef SNAPSHOT
		if (ControlFlags.TakeSnapshot)     // The TakeSnapshot control flag is set by pressing S6
		{
			SnapBuf1[pos_ptr] = ADCBUF2; // phase A Raw data	
			SnapBuf2[pos_ptr] = vpha;	 // phase A pre-filtered data (includes effect of blanking count)
			SnapBuf3[pos_ptr] = vpha_filtered_sample; // phase a filtered sample
			SnapBuf4[pos_ptr] = signal_average;
		pos_ptr++;
			if(pos_ptr > (SNAPSIZE-1))
			{
				pos_ptr = 0;
				ControlFlags.TakeSnapshot = 0;  // Clear the flag to prevent creating a circular buffer
			}
		}
	#endif

	if (ZeroCrossState < 6)
		phase_delay = FILTER_PHASE_DELAY + PROCESSING_DELAY + phase_advance;
	else
		phase_delay = FILTER_PHASE_DELAY + PROCESSING_DELAY_HS + phase_advance;

	switch(ZeroCrossState)
	{
// States 0 - 5 implement the low speed mode of this algorithm.  All three phase voltages are sampled. The sampling frequency
// is 49kHz when running in high speed mode. 
		case 0:
			if (vpha_filtered_sample < signal_average)   // signal is falling look for when it falls below center voltage
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();			
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 3;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState++;
			}
			break;
		case 1:
			if (vphc_filtered_sample > signal_average)
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 4;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState++;
			}		
			break;
		case 2:
			if (vphb_filtered_sample < signal_average)
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 5;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState++;
			}		
			break;
		case 3:
			if (vpha_filtered_sample > signal_average)
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 0;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState++;
				// do the change over from LowSpeedMode to HighSpeed Mode
				if (ControlFlags.HighSpeedMode)
				{
					ZeroCrossState = 6;
					TMR4 = 0;
					PR4 = ThirtyDegreeTime*2;
					IFS1bits.T4IF = 0;	  
					IEC1bits.T4IE = 1;
					ControlFlags.TakeSnapshot = 1;  // take shapshot on crossover
					ADCON1bits.ADON = 0;		// Turn ADC module off before modifying control bits;
					ADCSSL = ADCSSL_HIGH_SPEED;  // only read the pot, vbus and vpha
					ADCON2 = ADCON2_HIGH_SPEED;  // interrupt after three adc reads (adc interrupt frequency changes to 81.94 kHz)
					ADCON1bits.ADON = 1;		//Turn the ADC module back on 
				}
			}		
			break;
		case 4:
			if (vphc_filtered_sample < signal_average)
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 1;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState++;
			}		
			break;
		case 5:
			if (vphb_filtered_sample > signal_average)
			{
				SixtyDegreeTime[ZeroCrossState] = TMR1;
				TMR1 = 0;
				ThirtyDegreeTime = ThirtyDegreeTimeAverage();
				if (ThirtyDegreeTime < phase_delay)
					ThirtyDegreeTime = phase_delay;
				PR5 = ThirtyDegreeTime - phase_delay;
				NextSectorState = 2;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				OneEightyDegreeTime[SpeedPtr&0x000F] = ThirtyDegreeTime*6;
				SpeedPtr++;
				ZeroCrossState = 0;
			}		
			break;
// States 6 - 9 implement the high speed mode of this algorithm.  Only one phase voltage is sampled. The sampling frequency
// is 81kHz when running in high speed mode.  
		case 6:
			// Wait in this state until it's safe to check for the next zero cross event
//			if (vpha_filtered_sample > (signal_average + 15))   
//				ZeroCrossState++;
			if (Sector == 2)
				ZeroCrossState++;
			break;
		case 7:
			if (vpha_filtered_sample < signal_average)   // signal is falling look for when it falls below center voltage
			{
				OneEightyDegreeTime[SpeedPtr&0x000F] = TMR1;
				TMR1 = 0;
				ThreeSixtyDegreeTime = OneEightyDegreeTime[SpeedPtr&0x000F] + OneEightyDegreeTime[(SpeedPtr-1)&0x000F];
				PR4 = ThreeSixtyDegreeTime/6;
				if ((ThreeSixtyDegreeTime>>2) < phase_delay)
					PR4 = 0;
				else
					PR4 = (ThreeSixtyDegreeTime>>2) - phase_delay;
				SpeedPtr++;
				NextSectorState = 4;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				ZeroCrossState++;
			}
			break;	
		case 8:
			// Wait in this state until it's safe to check for the next zero cross event 
//			if (vpha_filtered_sample < (signal_average - 15))
//				ZeroCrossState++;
			if (Sector == 5)
				ZeroCrossState++;
			break;		
		case 9:
			if (vpha_filtered_sample > signal_average)   // signal is falling look for when it falls below center voltage
			{
				OneEightyDegreeTime[SpeedPtr&0x000F] = TMR1;
				TMR1 = 0;
				ThreeSixtyDegreeTime = OneEightyDegreeTime[SpeedPtr&0x000F] + OneEightyDegreeTime[(SpeedPtr-1)&0x000F];
				PR4 = ThreeSixtyDegreeTime/6;
				if ((ThreeSixtyDegreeTime>>2) < phase_delay)
					PR5 = 0;
				else
					PR5 = (ThreeSixtyDegreeTime>>2) - phase_delay;
				SpeedPtr++;
				NextSectorState = 1;
				T5CONbits.TON = 1; 	
				IEC1bits.T5IE = 1;
				ZeroCrossState = 6;
				if (!ControlFlags.HighSpeedMode)
				{
					ZeroCrossState = 4;             // Go back to the appropriate state in low-speed mode
					NextSectorState = 0;            // The next timer 3 interrupt will initiate a commutation to Sector 0
					PR5 = (PR4>>1) -  phase_delay;  // load PR4 with 30 degree time minus the phase delay
					IEC1bits.T4IE = 0;
					ADCON1bits.ADON = 0;		// Turn ADC module off before modifying control bits;
					ADCSSL = ADCSSL_LOW_SPEED;  // read vphb and vphc in addition to the pot, vbus, and vpha
					ADCON2 = ADCON2_LOW_SPEED;  // interrupt every five samples (adc interrupts every 49kHz)
					ADCON1bits.ADON = 1;		// Turn ADC module back on
				}
			}
			break;
		default:
			ZeroCrossState = 0;
			break;
	}
}


/*---------------------------------------------------------------------
  Function Name: Commutate
  Description:   Commutates the motor to the next drive state
  Inputs:        sector
  Returns:       None
-----------------------------------------------------------------------*/
void Commutate(unsigned int sector)
{
	OVDCON = SectortoState[sector];			// Change PWM phase 
	OldSector = sector;						// Keep track of last sector (for error checking elsewhere)
	BlankingCount = blanking_count_initial;
}

/*---------------------------------------------------------------------
  Function Name: ThirtyDegreeTimeAverage()
  Description:   This functions takes the stored sixty degree times for an 
                  entire electrical revolution and commputs the average 30
	              degree time
  Inputs:        SixtyDegreeTime[6] (sixty degree times for an entire electrical revolution
  Returns:       The average 30 degree time
-----------------------------------------------------------------------*/
unsigned int ThirtyDegreeTimeAverage()
{
	unsigned int i;
	unsigned long temp; 
	temp = 0;
	for (i=0; i<6; i++)
	{
		temp += SixtyDegreeTime[i];
	}
	i = __builtin_divud(temp,12);		// same as divide by 6 and then divide by 2 (returning the average 30 degree time
	return i;
}


void __attribute__((__interrupt__, no_auto_psv)) _IC1Interrupt( void )
{
    IFS0bits.IC1IF = 0;     // Reset interrupt flag
    TMR2 = 0;
    if (PORTDbits.RD0)
    {   /**********************************************************************/
        /*                 _______________                         _________ _*/
        /*                |               |                       |           */
        /* _______________|               |_______________________|           */
        /*                 <-------------------------------------->           */
        /*                         Constant Period of 20ms                    */
        /*                 <------------->                                    */
        /*                       PW                                           */
        /* PW (SpeedReference) is between 0 and 15ms                          */
        /* Then, when IC1BUF get the timer 2 value at each interrupt, value   */
        /* is comprised between 0 and 55296 with timer2 prescaler at 1:8 and  */
        /* Fcy at 29491200 Hz                                                 */
        /**********************************************************************/
        
        //SpeedReference = (((long)IC1BUF)*((long)MaxMotorSpeed))/((long)55296); 
        //SpeedReference = (((long)IC1BUF)*((long)MaxMotorSpeed))/((long)27648); 
        SpeedReference = (long)(((float)IC1BUF) * SPEED_REFERENCE_SCALE_FACTOR);
        if (SpeedReference < (START_MOTOR_VALUE-20))
        {
            //SpeedReference = (START_MOTOR_VALUE-20);
            if((++MotorOffCounter >= 10) && ((RunMode == SENSORLESS_RUNNING) || (RunMode == SENSORLESS_START) || (RunMode == SENSORLESS_INIT))) //if((++MotorOffCounter == 1000) && ((RunMode == SENSORLESS_INIT) || (RunMode == SENSORLESS_START) || (RunMode == SENSORLESS_RUNNING)))
            {
                RunMode = MOTOR_OFF;
                ControlFlags.RunMotor = 0;
                SensorlessStartState = 0;
                LED = 0;
            }
            MotorOnCounter = 0;
        }       
        else if ((SpeedReference >= (START_MOTOR_VALUE-20))&&(SpeedReference < (START_MOTOR_VALUE+20)))
        {
            if((++MotorOnCounter >= 10) && (RunMode == MOTOR_OFF))
            {
                LED = 1;
                RunMode = SENSORLESS_INIT;  
            }
            MotorOffCounter = 0;
        }
        else if (SpeedReference > MaxMotorSpeed) SpeedReference = MaxMotorSpeed;
        else
        {
            MotorOffCounter = 0;
            MotorOnCounter = 0;
        }
    }
    
}