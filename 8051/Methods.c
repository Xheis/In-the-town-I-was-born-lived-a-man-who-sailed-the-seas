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

#include "Servo.h"
#include "c8051f120.h"     // SFR declarations
#include "A3_XX.h"
#include "LCD.h"
#include "Methods.h"


/*    Definitions    */
#define		TIMEOUT					250		//A timeout value— if we've passed 250ms, we haven't received a signal


/*    Global Variables        */
unsigned short milliseconds = 0; // millis function variable
unsigned long microseconds = 0; // micros function variable

unsigned long microseconds_temp = 0;

//--------------------------------------------------------------------------------------------------------------------
//                              Functions & Methods
//--------------------------------------------------------------------------------------------------------------------


/*		Voltage_Reference_Init	*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Voltage_Reference_Init

        Description:      Initialise voltage references (Needed for DAC)
--------------------------------------------------------------------------------------------------------------------*/
void Voltage_Reference_Init()
{
    SFRPAGE   = ADC0_PAGE;	//Setup SFR References
    REF0CN    = 0x02;
}


/*		Oscillator_Init			*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Oscillator_Init

        Description:      Initialise the system Clock at a faster rate  (Needed for DAC)
--------------------------------------------------------------------------------------------------------------------*/
void Oscillator_Init()
{
	/* All values from the Config Wizard */
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
--------------------------------------------------------------------------------------------------------------------*/
void Timer_Init()
{
	/* Timer 1 and Timer 0 are setup */
	SFRPAGE   = TIMER01_PAGE;
	TCON      = 0x50;
    TMOD      = 0x11;
    CKCON     = 0x12;
    
	
	reset_Timer_0();
	reset_Timer_1();
	/* Timer 2 */
    SFRPAGE   = TMR2_PAGE; /* 2^14 sample rate */
    TMR2CN    = 0x04;
    TMR2CF    = 0x0A;
    RCAP2L    = 0x29;
    RCAP2H    = 0xFA;

	Timer3_Init();
}





/*		DAC_Init				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         DAC_Init

        Description:      Initialise DAC0. 
--------------------------------------------------------------------------------------------------------------------*/
void DAC_Init()
{
	SFRPAGE   = DAC0_PAGE;
    DAC0CN    = 0x84; 
}


/*		Interrupts_Init			*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Interrupts_Init

        Description:      Initialise interrupts
--------------------------------------------------------------------------------------------------------------------*/
void Interrupts_Init()
{
		IE = 0x0; 		//Clear the register 
		EA = 1; 		// Enable global interupts
		//ET2 = 1;		// Enable timer2 interrupt 
		ET0 = 1;		// Enable timer0 interrupt 
		ET1 = 1;
}






/*		Timer2_ISR				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         Timer2_ISR

        Description:      Timer Interrupt Service which will call our DAC_Multi_Sine_Wave(); function, and reset the Interrupt
--------------------------------------------------------------------------------------------------------------------*/
void Timer2_ISR (void) interrupt 5
{
    
    TF2 = 0;        // Reset Interrupt
}



/*		delay				*/
/*--------------------------------------------------------------------------------------------------------------------
        Function:         delay

        Description:      Creates a delay for any application e.g. debouncing
--------------------------------------------------------------------------------------------------------------------*/
void delay(unsigned short delay_len){

	unsigned short oldtime = millis(); 	//Store the current time
	while((millis()-oldtime)<delay_len); //wait for time to pass
}	

		








/*--------------------------------------------------------------------------------------------------------------------
        Function:         update_millis()

        Description:      Interrupt for a millisecond timer
--------------------------------------------------------------------------------------------------------------------*/
/* Interrupt for a millisecond timer */
void update_millis(void) interrupt 1
{
	milliseconds++;
	
	reset_Timer_0();
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         Reset Timer 0

        Description:      Resets and starts timer 0 for use in the millis function
--------------------------------------------------------------------------------------------------------------------*/
void reset_Timer_0(void){
	TF0 			= 0;  		/* Clear flag */
	TL0       		= 0x06;		/* Top up for a 1 millisecond delay */
  	TH0       		= 0xF8;
	TR0 			= 1; 		/* Enable Timer */
}


/*--------------------------------------------------------------------------------------------------------------------
        Function:         millis

        Description:      returns milliseconds passed
--------------------------------------------------------------------------------------------------------------------*/
unsigned short millis(){
	return(milliseconds);
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         millis RESET 

        Description:      Resets the milliseconds passed
--------------------------------------------------------------------------------------------------------------------*/
void millis_RESET(void){
	milliseconds = 0;
}








/*--------------------------------------------------------------------------------------------------------------------
        Function:         update_micros()

        Description:      Interrupt for a microsecond timer
--------------------------------------------------------------------------------------------------------------------*/
/* Interrupt for a millisecond timer */
void update_micros(void) interrupt 3
{
	microseconds++;
	//microseconds_temp;
	reset_Timer_1();
}



/*--------------------------------------------------------------------------------------------------------------------
        Function:         Reset Timer 1

        Description:      Resets and starts timer 0 for use in the millis function
--------------------------------------------------------------------------------------------------------------------*/
void reset_Timer_1(void){
	TF1 			= 0;  		/* Clear flag */
	TL1       		= 0x9E;		/* Top up for a 1 microsecond delay */
	TH1      		= 0xFF;
	TR1 			= 1; 			/* Enable Timer */
}

/*--------------------------------------------------------------------------------------------------------------------
        Function:         micros

        Description:      returns microseconds passed
--------------------------------------------------------------------------------------------------------------------*/
unsigned long micros(void){
	return(microseconds);
}
void delay_micro(unsigned int delay_len){
	
	unsigned long oldtime = micros(); 	//Store the current time
	while((micros()-oldtime)<delay_len); //wait for time to pass

}

void micros_RESET(void){
	microseconds = 0;
}








/*--------------------------------------------------------------------------------------------------------------------
        Function:         PulseGeneration()

        Description:      Generates and detects a pulse 
--------------------------------------------------------------------------------------------------------------------*/

void PulseGeneration(void)
{
	char i = 0;
	char signalRecieved = 0;
	unsigned short timeStart = 0;
	unsigned short timePassed = 0;
	
	for (i = 0; i < 4; i++)	//4 pulses
	{
		//Generate 40kHz pulse
		//TODO: pwm code
	}
	delay(1);			//Pause for a masking time
	
	//Start timer
	timeStart = millis();	//this is our starting time. We'll compare this time to current time to check for a time out
	while (!signalRecieved)
	{
		timePassed = millis() - timeStart;
		//Check to see if P0.1 is high
		if (P0 ^ 3)
		{
			//We got a signal
			signalRecieved = 1;
			SendUltrasonicTimes(timePassed);
		}
		else if (timePassed > TIMEOUT)
		{
			//We have not received a signal.
			
			break;	//Break out loop, and we'll check signal received
		}
	}
}

//Send stuff over the coms
void SendUltrasonicTimes(int tempTime)
{
	//send over com port
	// tempTime
}

void InitialiseUART()
{
	//Vector is 0x0023 for UART
	TI0 = 0x00; //This is our transmission complete interrupt.
	RI0 = 0x00; //This is our receive complete interrupt, which we need.
	/* NOTE: The software interrupts are not reset by hardware, so we must reset them manually. */
	SCON0 = 0x0;
}



