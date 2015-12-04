// Function declarations
void Commutate(unsigned int sector);

// General definitions
#define NO_OF_RUN_MODES 5
	
// algorithm state definitions (Loaded in RunMode)
#define MOTOR_OFF 0
#define SENSORLESS_INIT 1
#define SENSORLESS_START 2
#define SENSORLESS_RUNNING 3
#define HALL_SENSOR_MODE 4

// sensorless start mode state definitions (loaded into SensorlessStartState)
#define LOCK1 0
#define LOCK2 1
#define RAMP_INIT 2
#define RAMP 3

// This bit structure provides mode flags
struct ControlFlags{
        unsigned	RunMotor:1; 	
        unsigned	HighSpeedMode:1;
        unsigned 	TakeSnapshot:1;
		unsigned 	MediumEventFlag:1;
		unsigned	SlowEventFlag:1;
		unsigned	SpeedControlEnable:1;
		unsigned	EnablePotentiometer:1;
        unsigned   	:9;
}; 

extern volatile struct ControlFlags ControlFlags;

extern unsigned int ZeroCrossPeriodFalling[8];
extern unsigned int ZeroCrossPeriodRising[8];
extern unsigned int RunMode;
extern unsigned int SensorlessStartState;
extern unsigned int phase_advance;
extern int signal_average;
extern unsigned int pot;
extern int vbus;
extern int Sector;
extern unsigned long Speed;
extern unsigned int OneEightyDegreeTime[16];

extern unsigned int SpeedReference;
extern unsigned char comError;
