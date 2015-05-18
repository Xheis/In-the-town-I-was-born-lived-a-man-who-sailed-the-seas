#include "Servo.h"
#include "c8051f120.h"     // SFR declarations
#include "A3_XX.h"
#include "LCD.h"
#include "Methods.h"

// Definitions 
#define 	SERVO_TIMEOUT			20000 // microseconds


/***************************************** SERVO **************************************************/
void servoWrite_microseconds(unsigned char time)
{
	static unsigned long old_time = 0;
	static unsigned char inPulse = 0;
	int temp;  	
	temp = micros()-old_time;
	
	if(temp>SERVO_TIMEOUT){
		old_time = micros();
		inPulse = 1;
		Servo_Ctrl = 1; // Trigger pin
		RCAP3L  = servo_LB[time];
    	RCAP3H  = servo_HB[time];


		TR3 = 1;
		}


	
}
	

void Timer3_ISR(void){
	Servo_Ctrl = 0;
	TF3 = 0;
	TR3 = 1;
}


void Timer3_Init(){
	SFRPAGE   = TMR3_PAGE;
    TMR3CN    = 0x04;
    TMR3CF    = 0x08;
    RCAP3L    = servo_LB[90];
    RCAP3H    = servo_HB[90];
	}