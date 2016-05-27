/*
  MadgwickQuaternion.h - Library for using gyroscope.
  Edited by Kevin Martell Luya, May 17, 2016.
*/

#ifndef MadgwickQuaternion_h
#define MadgwickQuaternion_h

#include "Arduino.h"

class MadgwickQuaternion
{
  public:
      MadgwickQuaternion();
      void calculateMadgwickQuaternionValues(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
      void MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz);
      float getPitch();
      float getYaw();
      float getRoll();
      void setDeltat(float deltat_time);
      void setSerialDebug(bool debug);


  private:
      // global constants for 9 DoF fusion and AHRS (Attitude and Heading Reference System)
      #define GyroMeasError PI * (40.0f / 180.0f)       // gyroscope measurement error in rads/s (shown as 40 deg/s)
      #define GyroMeasDrift PI * (0.0f / 180.0f)      // gyroscope measurement drift in rad/s/s (shown as 0.0 deg/s/s)
      // There is a tradeoff in the beta parameter between accuracy and response speed.
      // In the original Madgwick study, beta of 0.041 (corresponding to GyroMeasError of 2.7 degrees/s) was found to give optimal accuracy.
      // However, with this value, the LSM9SD0 response time is about 10 seconds to a stable initial quaternion.
      // Subsequent changes also require a longish lag time to a stable output, not fast enough for a quadcopter or robot car!
      // By increasing beta (GyroMeasError) by about a factor of fifteen, the response time constant is reduced to ~2 sec
      // I haven't noticed any reduction in solution accuracy. This is essentially the I coefficient in a PID control sense; 
      // the bigger the feedback coefficient, the faster the solution converges, usually at the expense of accuracy. 
      // In any case, this is the free parameter in the Madgwick filtering and fusion scheme.
      #define beta sqrt(3.0f / 4.0f) * GyroMeasError   // compute beta

      float pitch, yaw, roll;
      float deltat;        // integration interval for both filter schemes
      float ax, ay, az, gx, gy, gz, mx, my, mz; // variables to hold latest sensor data values 
      float q[4] ;    // vector to hold quaternion
      
      bool serialDebug;

};

#endif