/*
 * Joystick Interface
 * 
 * Runs on Arduino Pro Mini, taking input from a slightly modified analog joystick
 * with two axes and two buttons and sending out a stream of serial data representing
 * the joystick state.
 * 
 * Connections:
 * 
 *  - pins 2,3 joystick buttons with pull down resistors to GND.
 *  - pins A0, A1 connected to the middle (wiper) pin of the pots on the X and Y axes of the joystick
 * 
 * Serial output format: "x,y,b1,b2"
 *  
 *  where 
 *    x = analogRead(x), range 0..1024
 *    y = analogRead(y), range 0..1024
 *    b1 = 0, 1 for button not pressed (0) or pressed (1)
 *    b2 = 0, 1 for button not pressed (0) or pressed (1)
 */

int interval=100;

int prevX = 0;
int prevY = 0;
int prevB1 = 0;
int prevB2 = 0;

#define AXIS_TOLERANCE 5

void setup() {
  Serial.begin(115200);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
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
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(",");
    Serial.print(b1);
    Serial.print(",");
    Serial.println(b2);

    prevX = x;
    prevY = y;
    prevB1 = b1;
    prevB2 = b2;
  }

  delay(interval);
}
