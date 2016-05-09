/*
  Accelerometer.h - Library for using gyroscope.
  Crated by Kevin Martell Luya, May 7, 2016.
*/

#ifndef Accelerometer_h
#define Accelerometer_h

#include "Arduino.h"

class Accelerometer
{
	public:
    	Accelerometer();
    	void calculateAccelerometerValues();
	    int setupADXL345();
		double getX();
		double getY();
		double getZ();

 	private:
	    #define Register_ID 0
		#define Register_2D 0x2D
		#define Register_X0 0x32
		#define Register_X1 0x33
		#define Register_Y0 0x34
		#define Register_Y1 0x35
		#define Register_Z0 0x36
		#define Register_Z1 0x37

		int ADXAddress;  // the default 7-bit slave address
		int reading;
		int val;
		int X0,X1,X_out;
		int Y0,Y1,Y_out;
		int Z1,Z0,Z_out;
		double Xg,Yg,Zg;
};

#endif