# Sensorless Controller for BLDC Motor
Hardware and Software description for a sensorless BLDC Controller

**Please be careful**: _the controller is not yet fully tested and still requires development._

Hardware is designed with Altium Designer, source files are available in "Hardware" folder.
Software is written for dsPIC30F3011 in MPLABX IDE and is available in "Software" folder.
The Controller is currently tested on Turnigy D2836/8 1100kv motor.

___

## 1. General Characteristics
-	Current: 120A
-	Voltage: 10 to 20V (3S, 4S, and 5S Lipo) 
-	Power dissipation without heatsink : 300W
-	Speed setpoint set with PWM signal 

___

## 2. Software Specifications

### Key points
* Firmware based on dsPIC30F3011
* Cycle frequency : 29 491 200 Hz (7.3728 MHz crystal with PLL 16x)
* Median filter to avoid command reading error

### Input PWM signal (speed setpoint) :

Crystal use instead of internal oscillator (as it's generally made on ESC) ensures a high precision reading of the speed setpoint coming from the PWM signal. 

![Alt text](/Documentation/chrono1.png?raw=true)

![Alt text](/Documentation/equ1.gif?raw=true)

In the equation above, *Speed* and *MaxMotorSpeed* are given in RPM (Revolutions per Minute), *PulseWidth* and *MaxPulseWidth* in µs (microseconde).

*Period* should be 25% greater than *MaxPulseWidth*

*MaxPulseWidth* and *MaxMotorSpeed* can be set in software in *general.h* file (see next part).

### Basic configuration
In the *general.f* file, you can change some parameters : 
- line 5 : You can adjust **motor** PWM frequency (min: 10kHz, max: 30kHz, default: 20Hz)
``` c 
#define   FPWM	  20000   // Hz
```

- line 8 : Set the maximum motor speed in RPM
``` c 
#define   MAX_MOTOR_SPEED     8000.0    // rpm
```

- line 11 : Set the speed at which the motor starts to spin (min: 2200, max: MAX_MOTOR_SPEED, default: 2200)
``` c 
#define   START_MOTOR_VALUE   2200	// rpm
```

- line 14 : Set the command pulse width which will correpond to the maximum motor speed (1)
``` c 
#define   MAX_PULSE_WIDTH     0.015   // sec 
```

(1) This controller can not perform clockwise or anticlockwise direction in the same application, under the start motor speed value (START_MOTOR_VALUE), the motor is stopped. To make the motor turns in an other direction, switch 2 phases between motor and controller.

### Advanced configuration
In the file *tuning.c*, you can change different parameter to enhance motor startup.

...
___

## 3. Hardware Specifications

### 3D view of the BLDC Controller: 

-	Top layer :

![Alt text](/Documentation/toplayer.png?raw=true)
-	Bottom layer :

![Alt text](/Documentation/bottomlayer.png?raw=true)
- Dimensions : 

![Alt text](/Documentation/dimensions.png?raw=true)
