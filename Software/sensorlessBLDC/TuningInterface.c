#include "dsp.h"
#include "general.h"
#include "hardware.h"
#include "Motor_isr.h"

unsigned int lock1_demand;			// demand for first lock (duty cycle in %)
unsigned int lock2_demand;			// demand for second lock (duty cycle in %)
unsigned int lock1_duration;		// hold time for first lock
unsigned int lock2_duration;		// hold time for second lock
unsigned int ramp_start_demand;		// demand for start of ramp (duty cycle in %)
unsigned int ramp_end_demand;		// demand for end of ramp (duty cycle in %)
unsigned int ramp_duration;			// time of the ramp
unsigned int ramp_start_speed;		// ramp start speed in electrical revolutions
unsigned int ramp_end_speed;		// ramp end speed in electrical revolutions
unsigned int ramp_demand_rate;				// intervals at which demand should be decreased
unsigned int ramp_speed_rate;		// interval at with sectors speed should be increased 
unsigned long speed_constant;		// constant used to calculate RPM from TMR1 
unsigned int RPM_converter_constant; // constant used to calculater desired RPM from pot reading
unsigned int blanking_count_initial; // blanking count
unsigned int phase_advance_start;	// phase advance start speed (RPM)
unsigned int phase_advance_slope;	// phase advance slope in degrees per 1000RPM
unsigned int speed_max;				// speed trip in RPM

fractional PIDGainCoefficients[3];	// Control gains for the speed control loop
fractional abcCoefficients[3] __attribute__ ((space(xmemory)));;
fractional controlHistory[3] __attribute__ ((space(ymemory)));;
tPID PIDStructure;					// PID Structure					

struct parameter_limits 
{
    	unsigned int min;	//minimum allowed value
    	unsigned int max;	//maximum allowed value
    };

// Hard code motor parameters here 
volatile unsigned int Lock1Demand = 22;			// % demand for first lock position
volatile unsigned int Lock2Demand = 22;			// % demand for second lock position
volatile unsigned int Lock1Duration = 400;		// Lock duration in 1ms increments
volatile unsigned int Lock2Duration = 400;		// Lock duration in 1ms increments
volatile unsigned int RampStartDemand = 47;		// % demand at the beginning of the open-loop starting ramp
volatile unsigned int RampEndDemand = 52;		// % demand at the end of the open-loop starting ramp
volatile unsigned int RampDuration = 1000;		// Ramp duration in 1ms increments
volatile unsigned int RampStartSpeed = 150;		// Speed (in RPM) at the beginning of the open-loop starting ramp
volatile unsigned int RampEndSpeed = 2100;		// Speed (in RPM) at the end of the open-loop starting ramp
volatile unsigned int MaxMotorSpeed = 8000;     // Maximum speed of the motor - used to normalize the potentiometer speed adjustment
volatile unsigned int BlankingCountInit = 4;	// The number of ADC samples "ignored" after commutating the motor windings
volatile unsigned int SpeedControl_P = 8000;	// The P term for the PI speed control loop
volatile unsigned int SpeedControl_I = 1800;		// The I term for the PI speed control loop
volatile unsigned int SpeedControl_D = 11000;	// The I term for the PI speed control loop
volatile unsigned int PhaseAdvanceStart = 10000;	// The RPM at which point phase advance begins to be added
volatile unsigned int PhaseAdvanceSlope = 0; 	// The number of degrees of phase advance per 1000 RPM (after the phase advance start speed is reached) 
volatile unsigned int InitialDemand = 52;		// % demand immediately after the starting ramp is completed.
volatile unsigned int NoOfMotorPoles = 14;		// Number of motor poles (not pole-pairs)
volatile unsigned int CrossOverERPS = 250; 		// low speed to high speed (and vice versa) crossover speed in electrical revs per second
volatile unsigned int NumOfTimer1TimeOuts = 10;  // Defines the number of Timer 1 timeouts that will sequentially occur before the motor shuts down

const struct parameter_limits PercentageLimits = {0,100};  			// Limits for all parameters which are specified in %
const struct parameter_limits LockDurationLimits = {0,1000}; 		// 1 second limit for lock duration
const struct parameter_limits RampDurationLimits = {500, 6500}; 	// The ramp duration is between 1/2 and 6.5 seconds
const struct parameter_limits RampStartSpeedLimits = {10, 500}; 	// The ramp start speed is between 10 and 500 RPM 
const struct parameter_limits RampEndSpeedLimits = {500, 5000}; 	// The ramp end speed is between 500 and 5000 RPM
const struct parameter_limits MaxMotorSpeedLimits = {0, 65535};		// Maximum motor speed is 65535 RPM
const struct parameter_limits BlankingCountInitLimits = {0, 20};	// Blanking count shouldn't be greater than 20 ADC interrupts in length
const struct parameter_limits ControlTermLimits = {0, 32000};		// The P and I speed control terms shouldn't be greater that 32000
const struct parameter_limits PhaseAdvanceStartLimits = {0, 65535};	// This can be any RPM in the full range on the motor speed
const struct parameter_limits PhaseAdvanceSlopeLimits = {0, 30};	// Phase advance can't be greater than 30 degrees
const struct parameter_limits NoOfMotorPolesLimits = {2, 20};  		// The motor must have at least 2 poles but no more than 20
const struct parameter_limits CrossOverLimits = {100, 900};			// The CrossOver Point must occur in this region
const struct parameter_limits NumOfTimer1TimeOutsLimits = {0,20};   // Limit the number of Timer 1 time outs to 0

