/*
  Accelerometer.h - Library for using gyroscope.
  Crated by Kevin Martell Luya, May 7, 2016.
*/

#include "Arduino.h"
#include "Accelerometer.h"
#include <Wire.h>  

Accelerometer::Accelerometer(){
	ADXAddress = 0x53;  // the default 7-bit slave address
	reading = 0;
	val=0;
}

int Accelerometer::setupADXL345(){
	Wire.beginTransmission(ADXAddress);
  	Wire.write(Register_2D);
    Wire.write(8);                //measuring enable
    Wire.endTransmission();     // stop transmitting
}

void Accelerometer::calculateAccelerometerValues(){
	//--------------X
	  Wire.beginTransmission(ADXAddress); // transmit to device
	  Wire.write(Register_X0);
	  Wire.write(Register_X1);
	  Wire.endTransmission();
	  Wire.requestFrom(ADXAddress,2); 
	  if(Wire.available()<=2)   
	  {
	    X0 = Wire.read();
	    X1 = Wire.read(); 
	    X1=X1<<8;
	    X_out=X0+X1;   
	  }

	  //------------------Y
	  Wire.beginTransmission(ADXAddress); // transmit to device
	  Wire.write(Register_Y0);
	  Wire.write(Register_Y1);
	  Wire.endTransmission();
	  Wire.requestFrom(ADXAddress,2); 
	  if(Wire.available()<=2)   
	  {
	    Y0 = Wire.read();
	    Y1 = Wire.read(); 
	    Y1=Y1<<8;
	    Y_out=Y0+Y1;
	  }
	  //------------------Z
	  Wire.beginTransmission(ADXAddress); // transmit to device
	  Wire.write(Register_Z0);
	  Wire.write(Register_Z1);
	  Wire.endTransmission();
	  Wire.requestFrom(ADXAddress,2); 
	  if(Wire.available()<=2)   
	  {
	    Z0 = Wire.read();
	    Z1 = Wire.read(); 
	    Z1=Z1<<8;
	    Z_out=Z0+Z1;
	  }
	  //
	  Xg=X_out/256.0;
	  Yg=Y_out/256.0;
	  Zg=Z_out/256.0;
	  delay(200); // TODO : pregunta para o profesor
}

double Accelerometer::getX(){
	return Xg;
}

double Accelerometer::getY(){
	return Yg;
}

double Accelerometer::getZ(){
	return Zg;
}