/*
  Gyroscope.h - Library for using gyroscope.
  Crated by Kevin Martell Luya, May 7, 2016.
*/

#ifndef Gyroscope_h
#define Gyroscope_h

#include "Arduino.h"

class Gyroscope
{
	public:
    	Gyroscope();
    	void calculateGyroscopeValues();
	    int setupL3G4200D(int scale);
		void writeRegister(int deviceAddress, byte address, byte val);
		int readRegister(int deviceAddress, byte address);
		int getX();
		int getY();
		int getZ();

 	private:
	    #define CTRL_REG1 0x20
		#define CTRL_REG2 0x21
		#define CTRL_REG3 0x22
		#define CTRL_REG4 0x23
		#define CTRL_REG5 0x24

		int L3G4200D_Address; //I2C address of the L3G4200D

		int x;			
		int y;
		int z;
};

#endif