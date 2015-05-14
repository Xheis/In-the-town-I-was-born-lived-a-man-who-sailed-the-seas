/*
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
---------				This is our Methods file that contains any 					---------
---------				computations we need																---------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
-----------------------------------------------------------------------------
*/



//--------------------------------------------------------------------------------------------------------------------
//                              Declarations
//--------------------------------------------------------------------------------------------------------------------

#include <c8051f120.h>     // SFR declarations
#include "A3_XX.h"
#include "LCD.h"
#include "Methods.h"

/*    Definitions    */
#define  	WAVE_RESOLUTION    		256   	// Our 256bit sine wave resolution
#define   	MAX_VOLUME        		16    	// 16 different volumes
#define   	MAX_FADER      			255    	// 256 different fading volumes
#define		FADING_TIME				1000
#define 	SINE_OFFSET     		128 		// DC offset for sin wave
#define 	DEFAULT_OCTAVE			4
#define 	DEFAULT_VOLUME			15			// Defualt volume. 0-15. 0=> mute
#define		NUM_NOTES						14



/* */

typedef struct {
	unsigned char on : 1;   /* 1 bit for setting and clearing */
	unsigned char fad_value;
} bitbang;



/*    Global Variables        */
unsigned short milliseconds = 0;
	





//--------------------------------------------------------------------------------------------------------------------
//                              Functions & Methods
//--------------------------------------------------------------------------------------------------------------------


/*		Voltage_Reference_Init	*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Voltage_Reference_Init

        Description:      Initialise voltage references (Needed for DAC)

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void Voltage_Reference_Init()
{
    SFRPAGE   = ADC0_PAGE;
    REF0CN    = 0x02;
}














/*		Oscillator_Init			*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Oscillator_Init

        Description:      Initialise the system Clock at a faster rate  (Needed for DAC)

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void Oscillator_Init()
{
    int i = 0;
    SFRPAGE   = CONFIG_PAGE;
    OSCICN    = 0x83;
    CCH0CN    &= ~0x20;
    SFRPAGE   = LEGACY_PAGE;
    FLSCL     = 0xB0;
    SFRPAGE   = CONFIG_PAGE;
    CCH0CN    |= 0x20;
    PLL0CN    |= 0x01;
    PLL0DIV   = 0x01;
    PLL0FLT   = 0x01;
    PLL0MUL   = 0x04;
    for (i = 0; i < 15; i++);  // Wait 5us for initialization
    PLL0CN    |= 0x02;
    while ((PLL0CN & 0x10) == 0);
    CLKSEL    = 0x02;
   
}





/*		Timer_Init				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer_Init

        Description:      Initialise timer ports and registers

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void Timer_Init()
{
	/* Timer 1 and Timer 0 */
	SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x40;
    TMOD      = 0x11;
    CKCON     = 0x02;

	reset_Timer_0();

	/* Timer 2 */
    SFRPAGE   = TMR2_PAGE; /* 2^14 sample rate */
    TMR2CN    = 0x04;
    TMR2CF    = 0x0A;
    RCAP2L    = 0x29;
    RCAP2H    = 0xFA;
}






/*		DAC_Init				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         DAC_Init

        Description:      Initialise DAC0. 

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void DAC_Init()
{
	//We want this function to set up our DAC for general use. We will use the SetDAC() method for specifically readying the DAC for playing notes
	SFRPAGE   = DAC0_PAGE;
    DAC0CN    = 0x84; 
}


/*		Interrupts_Init			*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Interrupts_Init

        Description:      Initialise interrupts

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void Interrupts_Init()
{
		IE = 0x0; 	/* Clear the register */
		EA = 1; 		/* Enable global interupts */
		ET2 = 1;		/* Enable timer2 interrupt */
		ET0 = 1;		/* Enable timer0 interrupt */

}





/*		Timer2_ISR				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer2_ISR

        Description:      

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void Timer2_ISR (void) interrupt 5
{
    
    TF2 = 0;        // Reset Interrupt

}







/*		delay				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         delay

        Description:      Creates a delay for any application e.g. debouncing

        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
void delay(unsigned short delay_len){

	unsigned short oldtime = millis(); /* Store the current time */

	while((millis()-oldtime)<delay_len); /* wait for time to pass */
}	

		




/*		mirror_binary			*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         mirror_binary

        Description:      Used to mirror the byte used for the LEDs on the Perif. Board.
													i.e. LED8 becomes the MSB, whereas it is usually the LSB
        Revisions:

--------------------------------------------------------------------------------------------------------------------*/
unsigned char mirror_binary(unsigned char num){
	char i;
	unsigned char temp = 0;
	for(i=0; i<8; i++){
		temp += (((num>>i)&0x01)<<(7-i));
	}
	return(temp);
}






/* Interrupt for a millisecond timer */
void update_millis(void) interrupt 1
{
	milliseconds++;
	reset_Timer_0();
}

void reset_Timer_0(void){
	TF0 			= 0;  /* Clear flag */
	TL0       		= 0x06;	/* Top up for a 1 millisecond delay */
  	TH0       		= 0xF8;
	TR0 			= 1; 		/* Enable Timer */
}

unsigned short millis(){
	return(milliseconds);
}

void millis_RESET(void){
	milliseconds = 0;
}
