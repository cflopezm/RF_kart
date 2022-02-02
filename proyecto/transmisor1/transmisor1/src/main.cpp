#include <Arduino.h>

// ------- parte de antena nRF24L01 -------

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

const int ce = 7;
const int csn = 8;

RF24 radio(ce, csn);

uint8_t address[][6] = {"1node","2node"}; // address as a path instead identifiying device


// we need a variable to uniquely identify which address this radio will use to transmit
bool radioNumber = 1; // 0 uses address[0] to transmmit, 1 uses address[1] to transmit

bool role = false; // false: RX ; true: TX

float payload = 0.0;

/* -------------------------------------------------------------------- */

// ------- Parte de encoder - sensor -------
int pinA = 16; // pin D0 - CLK
int pinB = 5;  // pin D1 - DT
int encoderPosCount = 0; //conteo posicion (-) izq (+) der

String stateEncoder, rfOutput;
int pinALast, aVal;
boolean bCW; // Establece sentido de giro
/* -------------------------------------------------------------------- */

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  pinMode(pinA, INPUT);
  pinMode(pinB, INPUT);
  pinALast = digitalRead(pinA);

  // ------- parte de antena nRF24L01 -------

  while (!Serial){// wait to ensure access to serial over USB
    
  }

  // initialize the transciever on the SPI bus
  if(!radio.begin()){
    Serial.println("Module nRF24L01 is not responding!!! D:");
    while(1){} // infinite loop
  }

  //print example
  Serial.println("Printing example...");

  // To set the radioNumber via the serial monitor on startup
  Serial.println(F("Which radio is this? Enter '0' or '1'. Defaults to '0' "));
  while(!Serial.available()){ // wait for user input     
  }

  char input = Serial.parseInt();
  radioNumber = input == 1;

  Serial.print("radioNumber = ");
  Serial.println((int)radioNumber);

  //
  Serial.println(F("*** press 'T' to begin transmitting to the other node"));

  radio.setPALevel(RF24_PA_LOW);

  radio.setPayloadSize(sizeof(payload));

  radio.openReadingPipe(1, address[!radioNumber]);

  if(role){
    radio.stopListening(); // put radio in TX mode
  }else{
    radio.startListening(); // put radio in RX mode
  }

}

void loop() {
  // put your main code here, to run repeatedly:

  aVal = digitalRead(pinA);

  if(aVal != pinALast){
    if(digitalRead(pinB) != aVal ){
      encoderPosCount++;
      bCW = true;
    }else{
      bCW = false;
      encoderPosCount--;
    }

    Serial.print("Rotated: ");
    if(bCW){
      stateEncoder = "CW";
    }else{
      stateEncoder = "CCW";
    }
    rfOutput = "Rotated: " + String(stateEncoder) + " " + "Encoder position: " + String(encoderPosCount);
    //Serial.println(rfOutput);
    
    //Serial.println(encoderPosCount);

    if(role){
      // device is a TX node

      unsigned long start_timer = micros();
      bool report = radio.write(&payload, sizeof(float));
      unsigned long end_timer = micros();

      if(report){

      Serial.print(F("Transmission successful! "));          // payload was delivered
      Serial.print(F("Time to transmit = "));
      Serial.print(end_timer - start_timer);                 // print the timer result
      Serial.print(F(" us. Sent: "));
      Serial.println(payload);                               // print payload sent
      payload += 0.01;                                       // increment float payload
    } else {
      Serial.println(F("Transmission failed or timed out")); // payload was not delivered
      }

      delay(500);



    }else{

    uint8_t pipe;
    if (radio.available(&pipe)) {             // is there a payload? get the pipe number that recieved it
      uint8_t bytes = radio.getPayloadSize(); // get the size of the payload
      radio.read(&payload, bytes);            // fetch payload from FIFO
      Serial.print(F("Received "));
      Serial.print(bytes);                    // print the size of the payload
      Serial.print(F(" bytes on pipe "));
      Serial.print(pipe);                     // print the pipe number
      Serial.print(F(": "));
      Serial.println(payload);                // print the payload's value
    }

    }

  }

  pinALast = aVal;
}