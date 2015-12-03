/*
Pin_Number	Pin_Name	Function
1			MCLR		Device reset via S1
2			AN0			Bus voltage sense
3			AN1			Motor Current sense
4			AN2			Potentiometer (R14) input
5			AN3			Phase 1 back EMF sense
6			AN4			Phase 2 back EMF sense
7			AN5			Phase 3 back EMF sense
8			Vss			Ground
9			OSC1		Crystal oscillator input
10			OSC2		Crystal oscillator input
11			RC13		Switch (S3) input 
12			RC14		Switch (S2) input
13			Vdd			+5V
14			EMUD2		In-circuit debugging data pin
15			EMUC2		In-circuit debuggin clock pin
16			FLTA		Over-Current fault input (Current trip level set by R60)
17			PGD			In-circuit serial programming data pin
18			PGC			In-circuit serial programming clock pin
19			Vss			Ground
20			Vdd			+5V
21			PWM3H		Drives high-side MOSFET (Q5) on Phase 3
22			PWM3L		Drives low-side MOSFET (Q6) on Phase 3
23			PWM2H		Drives high-side MOSFET (Q3) on Phase 2
24			PWM2L		Drives low-side MOSFET (Q4) on Phase 2
26			PWM1L		Drives low-side MOSFET (Q2) on Phase 1
26			PWM1H		Drives high-side MOSFET (Q1) on Phase 1
27			AVss		Ground
28			Avdd		+5V
*/
	
#define	BUTTON_S2	!PORTCbits.RC14		//S2 on MCLV
#define	BUTTON_S3	!PORTCbits.RC13		//S3 on MCLV
#define LED         LATCbits.LATC13