void GetParameters(void)
{
	unsigned int i;

	// First Perform some limit checking
	// Lock1Demand
	if (Lock1Demand > PercentageLimits.max)
		Lock1Demand = PercentageLimits.max;
	// Lock2Demand
	if (Lock2Demand > PercentageLimits.max)
		Lock2Demand = PercentageLimits.max;
	// RampStartDemand
	if (RampStartDemand > PercentageLimits.max)
		RampStartDemand = PercentageLimits.max;
	// RampEndDemand 
	if (RampEndDemand > PercentageLimits.max)
		RampEndDemand = PercentageLimits.max;
	// InitialDemand
	if (InitialDemand > PercentageLimits.max)
		InitialDemand = PercentageLimits.max;
	// Lock1Duration
	if (Lock1Duration > LockDurationLimits.max)
		Lock1Duration = LockDurationLimits.max;
	// Lock2Duration
	if (Lock2Duration > LockDurationLimits.max)
		Lock2Duration = LockDurationLimits.max;
	// RampDuration
	if (RampDuration < RampDurationLimits.min)
		RampDuration = RampDurationLimits.min; 
	if (RampDuration > RampDurationLimits.max)
		RampDuration = RampDurationLimits.max;
	// RampStartSpeed
	if (RampStartSpeed < RampStartSpeedLimits.min)
		RampStartSpeed = RampStartSpeedLimits.min; 
	if (RampStartSpeed > RampStartSpeedLimits.max)
		RampStartSpeed = RampStartSpeedLimits.max;
	// RampEndSpeed
	if (RampEndSpeed < RampEndSpeedLimits.min)
		RampEndSpeed = RampEndSpeedLimits.min; 
	if (RampEndSpeed > RampEndSpeedLimits.max)
		RampEndSpeed = RampEndSpeedLimits.max;
	// SpeedControl_P
	if (SpeedControl_P > ControlTermLimits.max)
		SpeedControl_P = ControlTermLimits.max;
	// SpeedControl_I
	if (SpeedControl_I > ControlTermLimits.max)
		SpeedControl_I = ControlTermLimits.max;
	// SpeedControl_D
	if (SpeedControl_D > ControlTermLimits.max)
		SpeedControl_D = ControlTermLimits.max;
	// MaxMotorSpeed
	if (MaxMotorSpeed > MaxMotorSpeedLimits.max)
		MaxMotorSpeed = MaxMotorSpeedLimits.max;
	// BlankingCountInit
	if (BlankingCountInit > BlankingCountInitLimits.max)
		BlankingCountInit = BlankingCountInitLimits.max;
	// PhaseAdvanceStart
	if (PhaseAdvanceStart > PhaseAdvanceStartLimits.max)
		PhaseAdvanceStart = PhaseAdvanceStartLimits.max;
	// PhaseAdvanceSlope
	if (PhaseAdvanceSlope > PhaseAdvanceSlopeLimits.max)
		PhaseAdvanceSlope = PhaseAdvanceSlopeLimits.max;
	// NoOfMotorPoles
	if (NoOfMotorPoles < NoOfMotorPolesLimits.min)
		NoOfMotorPoles = NoOfMotorPolesLimits.min; 
	if (NoOfMotorPoles > NoOfMotorPolesLimits.max)
		NoOfMotorPoles = NoOfMotorPolesLimits.max;
	// Check to make sure the Cross Over speed is in a certain range
	// The cross over speed is the electrical speed (in revolutions per second)
	//  that specifiecs when the high speed algorithm transitions from low to 
	//  high speed and vice versa.  Hysterisis is built in (see medium_event.c)
	if (CrossOverERPS < CrossOverLimits.min)
		CrossOverERPS = CrossOverLimits.min; 
	if (CrossOverERPS > CrossOverLimits.max)
		CrossOverERPS = CrossOverLimits.max;
	// NumOfTimer1TimeOuts 
	if (NumOfTimer1TimeOuts < NumOfTimer1TimeOutsLimits.min)
		NumOfTimer1TimeOuts = NumOfTimer1TimeOutsLimits.min; 
	if (NumOfTimer1TimeOuts > NumOfTimer1TimeOutsLimits.max)
		NumOfTimer1TimeOuts = NumOfTimer1TimeOutsLimits.max;

	lock1_demand = (unsigned int)((unsigned long)Lock1Demand)*FULL_DUTY/100;
	lock2_demand = (unsigned int)((unsigned long)Lock2Demand)*FULL_DUTY/100;
	lock1_duration = Lock1Duration*10;  //converts time in 1ms increments to time in 100us increments 
	lock2_duration = Lock2Duration*10;  //converts time in 1ms increments to time in 100us increments
	ramp_start_demand = (unsigned int)((unsigned long)RampStartDemand)*FULL_DUTY/100;
	ramp_end_demand = (unsigned int)((unsigned long)RampEndDemand)*FULL_DUTY/100;
	ramp_duration = RampDuration*10;  //converts time in 1ms increments to time in 100us increments 

	// speed (in number of 500us increments per sector as a function of RPM) 
	//     = 1/RPM * (60secs)/(1 minute) * (1 rev)/(# of poles) * (1 pole)/(6 sectors) * (10000 100us increments)/(1 second)
    //	   = 400000/(RPM*(# of poles))
	// The # of poles is equal to user_parameters_RAM[25] / 2 (magnetic poles verses electrical poles respectively)
	ramp_start_speed = (unsigned int)(200000UL/((unsigned long)NoOfMotorPoles*(unsigned long)RampStartSpeed));
	ramp_end_speed = (unsigned int)(200000UL/((unsigned long)NoOfMotorPoles*(unsigned long)RampEndSpeed));

	// the end speed must be greater (smaller number here) than the start speed
	if (ramp_start_speed < ramp_end_speed)
		ramp_start_speed = ramp_end_speed + 2;

	// the ramp speed rate is in # of times the speed should be incremented over the ramp duration
	ramp_speed_rate = (unsigned int)(ramp_duration/(ramp_start_speed - ramp_end_speed))+1;

	// the end demand must be larger than the start demand
	if (ramp_end_demand < ramp_start_demand)
		ramp_end_demand = ramp_start_demand + 2;	
		
	// ramp_demand_rate is the number that demand should be increased by every tiime the speed is increased
	ramp_demand_rate = (unsigned int)(ramp_duration/(ramp_end_demand - ramp_start_demand))+1;


	// speed_constant is used to take the time between zero crossings and convert this to RPM
	// RPM = 1/Timer 1 * 1/Timer 1 prescaler * Fcy * 60seconds/min * 1/2 (1/2 Electrical Revolution) 
	//		  * 2/# or magnetic poles
	// RPM = 60 FCY/(Timer 1 prescaler * # or poles * Timer 1)
	// speed_constant = (60 FCY/(Timer 1 prescaler * # or poles)
	speed_constant = (60UL*FCY)/((unsigned long)NoOfMotorPoles*TMR1_PRESCALER);			

	// get the speed multiplyer for the potentiometer for converting the ADC result to RPM
	// Example: if RPM_converter_constant = 3 and the ADC result for the POT is 1000, the POT
	// 			setting corresponds to 3000RPM
	RPM_converter_constant = (MaxMotorSpeed/1024) + 1;

	// get the blanking count
	blanking_count_initial = BlankingCountInit;

	// Phase advance start speed
	phase_advance_start = PhaseAdvanceStart;

	// phase advance slope in degrees per 1000 RPM
	phase_advance_slope = PhaseAdvanceSlope;

	// limiting speed
	speed_max = MaxMotorSpeed;

	// load the PID gain coeffecients into an array;
	PIDGainCoefficients[0] = SpeedControl_P;
	PIDGainCoefficients[1] = SpeedControl_I;
	PIDGainCoefficients[2] = SpeedControl_D;				

	// Initialize the PIDStructure variable before calculation the K1, K2, and K3 terms	
	PIDStructure.abcCoefficients = abcCoefficients;	
	PIDStructure.controlHistory = controlHistory;
	PIDCoeffCalc(PIDGainCoefficients, &PIDStructure);
	// initialize control history
	for (i=0; i>2; i++)
	{
		controlHistory[i] = 0;
	}
	if (RunMode == SENSORLESS_INIT)
		PIDStructure.controlOutput = (unsigned int) (32439UL*InitialDemand/100);
	else
		PIDStructure.controlOutput = 0;

	// Initialize 180 degree time
	for (i=0; i<16; i++)
	{
			OneEightyDegreeTime[i] = (unsigned int)((FCY*60)/(TMR1_PRESCALER*NoOfMotorPoles*RampStartSpeed));
	}
}


