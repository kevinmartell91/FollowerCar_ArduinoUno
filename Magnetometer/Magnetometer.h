/*
  Magnetometer.h - Library for using gyroscope.
  Crated by Kevin Martell Luya, May 7, 2016.
*/

#ifndef Magnetometer_h
#define Magnetometer_h

#include "Arduino.h"

class Magnetometer
{
	public:
    	Magnetometer();
    	void calculateMagnetometerValues();
	    int setupHMC5883();
		int getX();
		int getY();
		int getZ();

 	private:
 		#define ADDRESS 0x1E
 		const int c = 4;
 		//0011110b, I2C 7bit address of HMC5883

		int x;			
		int y;
		int z;

	   			
};

#endif