#include <Arduino.h>

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h> 

Adafruit_MPU6050 mpu;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while(!Serial)
    delay(10);
  
  Serial.println("IMU MPU 6050 test...");

  if(!mpu.begin()){
    Serial.println("Failed to find MPU 6050");
    while(true){
      delay(10);
    }
  }

  Serial.println("MPU 6050 found!!!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");

  switch (mpu.getAccelerometerRange())
  {
  case MPU6050_RANGE_2_G:
    Serial.println("+- 2G");
    break;

  case MPU6050_RANGE_4_G:
    Serial.print("+- 4G");
    break;
    
  case MPU6050_RANGE_8_G:
    Serial.print("+- 8G");
  
  case MPU6050_RANGE_16_G:
    Serial.print("+- 16G");
  }

  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.println("Gyro range set to: ");

  switch (mpu.getGyroRange())
  {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 DEG/s");
    break;

  case MPU6050_RANGE_500_DEG:
    Serial.print("+- 500 DEG/s");
    break;
    
  case MPU6050_RANGE_1000_DEG:
    Serial.print("+- 1000 DEG/s");
  
  case MPU6050_RANGE_2000_DEG:
    Serial.print("+- 1000 DEG/s");
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  
  switch (mpu.getFilterBandwidth())
  {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;

  case MPU6050_BAND_184_HZ:
    Serial.print("184 Hz");
    break;
    
  case MPU6050_BAND_94_HZ:
    Serial.print("94 Hz");
    break;

  case MPU6050_BAND_44_HZ:
    Serial.print("44 Hz");
    break;

  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  
  case MPU6050_BAND_10_HZ:
    Serial.print("10 Hz");
    break;

  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // print values
  Serial.print("\nAccelation X: ");
  Serial.print(a.acceleration.x);
  Serial.print("\tAccelation Y: ");
  Serial.print(a.acceleration.y);
  Serial.print("\tAccelation Z: ");
  Serial.print(a.acceleration.z);
  Serial.print(" m2/s");;

  Serial.print("\nRotation X: ");
  Serial.print(g.gyro.x);
  Serial.print("\tRotation Y: ");
  Serial.print(g.gyro.y);
  Serial.print("\tRotation Z: ");
  Serial.print(g.gyro.z);
  Serial.println(" rad/s");

  delay(500);
}