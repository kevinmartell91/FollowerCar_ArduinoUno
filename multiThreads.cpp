#include <Thread.h>
#include <ThreadController.h>
#include <Wire.h>
#include "Accelerometer.h"
#include "Gyroscope.h"
#include "Magnetometer.h"
#include "BarometricPressure.h"
#include "MadgwickQuaternion.h"

#define SENSOR_INTERVAL 200
#define FILTER_INTERVAL 200
#define SERIAL_DEBUGER false

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

// Global variables
   float ax ;
   float ay ;
   float az ;
   float gx ;
   float gy ;
   float gz ;
   float mx ;
   float my ;
   float mz ;

   uint32_t lastUpdate = 0; // used to calculate integration interval
   uint32_t Now = 0;        // used to calculate integration interval
   float deltat = 0;
   


// Intance of madgwick algorithm
MadgwickQuaternion* madgwickQuaternion = new MadgwickQuaternion();

void accelerometerCallback(){
  accelerometer->calculateAccelerometerValues();
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

void allSensorResultsCallback(){

  ax = accelerometer->getX();
  ay = accelerometer->getY();
  az = accelerometer->getZ();
  gx = gyroscope->getX();
  gy = gyroscope->getY();
  gz = gyroscope->getZ();
  mx = magnetometer->getX();
  my = magnetometer->getY();
  mz = magnetometer->getZ();

  //Serial.println("===========  accelerometer  =========");
  Serial.print("Accelerometer ( ");
  Serial.print(ax); Serial.print(" , ");
  Serial.print(ay); Serial.print(" , ");
  Serial.print(az); Serial.print(" ).");
  Serial.println();

  //Serial.println("============   gyroscope    ==========");
  Serial.print("Gyroscope ( ");
  Serial.print(gx);Serial.print(" , ");
  Serial.print(gy);Serial.print(" , ");
  Serial.print(gz);Serial.print(" ).");
  Serial.println();

  //Serial.println("============= magnetometer  =========");
  Serial.print("Magnetometer ( ");
  Serial.print(mx);Serial.print(" , ");
  Serial.print(my);Serial.print(" , ");
  Serial.print(mz);Serial.print(" ).");
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

void madgwickQuaternionCallback(){
  
  ax = accelerometer->getX();
  ay = accelerometer->getY();
  az = accelerometer->getZ();
  gx = gyroscope->getX();
  gy = gyroscope->getY();
  gz = gyroscope->getZ();
  mx = magnetometer->getX();
  my = magnetometer->getY();
  mz = magnetometer->getZ();

  //Serial.println("===========  accelerometer  =========");
  Serial.print("Accelerometer ( ");
  Serial.print(ax); Serial.print(" , ");
  Serial.print(ay); Serial.print(" , ");
  Serial.print(az); Serial.print(" ).");
  Serial.println();

  //Serial.println("============   gyroscope    ==========");
  Serial.print("Gyroscope ( ");
  Serial.print(gx);Serial.print(" , ");
  Serial.print(gy);Serial.print(" , ");
  Serial.print(gz);Serial.print(" ).");
  Serial.println();

  //Serial.println("============= magnetometer  =========");
  Serial.print("Magnetometer ( ");
  Serial.print(mx);Serial.print(" , ");
  Serial.print(my);Serial.print(" , ");
  Serial.print(mz);Serial.print(" ).");
  Serial.println();

  Now = micros();
  deltat = ((Now - lastUpdate)/1000000.0f); // set integration time by time elapsed since last filter update
  lastUpdate = Now;

  //Serial.print(" Deltat => ");Serial.print(deltat); 

  madgwickQuaternion->setDeltat(deltat);
  madgwickQuaternion->setSerialDebug(SERIAL_DEBUGER);

  madgwickQuaternion->calculateMadgwickQuaternionValues(
    //300,300,300,300,300,300,300,300,300
    ax,ay,az,gx,gy,gz,mx,my,mz
  );

  //Serial.println("===========  madgwick filter  =========");
  Serial.print("MadgwickQuaternion ( ");
  Serial.print(madgwickQuaternion->getPitch()); Serial.print(" , ");
  Serial.print(madgwickQuaternion->getYaw()); Serial.print(" , ");
  Serial.print(madgwickQuaternion->getRoll()); Serial.print(" ).");
  Serial.println();
  Serial.println("-----------------------------------------------------");
  Serial.println("-----------------------------------------------------");

}

void setup(){

  Wire.begin();
  Serial.begin(9600);
  delay(100);

  accelerometer->setupADXL345();
  gyroscope->setupL3G4200D(250); // Configure L3G4200  - 250, 500 or 2000 deg/sec
  magnetometer->setupHMC5883();
  barometricPressure->setupBMP085Calibration();

  delay(200);

  // Configure accelerometroThread
  accelerometroThread->onRun(accelerometerCallback);
  accelerometroThread->setInterval(SENSOR_INTERVAL);

  // Configure gyroscopeThread
  gyroscopeThread->onRun(gyroscopeCallback);
  gyroscopeThread->setInterval(SENSOR_INTERVAL);

  // Configurate magnetometerThread
  magnetometerThread->onRun(magnetometerCallback);
  magnetometerThread->setInterval(SENSOR_INTERVAL);

  // Configurate barometricPressureThread
  barometricPressureThread->onRun(barometricPressureCallback);
  barometricPressureThread->setInterval(SENSOR_INTERVAL);

  // Configure allSensorResultsThread
  // allSensorResultsThread->onRun(allSensorResultsCallback);
  // allSensorResultsThread->setInterval(SENSOR_INTERVAL);

  // Configurate Madgwick algorithm
  madgwickQuaternionThread->onRun(madgwickQuaternionCallback);
  madgwickQuaternionThread->setInterval(FILTER_INTERVAL);


  // Adds both threads to the sensor controllers of threads
  controll.add(accelerometroThread);
  controll.add(gyroscopeThread);
  controll.add(magnetometerThread);
  controll.add(barometricPressureThread);
  controll.add(madgwickQuaternionThread);
  //controll.add(allSensorResultsThread);
}

void loop(){
  // run ThreadController
  // this will check every thread inside ThreadController,
  // if it should run. If yes, he will run it;
  controll.run();
  // Rest of code
 
}