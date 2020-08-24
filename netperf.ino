/*
 File    : netperf.ino - monitor meraki gear and update ESP32 via MQTT
 Author  : Joe McManus josephmc@alumni.cmu.edu
 Version : 0.1 08/23/2020
 Copyright (C) 2020 Joe McManus

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public Licensex
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <WiFi.h>
#include <Wire.h>
#include <PubSubClient.h>
#include "Qwiic_LED_Stick.h"

// WiFi network name and password:
const char * networkName = "yourSSID";
const char * networkPswd = "yourPass";

const char *ID = "esp32";  
const char *ledOne = "ledOne";
const char *ledTwo = "ledTwo";
const char *ledThree = "ledThree";

//Using multiple Qwiic LEDs means assigning new i2c addresses
//Use this the first time:
//https://github.com/sparkfun/SparkFun_Qwiic_LED_Stick_Arduino_Library/tree/master/examples/Example10_ChangeAddress
 
LED LEDStickOne;
LED LEDStickTwo; //Starting with a new address
LED LEDStickThree; //LEDStickThree.begin(0x31)
/*
   Red 255, 0 0
   Green 0, 255, 0
   Blue 0, 0, 255
   Yellow 255,255,0
   https://www.rapidtables.com/web/color/RGB_Color.html
*/

IPAddress broker(192, 168, 128, 46);

WiFiClient wclient;
PubSubClient client(wclient); // Setup MQTT client

// Handle incomming messages from the broker
void callback(char* topic, byte* payload, unsigned int length) {
  String response;
  // Add somethign here
  //for (int i = 0; i < length; i++) {
  //  response += (char)payload[i];
  //}
  if (strcmp(topic, "ledOne") == 0) {
    Serial.print("Topic : ");
    Serial.println(topic);
    for (int i = 0; i < length; i++) {
      response += (char)payload[i];
    }
    setLED(LEDStickOne, response.toInt());
  }
  if (strcmp(topic, "ledTwo") == 0) {
    Serial.print("Topic : ");
    Serial.println(topic);
    for (int i = 0; i < length; i++) {
      response += (char)payload[i];
    }
     setLED(LEDStickTwo, response.toInt());
  }
    if (strcmp(topic, "ledThree") == 0) {
    Serial.print("Topic : ");
    Serial.println(topic);
    for (int i = 0; i < length; i++) {
      response += (char)payload[i];
    }
     setLED(LEDStickThree, response.toInt());
  }

  
}

// Connect to WiFi network
void setup_wifi() {
  Serial.print("\nConnecting to ");
  Serial.println(networkName);

  WiFi.begin(networkName, networkPswd); // Connect to network

  while (WiFi.status() != WL_CONNECTED) { // Wait for connection
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

// Reconnect to client
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(ID)) {
      client.subscribe(ledOne);
      client.subscribe(ledTwo);
      client.subscribe(ledThree);
      Serial.println("connected");

    } else {
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setLED(LED &LEDStick, int level) {
  Serial.println(level);
  if (level <= 1) {
    LEDStick.setLEDColor(1, 0, 0, 255);

  } else if (level == 2) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);

  } else if (level == 3) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0 );

  } else if (level == 4) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);

  } else if (level == 5) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 0, 255, 0);

  } else if (level == 6) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 0, 255, 0);
    LEDStick.setLEDColor(6, 0, 255, 0);

  } else if (level == 7) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 0, 255, 0);
    LEDStick.setLEDColor(6, 255, 255, 0);
    LEDStick.setLEDColor(7, 255, 255, 0);

  } else if (level == 8) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 0, 255, 0);
    LEDStick.setLEDColor(6, 255, 255, 0);
    LEDStick.setLEDColor(7, 255, 255, 0);
    LEDStick.setLEDColor(8, 255, 255, 0);

  } else if (level == 9) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 255, 255, 0);
    LEDStick.setLEDColor(6, 255, 255, 0);
    LEDStick.setLEDColor(7, 255, 255, 0);
    LEDStick.setLEDColor(8, 255, 255, 0);
    LEDStick.setLEDColor(9, 255, 0, 0);

  } else if (level >= 10) {
    LEDStick.setLEDColor(1, 0, 0, 255);
    LEDStick.setLEDColor(2, 0, 0, 255);
    LEDStick.setLEDColor(3, 0, 255, 0);
    LEDStick.setLEDColor(4, 0, 255, 0);
    LEDStick.setLEDColor(5, 255, 255, 0);
    LEDStick.setLEDColor(6, 255, 255, 0);
    LEDStick.setLEDColor(7, 255, 255, 0);
    LEDStick.setLEDColor(8, 255, 255, 0);
    LEDStick.setLEDColor(9, 255, 0, 0);
    LEDStick.setLEDColor(10, 255, 0, 0);
  }
  
  //After assigning new values, blank the old ones out 
  for ( int i = level + 1; i <= 10; i++) {
    LEDStick.setLEDColor(i, 0, 0, 0);
  }
  LEDStick.setLEDBrightness(1);

}

void setup() {
  Wire.begin();
  Serial.begin(115200); // Start serial communication at 115200 baud
  LEDStickOne.begin();
  LEDStickTwo.begin(0x29);
  LEDStickThree.begin(0x31);
  delay(100);
  setup_wifi(); // Connect to network
  client.setServer(broker, 1883);
  client.setCallback(callback);// Initialize the callback routine

}

void loop() {
  if (!client.connected())  // Reconnect if connection is lost
  {
    reconnect();
  }
  client.loop();
}
