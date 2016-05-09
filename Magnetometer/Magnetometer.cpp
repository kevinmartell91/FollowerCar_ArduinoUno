/*
  Magnetometer.h - Library for using gyroscope.
  Crated by Kevin Martell Luya, May 7, 2016.
*/

#include "Arduino.h"
#include "Magnetometer.h"
#include <Wire.h>  


Magnetometer::Magnetometer(){
}

void Magnetometer::calculateMagnetometerValues(){

	//Tell the HMC5883 where to begin reading data
	Wire.beginTransmission(ADDRESS);
	Wire.write(0x03); //select register 3, X MSB register
	Wire.endTransmission();


	//Read data from each axis, 2 registers per axis
	Wire.requestFrom(ADDRESS, 6);
	if(6<=Wire.available()){
		x = Wire.read()<<8; //X msb
		x |= Wire.read(); //X lsb
		z = Wire.read()<<8; //Z msb
		z |= Wire.read(); //Z lsb
		y = Wire.read()<<8; //Y msb
		y |= Wire.read(); //Y lsb
	}

	delay(250); // TODO : preguntar para o profesor
}

int Magnetometer::setupHMC5883(){
	//Put the HMC5883 IC into the correct operating mode
	Wire.beginTransmission(ADDRESS); //open communication with HMC5883
	Wire.write(0x02); //select mode register
	Wire.write(0x00); //continuous measurement mode
	Wire.endTransmission();

}

int Magnetometer::getX(){
	return x;
}

int Magnetometer::getY(){
	return y;
}

int Magnetometer::getZ(){
	return z;
}