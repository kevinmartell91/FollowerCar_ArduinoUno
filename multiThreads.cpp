#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Magnetometer.h"
#include "BarometricPressure.h"
#include "MadgwickQuaternion.h"

// ThreadController that will controll all threads of sensors
ThreadController controll = ThreadController();

Thread* allSensorResultsThread = new Thread();
Thread* accelerometroThread = new Thread();
Thread* gyroscopeThread = new Thread();
Thread* magnetometerThread =  new Thread();
Thread* barometricPressureThread = new Thread();
Thread* madgwickQuaternionThread = new Thread();

// Instances of Sensors
Accelerometer* accelerometer = new Accelerometer();
Gyroscope* gyroscope = new Gyroscope();
Magnetometer* magnetometer = new Magnetometer();
BarometricPressure* barometricPressure =  new BarometricPressure();
// Intance of madgwick algorithm
MadgwickQuaternion* madgwickQuaternion = new MadgwickQuaternion();


// callback for myThread
void accelerometerCallback(){
  accelerometer->calculateAccelerometerValues();
  //Serial.println(millis());
}

void gyroscopeCallback(){
  gyroscope->calculateGyroscopeValues();
  
}

void magnetometerCallback(){
  magnetometer->calculateMagnetometerValues();

}

void barometricPressureCallback(){

  barometricPressure->calculateBarometricPressureValues();

}

void madgwickQuaternionCallback(){
  madgwickQuaternion->calculateMadgwickQuaternionValues(
    accelerometer->getX(),
    accelerometer->getY(),
    accelerometer->getZ(),
    gyroscope->getX(),
    gyroscope->getY(),
    gyroscope->getZ(),
    magnetometer->getX(),
    magnetometer->getY(),
    magnetometer->getZ()
  );

  //Serial.println("===========  madgwick filter  =========");
  Serial.print("MadgwickQuaternion ( ");
  Serial.print(madgwickQuaternion->getPitch()); Serial.print(" , ");
  Serial.print(madgwickQuaternion->getYaw()); Serial.print(" , ");
  Serial.print(madgwickQuaternion->getRoll()); Serial.print(" ).");
  Serial.println();
  Serial.println("-----------------------------------------");

}

// callback for hisThread
void allSensorResultsCallback(){
  //Serial.println("===========  accelerometer  =========");
  Serial.print("Accelerometer ( ");
  Serial.print(accelerometer->getX()); Serial.print(" , ");
  Serial.print(accelerometer->getY()); Serial.print(" , ");
  Serial.print(accelerometer->getZ()); Serial.print(" ).");
  Serial.println();

  //Serial.println("============   gyroscope    ==========");
  Serial.print("Gyroscope ( ");
  Serial.print(gyroscope->getX());Serial.print(" , ");
  Serial.print(gyroscope->getY());Serial.print(" , ");
  Serial.print(gyroscope->getZ());Serial.print(" ).");
  Serial.println();

  //Serial.println("============= magnetometer  =========");
  Serial.print("Magnetometer ( ");
  Serial.print(magnetometer->getX());Serial.print(" , ");
  Serial.print(magnetometer->getY());Serial.print(" , ");
  Serial.print(magnetometer->getZ());Serial.print(" ).");
  Serial.println();

  //Serial.println("============= barometricPressure  =========");
  Serial.println("barometricPressure");
  Serial.print("  Temperatura: ");
  // Mostra a temperatura com 2 casas decimais
  Serial.print(barometricPressure->getTemperature(), 2); 
  Serial.println(" C");

  Serial.print("  Pressao: ");
  Serial.print(barometricPressure->getPressure()); 
  Serial.println(" Pa");

  Serial.print("  Atmosfera padrao : ");
  // Mostra o valor com 4 casas decimais
  Serial.println(barometricPressure->getAtmosphere(), 4); //display 4 decimal places

  Serial.print("  Altitude: ");
  // Mostra o valor com 2 casas decimais
  Serial.print(barometricPressure->getAltitud(), 2); 
  Serial.println(" M");
  Serial.println();
  Serial.println("---------------------------------------------");
  Serial.println();
}

void setup(){

  Wire.begin();
  Serial.begin(9600);
  delay(100);

  accelerometer->setupADXL345();
  gyroscope->setupL3G4200D(2000); // Configure L3G4200  - 250, 500 or 2000 deg/sec
  magnetometer->setupHMC5883();
  barometricPressure->setupBMP085Calibration();

  delay(200);


  // Configure accelerometroThread
  accelerometroThread->onRun(accelerometerCallback);
  accelerometroThread->setInterval(500);

  // Configure gyroscopeThread
  gyroscopeThread->onRun(gyroscopeCallback);
  gyroscopeThread->setInterval(500);

  // Configurate magnetometerThread
  magnetometerThread->onRun(magnetometerCallback);
  magnetometerThread->setInterval(500);

  // Configurate barometricPressureThread
  barometricPressureThread->onRun(barometricPressureCallback);
  barometricPressureThread->setInterval(500);

  // Configure allSensorResultsThread
  allSensorResultsThread->onRun(allSensorResultsCallback);
  allSensorResultsThread->setInterval(500);

  // Configurate Madgwick algorithm
  madgwickQuaternionThread->onRun(madgwickQuaternionCallback);
  madgwickQuaternionThread->setInterval(500);


  // Adds both threads to the sensor controllers of threads
  controll.add(accelerometroThread);
  controll.add(gyroscopeThread); // & to pass the pointer to it
  controll.add(magnetometerThread);
  controll.add(barometricPressureThread);
  controll.add(madgwickQuaternionThread);
  controll.add(allSensorResultsThread);
}

void loop(){
  // run ThreadController
  // this will check every thread inside ThreadController,
  // if it should run. If yes, he will run it;
  controll.run();
  delay(1000);

  // Rest of code
 
}