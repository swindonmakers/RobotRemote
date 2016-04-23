/*
 * RobotRemote - SerialClient
 * 
 * Simple serial client that pushes data back out over serial
 * 
 */
 
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WebSocketsClient.h>   // https://github.com/Links2004/arduinoWebSockets/
#include <Hash.h>
#include <RobotWifi.h>

RobotWifi robotWifi;
WebSocketsClient webSocket;

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
    Serial.begin(115200);
    Serial.setDebugOutput(false);
    Serial.println();
    Serial.println();
    Serial.println("Robot Remote - Serial Client");

    robotWifi.join();
    Serial.println("Connected to wifi");

    webSocket.begin("192.168.4.1", 81);
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    
}
