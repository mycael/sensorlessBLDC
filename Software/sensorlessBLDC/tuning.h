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



