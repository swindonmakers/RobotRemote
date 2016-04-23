/*
 * Robot Remote - Serial Client
 * 
 * Simple serial client that pushes data back out over serial
 * 
 */

#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>   // https://github.com/Links2004/arduinoWebSockets/
#include <Hash.h>
#include <RobotWifi.h>
#include <Ticker.h>

#define PIN_LED 2
Ticker led;

RobotWifi robotWifi;
WebSocketsClient webSocket;

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

void webSocketEvent(WStype_t type, uint8_t * payload, size_t len) 
{
    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[WSc]Disconnected\n");
            break;
            
        case WStype_CONNECTED:
            Serial.printf("[WSc]Connected\n");
            break;
            
        case WStype_TEXT:
            Serial.printf("[WSc]%s\n", payload);
            break;
            
        case WStype_BIN:
            Serial.printf("[WSc] get binary length: %u\n", len);
            hexdump(payload, len);
            break;
    }
}

void setup() {
  Serial.begin(9600);
  Serial.setDebugOutput(false);
  Serial.println();
  Serial.println();
  Serial.println("Robot Remote - Serial Client");
  
  pinMode(PIN_LED, OUTPUT);
  ledBlink(500);
  
  robotWifi.join();
  Serial.println("Connected to wifi");
  
  webSocket.begin("192.168.4.1", 81);
  webSocket.onEvent(webSocketEvent);

  ledOn();
}

void loop() {
    
}
