#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <SPI.h>

#define SOUND_SPEED 0.034


// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0xFF, 0x84, 0xF3, 0xEB, 0xC9, 0xE0};

/* ----------------- HC-SR04 ----------------- */

// echo and trigger from ultrasonic - ESP32 
const int trigPin = 18;
const int echoPin = 5;

//Defines variables
long duration;
int distance;

/* ----------------- END HC-SR04 ----------------- */

/* ----------------- MLX9061 Temperatura ----------------- */
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
float tempEnv;
float tempObj;


/* ----------------- END MLX9061 ----------------- */

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  char a[32];
  int b;   // distancia en cm
  float c; // temperatura en °C
  //bool d;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  // set pin HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
 
  // begin MLX
  mlx.begin();

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {
  // distance sensor data
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration * SOUND_SPEED)/2;

  // temperature sensor data
  tempEnv = mlx.readAmbientTempC();
  tempObj = mlx.readObjectTempC();

  // Set values to send
  strcpy(myData.a, "Envio exitoso!!!");
  myData.b = distance;
  myData.c = tempObj;
  //myData.d = false;
  
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
   
  if (result == ESP_OK) {
    Serial.print("Distance: ");
    Serial.println(distance);
    Serial.print("Temperature: ");
    Serial.println(tempObj);
    Serial.println("\tSent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  
  delay(500);
}