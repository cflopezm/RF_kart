#include <Arduino.h>


#include <ESP8266WiFi.h>
#include <espnow.h>

// dependencias oled
#include <Wire.h>
#include <OLED_I2C.h>

// Oled definition

OLED  screen(4, 5);
extern uint8_t SmallFont[];// Selecciono el tamaño de fuente (Small o Big)
extern uint8_t MediumNumbers[];// Selecciono el tamaño de fuente para los numeros

//int distancia = 0;
// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    //String d;
    bool e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Callback function that will be executed when data is received
void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {
  memcpy(&myData, incomingData, sizeof(myData));
  //distancia = myData.c;
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Char: ");
  Serial.println(myData.a);
  Serial.print("Int: ");
  Serial.println(myData.b);
  Serial.print("Distancia: ");
  Serial.println(myData.c);
  // print value on OLED

  screen.print("Nivel de gasolina:", 0, 0);
  screen.printNumF(myData.c, 2, 0, 15);
  screen.print("cm", 50, 15);
  screen.update();

  // fin oled

  /*Serial.print("String: ");
  Serial.println(myData.d);*/
  Serial.print("Bool: ");
  Serial.println(myData.e);
  Serial.println();
}

 
void setup() {
  // OLED welcome message

  screen.begin();//inicializa el display OLED
  screen.setFont(SmallFont);//seteo el tamaño de la fuente
  /*screen.print("Bienvenido kart UN", CENTER, 32);//imprime la frase entre comillas
  screen.update();// actualiza la pantalla haciendo lo anterior
  delay(2000);*/
  screen.clrScr(); // borra la pantalla
  
  // Initialize Serial Monitor
  Serial.begin(115200);
  
   
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  esp_now_register_recv_cb(OnDataRecv);

  

    
  
}

void loop() {
  

}