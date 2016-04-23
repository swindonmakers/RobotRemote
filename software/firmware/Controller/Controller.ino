/*
 * RobotRemote - Controller
 * 
 * Firmware for a RobotRemote controller
 * 
 */
 
#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <WebSocketsServer.h>   // https://github.com/Links2004/arduinoWebSockets/
#include <Hash.h>

#include <ESP8266WebServer.h>
#include <RobotWifi.h>

WebSocketsServer webSocket = WebSocketsServer(81);
RobotWifi robotWifi;

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t lenght) {

  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %s url: %s\n", num, ip.toString().c_str(), payload);

        // send message to client
        webSocket.sendTXT(num, "Connected to Serial on " + WiFi.localIP().toString() + "\n");
      }
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get Text: %s\n", num, payload);

      // send message to client
      // webSocket.sendTXT(num, "message here");

      // send data to all connected clients
      // webSocket.broadcastTXT("message here");
      break;
    case WStype_BIN:
      Serial.printf("[%u] get binary lenght: %u\n", num, lenght);
      hexdump(payload, lenght);

      // send message to client
      // webSocket.sendBIN(num, payload, lenght);
      break;
  }

}

void setup() {
  // Serial.begin(921600);
  Serial.begin(115200);

  //Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();
  
  //RobotWifi
  robotWifi.host();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  Serial.println("START MIRRORING SERIAL");
}

void loop() {
  Serial.setTimeout(200);
  String line = Serial.readStringUntil('\n');
  if (line.length() > 0) {
    // add back line ending
    line += '\n';
    webSocket.broadcastTXT(line);
    Serial.print(line);
  }
  //webSocket.loop();
}

