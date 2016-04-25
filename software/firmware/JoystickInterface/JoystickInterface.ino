/*
 * Joystick Interface
 * 
 * Runs on Arduino Pro Mini, taking input from a slightly modified analog joystick
 * with two axes and two buttons and sending out a stream of serial data representing
 * the joystick state.
 * 
 * Connections:
 * 
 *  pins 2,3 joystick buttons with pull down resistors to GND.
 *  pins A0, A1 connected to the middle (wiper) pin of the pots on the X and Y axes of the joystick
 *  
 *  pin 4 - lcd RS (pin 4)
 *  pin 5 - lcd Enable (pin 6)
 *  pin 6 - lcd Data 7 (pin 14)
 *  pin 7 - lcd Data 6 (pin 13)
 *  pin 8 - lcd Data 5 (pin 12)
 *  pin 9 - lcd Data 4 (pin 11)
 * 
 * Serial output format: "x,y,b1,b2"
 *  
 *  where 
 *    x = analogRead(x), range 0..1024
 *    y = analogRead(y), range 0..1024
 *    b1 = 0, 1 for button not pressed (0) or pressed (1)
 *    b2 = 0, 1 for button not pressed (0) or pressed (1)
 */

#include <LiquidCrystal.h>
#include <NewPing.h>

LiquidCrystal lcd(4, 5, 9, 8, 7 ,6);
NewPing sonar(10, 11, 30);

int interval=100;

int prevX = 0;
int prevY = 0;
int prevB1 = 0;
int prevB2 = 0;

#define AXIS_TOLERANCE 5

unsigned long startTime = 0;
bool inRun = false;

unsigned long lastLcdUpdate = 0;

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  lcd.begin(20,2);
  lcd.print("Lobstacle Challenge");
  lcd.setCursor(0,1);
  lcd.print("Press fire to go...");
}

void loop() {
  int x = analogRead(A0);
  int y = analogRead(A1);
  int b1 = digitalRead(2);
  int b2 = digitalRead(3);

  if (abs(x-prevX) > AXIS_TOLERANCE
    || abs(y-prevY) > AXIS_TOLERANCE
    || b1 != prevB1
    || b2 != prevB2)
  {
    prevX = x;
    prevY = y;
    prevB1 = b1;
    prevB2 = b2;
    
    if (inRun) {
      // Send latest joystick positions to Logobot
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.print(",");
      Serial.print(b1);
      Serial.print(",");
      Serial.println(b2);

    }
    else {
      if (b1 == 1) { // go!
        lcd.clear();
        lcd.print("3");
        delay(500);
        lcd.print(".");
        delay(500);
        lcd.print("2");
        delay(500);
        lcd.print(".");
        delay(500);
        lcd.print("1");
        delay(500);
        lcd.print(".");
        delay(500);
        lcd.clear();
        lcd.print("GO!!");
        
        startTime = millis();
        inRun = true;
      }
    }

  }

  if (inRun) {
    // Read Sonar
    int dist = sonar.ping_cm();
    lcd.setCursor(8, 0);
    lcd.print("        ");
    lcd.setCursor(8, 0);
    lcd.print("Dist: ");
    if (dist > 0) {
      lcd.print(dist);
      if (dist <= 10) {
        // close enough
        // Stop run
        inRun = false;
        // Stop Logobot
        Serial.println("500,500,0,0");
        // Update LCD
        lcd.setCursor(0,0);
        lcd.print("HOME!");
      }
    } else {
      lcd.print("-");
    }
  }

  if (inRun && millis() - lastLcdUpdate > 1000) {
      lcd.setCursor(0, 1);
      lcd.print("Time: ");
      lcd.print((millis() - startTime)/1000);
      lcd.print("s");
  }

  delay(interval);
}
