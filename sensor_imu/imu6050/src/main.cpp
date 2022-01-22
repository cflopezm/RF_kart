#include <Arduino.h>

#include <Wire.h> 

#define MPU 0x68

// Factores de conversion
#define A_R 16384.0
#define G_R 131.0

// conversion de rad -> deg
#define RAD2DEG 57.295779

// valores de MPU 6050 de 16 bits

int16_t ax, ay, az, gx, gy, gz;

// angulos
float Acc[2];
float Gy[3];
float Angle[3];

String values;
long time_prev;
float dt;


void setup() {
  // put your setup code here, to run once:

  Wire.begin(4,5); // pin D2=SDA , D1=SCL
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);
  Wire.write(0);
  Wire.endTransmission(true);
  Serial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:

  Wire.beginTransmission(MPU);
  Wire.write(0x3B); // el registro 0x3B es ax (acc x)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 6, true); // se piden 6 registros desde 0x3B

  ax = Wire.read()<<8|Wire.read();
  ay = Wire.read()<<8|Wire.read();
  az = Wire.read()<<8|Wire.read();

  // A partir de los valores del acelerometro, se calculan los angulos Y, X
  //respectivamente, con la formula de la tangente.
  Acc[1] = atan(-1*(ax/A_R)/sqrt(pow((ay/A_R),2) + pow((az/A_R),2)))*RAD2DEG;
  Acc[0] = atan((ay/A_R)/sqrt(pow((ax/A_R),2) + pow((az/A_R),2)))*RAD2DEG;

  //Leer los valores del Giroscopio
   Wire.beginTransmission(MPU);
   Wire.write(0x43);
   Wire.endTransmission(false);
   Wire.requestFrom(MPU,6,true);   //A partir del 0x43, se piden 6 registros
   gx = Wire.read()<<8|Wire.read(); //Cada valor ocupa 2 registros
   gy = Wire.read()<<8|Wire.read();
   gz = Wire.read()<<8|Wire.read();
 
   //Calculo del angulo del Giroscopio
   Gy[0] = gx/G_R;
   Gy[1] = gy/G_R;
   Gy[2] = gz/G_R;

   dt = (millis() - time_prev) / 1000.0;
   time_prev = millis();
 
   //Aplicar el Filtro Complementario
   Angle[0] = 0.98 *(Angle[0]+Gy[0]*dt) + 0.02*Acc[0];
   Angle[1] = 0.98 *(Angle[1]+Gy[1]*dt) + 0.02*Acc[1];

   //Integración respecto del tiempo paras calcular el YAW
   Angle[2] = Angle[2]+Gy[2]*dt;
 
   //Mostrar los valores por consola
   values = "90, " +String(Angle[0]) + "," + String(Angle[1]) + "," + String(Angle[2]) + ", -90";
   Serial.println(values);
   
   delay(10);
 
}