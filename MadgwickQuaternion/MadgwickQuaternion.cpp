/*
  Accelerometer.h - Library for using gyroscope.
  Edited by Kevin Martell Luya, May 17, 2016.
*/

#include "Arduino.h"
#include "MadgwickQuaternion.h"
#include <Wire.h>  


MadgwickQuaternion::MadgwickQuaternion()
{
    deltat = 0.0f;
	//q[4] = {1.0f, 0.0f, 0.0f, 0.0f};    // vector to hold quaternion
	q[0] = 1.0f;
	q[1] = 0.0f;
	q[2] = 0.0f;
	q[3] = 0.0f;

    serialDebug = false;	
}

void MadgwickQuaternion::calculateMadgwickQuaternionValues(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    if(serialDebug)
    {
        Serial.println("++++++++++++ input received");
        Serial.println(ax);
        Serial.println(ay);
        Serial.println(az);
        Serial.println(gx);
        Serial.println(gy);
        Serial.println(gz);
        Serial.println(mx);
        Serial.println(my);
        Serial.println(mz);
    }
    
	MadgwickQuaternionUpdate(ax, ay, az, gx*PI/180.0f, gy*PI/180.0f, gz*PI/180.0f,  mx,  my,  mz);

	// Define output variables from updated quaternion---these are Tait-Bryan angles, commonly used in aircraft orientation.
	// In this coordinate system, the positive z-axis is down toward Earth. 
	// Yaw is the angle between Sensor x-axis and Earth magnetic North (or true North if corrected for local declination, looking down on the sensor positive yaw is counterclockwise.
	// Pitch is angle between sensor x-axis and Earth ground plane, toward the Earth is positive, up toward the sky is negative.
	// Roll is angle between sensor y-axis and Earth ground plane, y-axis up is positive roll.
	// These arise from the definition of the homogeneous rotation matrix constructed from quaternions.
	// Tait-Bryan angles as well as Euler angles are non-commutative; that is, the get the correct orientation the rotations must be
	// applied in the correct order which for this configuration is yaw, pitch, and then roll.
	// For more see http://en.wikipedia.org/wiki/Conversion_between_quaternions_and_Euler_angles which has additional links.
	yaw   = atan2(2.0f * (q[1] * q[2] + q[0] * q[3]), q[0] * q[0] + q[1] * q[1] - q[2] * q[2] - q[3] * q[3]);   
	pitch = -asin(2.0f * (q[1] * q[3] - q[0] * q[2]));
	roll  = atan2(2.0f * (q[0] * q[1] + q[2] * q[3]), q[0] * q[0] - q[1] * q[1] - q[2] * q[2] + q[3] * q[3]);


	pitch *= 180.0f / PI;
	yaw   *= 180.0f / PI; 
	yaw   -= 13.8; // Declination at Danville, California is 13 degrees 48 minutes and 47 seconds on 2014-04-04
	roll  *= 180.0f / PI;


    if(serialDebug)
    {
        Serial.println("++++++++++++ RESULT RAW - YAW , PITCH , ROLL");
        Serial.println(yaw);
        Serial.println(pitch);
        Serial.println(roll);
        Serial.println("====================================================");
        Serial.println("====================================================");
     
    }

}

