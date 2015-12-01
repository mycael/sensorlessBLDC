# Sensorless Controller for BLDC Motor
Hardware and Software description for a sensorless BLDC Controller

**Please be careful**: _the controller is not yet fully tested and still requires development._

Hardware is designed with Altium Designer, source files are available in "Hardware" folder.
Software is written for dsPIC30F3011 in MPLABX IDE and is available in "Software" folder.
The Controller is currently tested on Turnigy D2836/8 1100kv motor.

___

## 1. General Characteristics:
-	Current: 120A
-	Voltage: 10 to 20V (3S, 4S, and 5S Lipo) 
-	Power dissipation without heatsink : 300W
-	Speed setpoint set with PWM signal 

___

## 2. Software Specifications

### Input PWM signal (speed setpoint) :

![Alt text](/Documentation/chrono1.png?raw=true)

![Alt text](/Documentation/equ1.gif?raw=true)

In the equation above, *Speed* and *MaxMotorSpeed* are given in RPM (Revolutions per Minute), *PulseWidth* and *MaxPulseWidth* in µs (microseconde).

*Period* should be 25% greater than *MaxPulseWidth*

*MaxPulseWidth* and *MaxMotorSpeed* can be set in software in *tuning.c* file.

___

## 3. Hardware Specifications

### 3D view of the BLDC Controller: 

-	Top layer :

![Alt text](/Documentation/toplayer.png?raw=true)
-	Bottom layer :

![Alt text](/Documentation/bottomlayer.png?raw=true)
- Dimensions : 

![Alt text](/Documentation/dimensions.png?raw=true)
