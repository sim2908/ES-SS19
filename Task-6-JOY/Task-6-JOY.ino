/*
 * JOYSTICK SETUP
 */

const uint8_t joyX_pin = 8;
const uint8_t joyY_pin = 9;
const uint8_t joyButton = 47;

volatile bool pressed = false;

void setup() {
  Serial.begin(9600);

  attachInterrupt(joyButton, alarm, RISING);
}

void loop() {
  Serial.print("X");
  int x = analogRead(8);
  Serial.println(x);
  Serial.print("Y");
  int y = analogRead(9);
  Serial.println(y);
  Serial.println("__________");
  delay(250);

  if(pressed) {
    Serial.println("alarm");
    pressed = false;
    }
}

void alarm () {
  pressed = true;
  }
