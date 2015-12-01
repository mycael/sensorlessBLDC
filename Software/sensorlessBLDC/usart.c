#include "usart.h"

void Init_USART(void)
{
    float U1BRG_f;
    U1MODE = 0x8000;
    U1STA = 0x0000;
    //************** set baud *******************************
    U1BRG_f = (((float)FCY/16.0)/(float)BAUD) - 1.0;	
    if ((U1BRG_f - (int)U1BRG_f) < 0.5) U1BRG = (int)U1BRG_f;
    else U1BRG = (int)U1BRG_f + 1;
    //*******************************************************

    U1STAbits.UTXEN = 1;           // Initiate transmission
}


void SendStr(char *s)
{
    while (*s)
	{
        while (U1STAbits.UTXBF);
        U1TXREG = *s++;
	}
}
