// LCD.c - This .c file contains the declarations 
// for the LCD functions 
#include <c8051f120.h>     // SFR declarations
#include "A3_XX.h"
#include "LCD.h"
#include "Methods.h"






/*--------------------------------------------------------------------------------------------------------------------
        Function:         LCD_Init

        Description:      Initialise LCD. Note that the LCD has a 4 bit hardware connection 

													P3.7	BL	=> Backlight		
													P3.6	E		=> Enable		
													P3.5	R/W	=> Read/Write line		
													P3.4	RS	=> Register Select		
													P3.3	D7	=> Data line		
													P3.2	D6	=> Data line		
													P3.1	D5	=> Data line		
													P3.0	D4	=> Data line															

        Revisions:


*/
/*
 * This function initializes the LCD during power on and first use
*/
void initLCD(){
	delay(30);// Wait 30 milliseconds

	P3 = FUNCTION_SET; 	// Send the function set to the LCD
	pulse_Enable(5); 	// Pulse the strobe

	cmdLCD(TWO_LINE_CONTROL); // Tell LCD to use 2 lines on the screen
	
	cmdLCD(CONTROL);// Specify the control type
	

	cmdLCD(ENTRY_MODE_SET);	// Entry Mode Set
	
	cmdLCD(CLR_DISP); // Clear the Display
	printLCD("    ELEC2700    "); // Print a label to the screen
	delay(1000);	// Show label for 1 second
	cmdLCD(CLR_DISP); // Clear the Display

	BL = 1; // Set the BL to 1 so that the push button can still be used
	
// Now LCD may be used!!!
}



/* This function prints strings of characters to the display */
void printLCD(unsigned char *word){
	int i = 0;
	while(*word){ // print character by character till we reach the NULL character
		writeLCD(*word++); // write current character to LCD and increment pointer to the next character in the word
	}
}



/* Write a single character to the LCD screen */
void writeLCD(unsigned char character){ 
	/* Send first Nibble */
	P3 = 0; 	// Clear P3
	RS = 1; 	// Set Register Select for writing charcters to LCD
	P3 = P3|(character>>4)&(0x0F); // Upper nibble of character
	pulse_Enable(1); // Strobe
	
	/* Send last Nibble */
	P3 = 0; 	//  Clear P3 again
	RS = 1;	// Set Register Select for writing charcters to LCD
	P3 = P3|(character)&(0x0F); 	// Lower nibble of character
	pulse_Enable(1);	// Strobe		
	BL = 1;	// Set the BL to 1 so that the push button can still be used
	RS = 0;	// Clear Regsister Select
}



/* This function is for writing numbers to the LCD */
void writeIntLCD(unsigned char num){	

	if(num>=100){ // Number is less than 100
		writeLCD((num/100) + 48); //Highest decimal value
		writeLCD((num/10)%10 + 48); // Middle decimal value
		writeLCD((num%10) + 48); // Lowest decimal value 
	}
	else if(num>=10){// Number is less than 100 but greater or equal to 10
		writeLCD(' '); // Write a clear space to justify the number
		writeLCD((num/10)%10 + 48); // Middle decimal value 
		writeLCD((num%10) + 48); // Lowest decimal value 
	}
	else if(num<10){// Number is less than 10
		writeLCD(' ');	// Write a clear space to justify the number
		writeLCD(' ');	// Write another clear space to justify the number
		writeLCD((num%10) + 48); // Lowest decimal value 
	}
}


/* This function sets the cursor to any position on the 16x2 display */
void setCursor(unsigned char x, unsigned char y){
	unsigned char i;
	if(y==0){ //  set the cursor to the start of line 2
	cmdLCD(START_LINE1);
	}
	else{//  set the cursor to the start of line 2
	cmdLCD(START_LINE2);
	}
	for(i = 0; i<x; i++){
		cmdLCD(MOVE_RIGHT);/* Move lcd from start of line to the position specified by x */
	}
}


/* This function strobes the enable pin for the time specified by i */
void pulse_Enable(unsigned char i){
	ENABLE = 1; //  Set enable
	delay(i); // Wait
	ENABLE = 0; // Clear enable
	delay(i);	 // Wait
}

/* This function is for sending commands to the LCD */
void cmdLCD(unsigned char cmd){
	// Send first Nibble 
	P3 = 0; // Clear P3
	P3 = P3|(cmd>>4)&(0x0F); // Upper nibble of character
	pulse_Enable(3);	// Strobe
	
	/* Send last Nibble */
	P3 = 0;	// Clear P3
	P3 = P3|(cmd)&(0x0F); // Lower nibble of character
	pulse_Enable(3);	// Strobe
	BL = 1;		// Set the BL to 1 so that the push button can still be used
}


