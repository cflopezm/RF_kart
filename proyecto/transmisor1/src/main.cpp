#include <Arduino.h>

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//pins esp32 wroover
const int trigPin = 5;
const int echoPin = 18;

//Defines variables

long duration;
int distance;


void setup() {
  // put your setup code here, to run once:

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(112500);

}

void loop() {
  // put your main code here, to run repeatedly:
  //Clear the trigPin
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);

  //Sets the trigPin on high for 10 ms
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  //Reads the echo pin. Return the sound wave travel time in mircroseconds
  duration = pulseIn(echoPin, HIGH);
  distance = (duration*0.034)/2;

  //Print in serial monitor
  delay(500);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
}