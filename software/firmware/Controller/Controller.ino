/*
 * Robot Remote - Controller
 * 
 * Firmware for a Robot Remote controller
 * 
 */
 
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WebSocketsServer.h>   // https://github.com/Links2004/arduinoWebSockets/
#include <Hash.h>
#include <Ticker.h>
#include <RobotWifi.h>
#include "Webpages.h"

#define PIN_LED 2
Ticker led;

RobotWifi robotWifi;
ESP8266WebServer webserver(80);
WebSocketsServer webSocket = WebSocketsServer(81);

void ledTick()
{
  digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}

void ledBlink(int intervalMs)
{
  led.attach_ms(intervalMs, ledTick);
}

void ledOff()
{
  led.detach();
  digitalWrite(PIN_LED, LOW);
}

void ledOn()
{
  led.detach();
  digitalWrite(PIN_LED, HIGH);
}

static void handleRoot()
{
  webserver.send(200, F("text/html"), pageWebSocketMonitor);
}

static void handleNotFound()
{
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += webserver.uri();
  message += "\nMethod: ";
  message += (webserver.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += webserver.args();
  message += "\n";
  for (uint8_t i=0; i<webserver.args(); i++){
    message += " " + webserver.argName(i) + ": " + webserver.arg(i) + "\n";
  }
  webserver.send(404, F("text/plain"), message);
}

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
  
  pinMode(PIN_LED, OUTPUT);
  ledBlink(500);

  // Setup as the Robot Remote host.
  robotWifi.host();
  Serial.println("Wifi AP started");

  // Start web server
  webserver.on("/", handleRoot);
  webserver.onNotFound(handleNotFound);
  webserver.begin();
  Serial.println("Webserver started");

  // Start a websocket server
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("Websocket server started");

  ledOn();
}

void loop() 
{
  webserver.handleClient();
  
  // Read input from serial and post out to websocket clients
  String line = Serial.readStringUntil('\n');
  if (line.length() > 0) {
    ledOff();
    webSocket.broadcastTXT("[js]" + line);
    ledOn();
  }
}

