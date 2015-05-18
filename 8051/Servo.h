/* 		Servo.h    */
#ifndef SERVO_H
#define SERVO_H


// Prototypes
void servoWrite_microseconds(unsigned int);
void Timer3_Init();

// Global Varibles
unsigned char code servo_HB[] = {
	#include "servo_HB.csv"
	};
unsigned char code servo_LB[] = {
	#include "servo_LB.csv"
	};









#endif