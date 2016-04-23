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

#define USE_SERIAL Serial

void webSocketEvent(WStype_t type, uint8_t * payload, size_t lenght) {


    switch(type) {
        case WStype_DISCONNECTED:
            USE_SERIAL.printf("[WSc] Disconnected!\n");
            break;
        case WStype_CONNECTED:
            {
                USE_SERIAL.printf("[WSc] Connected to url: %s\n",  payload);
				
			    // send message to server when Connected
				webSocket.sendTXT("Connected");
            }
            break;
        case WStype_TEXT:
            USE_SERIAL.printf("[WSc] get text: %s\n", payload);

			// send message to server
			// webSocket.sendTXT("message here");
            break;
        case WStype_BIN:
            USE_SERIAL.printf("[WSc] get binary lenght: %u\n", lenght);
            hexdump(payload, lenght);

            // send data to server
            // webSocket.sendBIN(payload, lenght);
            break;
    }

}

void setup() {
    USE_SERIAL.begin(115200);

    //Serial.setDebugOutput(true);
    USE_SERIAL.setDebugOutput(false);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

    robotWifi.join();

    webSocket.begin("192.168.4.1", 81);
    //webSocket.setAuthorization("user", "Password"); // HTTP Basic Authorization
    webSocket.onEvent(webSocketEvent);
}

void loop() {
    //webSocket.loop();
}
