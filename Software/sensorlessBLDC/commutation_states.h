// These are the override control states for soft chopping with
// synchronous rectification.  BEMF zero cross will be referenced 
// to 0V DC.

#define		SS_STATE0 	0x0304		// A complementary, B bottom
#define		SS_STATE1 	0x0310		// A complementary, C bottom		 
#define		SS_STATE2 	0x0C10		// B complementary, C bottom
#define		SS_STATE3 	0x0C01		// B complementary, A bottom
#define		SS_STATE4 	0x3001		// C complementary, A bottom
#define  	SS_STATE5   0x3004		// C complementary, B bottom

// These are the override control states for hard chopping.
// BEMF zero crossing events will be referenced to Vbus/2.

// Clockwise

#define		STATE0 	0x0600		// A top, B bottom, C BEMF
#define		STATE1 	0x1200		// A top, C bottom, B BEMF		 
#define		STATE2 	0x1800		// B top, C bottom, A BEMF
#define		STATE3 	0x0900		// B top, A bottom, C BEMF
#define		STATE4 	0x2100		// C top, A bottom, B BEMF
#define  	STATE5  0x2400		// C top, B bottom, A BEMF


// Anticlockwise
/*
#define		STATE0 	0x0900		// B top, A bottom, C BEMF
#define		STATE1 	0x2100		// C top, A bottom, B BEMF
#define  	STATE2  0x2400		// C top, B bottom, A BEMF
#define		STATE3 	0x0600		// A top, B bottom, C BEMF
#define		STATE4 	0x1200		// A top, C bottom, B BEMF		 
#define		STATE5 	0x1800		// B top, C bottom, A BEMF
*/



// Sector 0 -- looking for FALLING edge Phase C
// Sector 1 -- looking for RISING edge Phase B
// Sector 2 -- looking for FALLING edge Phase A		
// Sector 3 -- looking for RISING edge Phase C
// Sector 4 -- looking for FALLING edge Phase B
// Sector 5 -- looking for RISING edge Phase A		
