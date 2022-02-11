# Proyecto envío de datos de sensores kart UN

El presente proyecto busca desarrollar un sistema para el envío de datos procedente de sensores, lo cuales realizan mediciones de variables de un kart. 

Para el desarrollo del proyecto se tiene contemplado medir la temperatura de la llanta de un kart y el nivel tel tanque de gasolina, por lo que se usara el sensor de distancia **HC-SR04** y el sensor de temperatura a distancia **MLX90614**.

Para el manejo de los sensores y el envío de datos se usó el microcontrolador **ESP32**, el cual cuenta con el protocolo ESP-NOW, que permite el envio de datos por radiofrecuencia entre los dispositivos de la familia ESP.


## Desarrollo del proyecto

Este proyecto usa el protocolo ESP-NOW, pero para usarlo es necesario conocer la dirección MAC del dispositivo que sera el receptor. Para obtener esa información es necesario subir un código a la tarjeta e imprimir el valor por el monitor serial.

 Si se usa el ESP32 el código es el siguiente:

```c++

#include "WiFi.h"
 
void setup(){
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
```

Si se usa el ESP8266 el código es el siguiente:

```c++
#include <ESP8266WiFi.h>

void setup(){
  Serial.begin(115200);
  Serial.println();
  Serial.print("MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){

}
```
Solo es necesario conocer el valor MAC del receptor, no del transmisor. Con este valor se puede iniciar con la programación de los nodos.

### Transmisor

El transmisor es el microcontrolador que obtiene los valores de los sensores para luego transmitirlos. En este caso se usaron 2 sensores y su conexión se especifica en la siguiente tabla


|         Sensor          | Pin sensor | pin ESP32 |
|:-----------------------:|:----------:|:---------:|
|  Ultrasonido<br>HC-SR04 |     Vcc    |     5V    |
|                         |    trig    |     18    |
|                         |    echo    |     5     |
|                         |     GND    |    GND    |
| Temperatura<br>MLX90614 |     Vin    |    3.3v   |
|                         |     GND    |    GND    |
|                         |     SCL    |     22    |
|                         |     SDA    |     21    |

### Receptor

El receptor cuenta con un pantalla OLED de 0.96" que permite visualizar los dato fuera del monitor serial. Su conexión al ESP8266 utilizado  se describe en la siguiente tabla

| Pines pantalla OLED | Pines ESP8266 |
|:-------------------:|:-------------:|
|         GND         |      GND      |
|         VDD         |       5V      |
|         SCK         |       5       |
|         SDA         |       4       |


