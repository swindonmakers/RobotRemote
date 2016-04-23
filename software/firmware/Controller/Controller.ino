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

RobotWifi robotWifi;
WebSocketsServer webSocket = WebSocketsServer(81);


void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t len)
{
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED:
      {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] Connected from %s url: %s\n", num, ip.toString().c_str(), payload);
  
        // send message to client
        webSocket.sendTXT(num, "Connected to Serial on " + WiFi.softAPIP().toString() + "\n");
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
      Serial.printf("[%u] get binary length: %u\n", num, len);
      hexdump(payload, len);
      break;
  }
}

void setup() 
{
  Serial.begin(115200);
  Serial.setDebugOutput(false);
  Serial.println();
  Serial.println();
  Serial.println("Robot Remote - Controller");
  Serial.setTimeout(200);

  // Setup as the Robot Remote host.
  robotWifi.host();
  Serial.println("Wifi AP started");

  // Start a websocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() 
{
  // Read input from serial and post out to websocket clients
  String line = Serial.readStringUntil('\n');
  if (line.length() > 0) {
    webSocket.broadcastTXT("[js]" + line);
  }
}