void MadgwickQuaternion::MadgwickQuaternionUpdate(float ax, float ay, float az, float gx, float gy, float gz, float mx, float my, float mz)
{
    if(serialDebug)
    {
        Serial.println("+++++++++++ MadgwickQuaternionUpdate inputs in radians");
        Serial.println(ax);
        Serial.println(ay);
        Serial.println(az);
        Serial.println(gx);
        Serial.println(gy);
        Serial.println(gz);
        Serial.println(mx);
        Serial.println(my);
        Serial.println(mz);
    }

    float q1 = q[0], q2 = q[1], q3 = q[2], q4 = q[3];   // short name local variable for readability
    float norm;
    float hx, hy, _2bx, _2bz;
    float s1, s2, s3, s4;
    float qDot1, qDot2, qDot3, qDot4;

    // Auxiliary variables to avoid repeated arithmetic
    float _2q1mx;
    float _2q1my;
    float _2q1mz;
    float _2q2mx;
    float _4bx;
    float _4bz;
    float _2q1 = 2.0f * q1;
    float _2q2 = 2.0f * q2;
    float _2q3 = 2.0f * q3;
    float _2q4 = 2.0f * q4;
    float _2q1q3 = 2.0f * q1 * q3;
    float _2q3q4 = 2.0f * q3 * q4;
    float q1q1 = q1 * q1;
    float q1q2 = q1 * q2;
    float q1q3 = q1 * q3;
    float q1q4 = q1 * q4;
    float q2q2 = q2 * q2;
    float q2q3 = q2 * q3;
    float q2q4 = q2 * q4;
    float q3q3 = q3 * q3;
    float q3q4 = q3 * q4;
    float q4q4 = q4 * q4;

    // Normalise accelerometer measurement
    norm = sqrt(ax * ax + ay * ay + az * az);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    ax *= norm;
    ay *= norm;
    az *= norm;

    // Normalise magnetometer measurement
    norm = sqrt(mx * mx + my * my + mz * mz);
    if (norm == 0.0f) return; // handle NaN
    norm = 1.0f/norm;
    mx *= norm;
    my *= norm;
    mz *= norm;

    if(serialDebug)
    {
        Serial.println("+++++++++++ Normalise magnetometer measurement");
        Serial.println(ax);
        Serial.println(ay);
        Serial.println(az);
        Serial.println(mx);
        Serial.println(my);
        Serial.println(mz);
    }
    // Reference direction of Earth's magnetic field
    _2q1mx = 2.0f * q1 * mx;
    _2q1my = 2.0f * q1 * my;
    _2q1mz = 2.0f * q1 * mz;
    _2q2mx = 2.0f * q2 * mx;
    hx = mx * q1q1 - _2q1my * q4 + _2q1mz * q3 + mx * q2q2 + _2q2 * my * q3 + _2q2 * mz * q4 - mx * q3q3 - mx * q4q4;
    hy = _2q1mx * q4 + my * q1q1 - _2q1mz * q2 + _2q2mx * q3 - my * q2q2 + my * q3q3 + _2q3 * mz * q4 - my * q4q4;
    _2bx = sqrt(hx * hx + hy * hy);
    _2bz = -_2q1mx * q3 + _2q1my * q2 + mz * q1q1 + _2q2mx * q4 - mz * q2q2 + _2q3 * my * q4 - mz * q3q3 + mz * q4q4;
    _4bx = 2.0f * _2bx;
    _4bz = 2.0f * _2bz;

    if(serialDebug)
    {
        Serial.println("+++++++++++ Reference direction of Earth's magnetic field");
        Serial.println(_2bx);
        Serial.println(_2bz);
        Serial.println(_4bx);
        Serial.println(_4bz);
    }

    // Gradient decent algorithm corrective step
    s1 = -_2q3 * (2.0f * q2q4 - _2q1q3 - ax) + _2q2 * (2.0f * q1q2 + _2q3q4 - ay) - _2bz * q3 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q4 + _2bz * q2) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q3 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s2 = _2q4 * (2.0f * q2q4 - _2q1q3 - ax) + _2q1 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q2 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + _2bz * q4 * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q3 + _2bz * q1) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q4 - _4bz * q2) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s3 = -_2q1 * (2.0f * q2q4 - _2q1q3 - ax) + _2q4 * (2.0f * q1q2 + _2q3q4 - ay) - 4.0f * q3 * (1.0f - 2.0f * q2q2 - 2.0f * q3q3 - az) + (-_4bx * q3 - _2bz * q1) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (_2bx * q2 + _2bz * q4) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + (_2bx * q1 - _4bz * q3) * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);
    s4 = _2q2 * (2.0f * q2q4 - _2q1q3 - ax) + _2q3 * (2.0f * q1q2 + _2q3q4 - ay) + (-_4bx * q4 + _2bz * q2) * (_2bx * (0.5f - q3q3 - q4q4) + _2bz * (q2q4 - q1q3) - mx) + (-_2bx * q1 + _2bz * q3) * (_2bx * (q2q3 - q1q4) + _2bz * (q1q2 + q3q4) - my) + _2bx * q2 * (_2bx * (q1q3 + q2q4) + _2bz * (0.5f - q2q2 - q3q3) - mz);

    if(serialDebug)
    {
        Serial.println("+++++++++++ Gradient decent algorithm corrective step");
        Serial.println(s1);
        Serial.println(s2);
        Serial.println(s3);
        Serial.println(s4);
    }

    norm = sqrt(s1 * s1 + s2 * s2 + s3 * s3 + s4 * s4);    // normalise step magnitude
    norm = 1.0f/norm;
    s1 *= norm;
    s2 *= norm;
    s3 *= norm;
    s4 *= norm;

    if(serialDebug)
    {
        Serial.println("+++++++++++ Gnormalise step magnitude");
        Serial.println(s1);
        Serial.println(s2);
        Serial.println(s3);
        Serial.println(s4);
    }

    // Compute rate of change of quaternion
    qDot1 = 0.5f * (-q2 * gx - q3 * gy - q4 * gz) - beta * s1;
    qDot2 = 0.5f * (q1 * gx + q3 * gz - q4 * gy) - beta * s2;
    qDot3 = 0.5f * (q1 * gy - q2 * gz + q4 * gx) - beta * s3;
    qDot4 = 0.5f * (q1 * gz + q2 * gy - q3 * gx) - beta * s4;

    if(serialDebug)
    {
        Serial.println("+++++++++++ Compute rate of change of quaternion");
        Serial.println(qDot1);
        Serial.println(qDot2);
        Serial.println(qDot3);
        Serial.println(qDot4);
    }

    // Integrate to yield quaternion
    q1 += qDot1 * deltat;
    q2 += qDot2 * deltat;
    q3 += qDot3 * deltat;
    q4 += qDot4 * deltat;

    if(serialDebug)
    {
        Serial.println("+++++++++++ Integrate to yield quaternion");
        Serial.println(q1);
        Serial.println(q2);
        Serial.println(q3);
        Serial.println(q4);
    }

    norm = sqrt(q1 * q1 + q2 * q2 + q3 * q3 + q4 * q4);    // normalise quaternion
    norm = 1.0f/norm;
    q[0] = q1 * norm;
    q[1] = q2 * norm;
    q[2] = q3 * norm;
    q[3] = q4 * norm;

    if(serialDebug)
    {
        Serial.println("+++++++++++ normalise quaternion 1");
        Serial.println(q[0]);
        Serial.println(q[1]);
        Serial.println(q[2]);
       Serial.println(q[3]);
    }
    

}

float MadgwickQuaternion::getPitch()
{
	return pitch;		
}

float MadgwickQuaternion::getYaw()
{
	return yaw;		
}

float MadgwickQuaternion::getRoll()
{
	return roll;		
}

void MadgwickQuaternion::setDeltat(float deltat_time)
{
    deltat = deltat_time;   
}
void MadgwickQuaternion::setSerialDebug(bool debug)
{
    serialDebug = debug;
}

