/*

Free Speech Zone
Wifi-Connected Thermal Printer, censoring everything people say.
Copyright (c) 2016 Kamel Makhloufi. All rights reserved.

This project is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This project is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

---------------

Use the SoftwareSerial library included in the lib folder.
Platformio's SoftwareSerial doesn't allow to invert the signal, needed for the
STP-103 thermal printer

---------------

PINOUT :
GPIO12 -> STP-103 tx (20)
GPIO13 -> STP-103 rx (19)

The ESP8266 is 3.3v while the printer is 5v. You'll have to convert the levels
and regulate the voltage to power your module.

If you don't need to receive messages from the printer, a simple circuit like
http://i.stack.imgur.com/HDchC.png
on GPIO13 (tx) is enough.

You'll find +5V and GND on the voltage regulators inside the printer.

---------------

Don't forget to change your WiFi SSID & passwords and your mqtt server infos.

*/

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include "stp103.h"

#define wifi_ssid "your_ssid"
#define wifi_password "your_pass"

#define mqtt_server "mqtt_server_host"
#define mqtt_port 1883
#define mqtt_user "mqtt_user"
#define mqtt_password "mqtt_pass"
#define mqtt_topic "mqtt_topic"

WiFiClient espClient;
PubSubClient mqtt_client(espClient);

void setup_wifi() {
  delay(10);
  // Start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // Setup OTA update
  ArduinoOTA.setHostname("expression");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String message = "";
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i],HEX);
    // Replace * by block character for censoring
    if ((char)payload[i] == '*') {
      message += (char)219;
    } else {
      message += (char)payload[i];
    }
  }
  // Print message on paper roll
  printPaper(message);
  Serial.println();
}

void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!mqtt_client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (mqtt_client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      mqtt_client.subscribe("print");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqtt_client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // Wait a bit for printer to start
  delay(1000);

  // Setup debug serial
  Serial.begin(115200);

  setup_wifi();

  // Setup MQTT client
  mqtt_client.setServer(mqtt_server, mqtt_port);
  mqtt_client.setCallback(mqtt_callback);

  delay(1500);

  // Setup Printer
  setupPaperPrinter();
}

void loop() { // run over and over
  if (!mqtt_client.connected()) {
    mqtt_reconnect();
  }
  mqtt_client.loop();
  ArduinoOTA.handle();
}
