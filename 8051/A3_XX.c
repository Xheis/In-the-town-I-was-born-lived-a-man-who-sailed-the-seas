/*_____________________________________________________________________________________________________________________

        Project:        
        Module:         
        Author:         
        Date:           

        Description:
        This program ....

        Notes:          

        Revisions:


_______________________________________________________________________________________________________________________*/

#include "c8051f120.h"     // SFR declarations
#include "A3_XX.h"
#include "LCD.h"
#include "Methods.h"

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Main

        Description:      Main routine

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void main(void)
{
  Oscillator_Init();
	General_Init();
	Timer_Init();
	//LCD_Init();	
	Interrupts_Init();	

	while(1)
	{	
		LD1 = ~LD1;
		delay_micro(100);
		// Do stuff

	}
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         General_Init

        Description:      Initialise ports, watchdog....
--------------------------------------------------------------------------------------------------------------------*/
void General_Init()
{
	WDTCN = 0xde;
	WDTCN = 0xad;
  	SFRPAGE = CONFIG_PAGE;
	P0MDOUT = 0x10;		// NOTE: Pushpull required for Servo control OTHERWISE TOO WEAK TO DRIVE PROPERLY SINCE ONLY 3.3V!!!!
	P1MDOUT = 0x00;		// Ensure not pushpull outputs....this could damage microcontroller...
	P2MDOUT = 0xff;		// Need to make pushpull outputs to drive LEDs properly
	XBR2 = 0x40;
	Servo_Ctrl = 0;	// Initialise servo control pin to 0
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer_Init

        Description:      
*/
