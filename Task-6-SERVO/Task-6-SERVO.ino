/*
   SERVO SETUP
*/

#include <Servo.h>
#include <DueTimer.h>

const uint8_t joyX_pin = 8;
const uint8_t joyY_pin = 9;
const uint8_t joyButton = 47;

const uint8_t s1_pin = 2;
const uint8_t s2_pin = 3;

volatile bool pressed = false;

DueTimer timer;
volatile int debounce_counter = 0;
volatile int debounced_button_state = HIGH;
int last_button_state = HIGH;

boolean servoRState = false;

Servo servoB;
Servo servoT;

int servoBAngle = 90;
int servoTAngle = 90;

void setup() {
  Serial.begin(9600);

  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);

  pinMode(joyButton, INPUT_PULLUP);

  if (timer.configure(100, debounceButton)) {
    timer.start();
  }

  servoB.attach(2);
  servoT.attach(3);
}

void loop() {

  Serial.println("Start");

  handleButton();

  if (servoRState) {

    Serial.println("R");

    servoT.write(servoTAngle);
    servoB.write(servoBAngle);

    servoTAngle = max(min(servoTAngle + relativeJoyToServo(joyX_pin), 180), 0);
    servoBAngle = max(min(servoBAngle + relativeJoyToServo(joyY_pin), 180), 0);


  } else {

    Serial.println("D");
    directJoyToServo(joyX_pin, servoB);
    directJoyToServo(joyY_pin, servoT);
  }



  delay(15);
}


void directJoyToServo(int pin, Servo servo) {
  float f = (float) analogRead(pin);
  int angle = map(f, 0, 1023, 0, 180);
  servo.write(angle);
}

int relativeJoyToServo(int pin) {
  float f = (float) analogRead(pin);

  if (!(f > 562|| f < 462)) {
    return 0;
    }
  
  int angle = map(f, 0, 1023, -5, 5);
  return angle;
}

void debounceButton(void) {
  int read_state = digitalRead(joyButton);

  if (read_state != debounced_button_state) {
    debounce_counter++;

    if (debounce_counter > 5) {
      debounce_counter = 0;

      debounced_button_state = read_state;
    }
  } else {
    debounce_counter = 0;
  }
}

void handleButton () {
  if (debounced_button_state == LOW && last_button_state == HIGH) {
    servoRState = !servoRState;
    servoBAngle = 90;
    servoTAngle = 90;
  }

  last_button_state = debounced_button_state;
}
