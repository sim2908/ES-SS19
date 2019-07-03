/*
   SERVO SETUP
*/

#include <Servo.h>
#include <DueTimer.h>

const uint8_t led_pin = -1;

const uint8_t joyX_pin = 8;
const uint8_t joyY_pin = 9;

const uint8_t joyButton = 47;
const uint8_t playButton = -1;
const uint8_t ledButton = -1;


const uint8_t s1_pin = 2;
const uint8_t s2_pin = 3;

volatile bool pressed = false;

DueTimer timer;

volatile int debounce_counter_rst = 0;
volatile int debounced_button_state_rst = HIGH;
int last_button_state_rst = HIGH;

volatile int debounce_counter_joy = 0;
volatile int debounced_button_state_joy = HIGH;
int last_button_state_joy = HIGH;

volatile int debounce_counter_play = 0;
volatile int debounced_button_state_play = HIGH;
int last_button_state_play = HIGH;

volatile int debounce_counter_led = 0;
volatile int debounced_button_state_led = HIGH;
int last_button_state_led = HIGH;


Servo servoB;
Servo servoT;

int servoBAngle = 90;
int servoTAngle = 90;

void setup() {
  Serial.begin(9600);

  pinMode(s1_pin, OUTPUT);
  pinMode(s2_pin, OUTPUT);

  pinMode(led_pin, OUTPUT);

  pinMode(joyButton, INPUT_PULLUP);
  pinMode(playButton, INPUT_PULLUP);
  pinMode(ledButton, INPUT_PULLUP);

  if (timer.configure(100, debounceButtons)) {
    timer.start();
  }

  servoB.attach(2);
  servoT.attach(3);
}

void loop() {

  handleButton();

  servoT.write(servoTAngle);
  servoB.write(servoBAngle);

  servoTAngle = max(min(servoTAngle + relativeJoyToServo(joyX_pin), 180), 0);
  servoBAngle = max(min(servoBAngle + relativeJoyToServo(joyY_pin), 180), 0);

  delay(15);
}

void handleButton () {
  if (debounced_button_state_joy == LOW && last_button_state_joy == HIGH) {
    savePos();
  }
  if (debounced_button_state_rst == LOW && last_button_state_rst == HIGH) {
    deletePos();
  }
  if (debounced_button_state_led == LOW && last_button_state_led == HIGH) {
    digitalWrite(led_pin,!digitalRead(led_pin));
  }
  if (debounced_button_state_play == LOW && last_button_state_play == HIGH) {
    //playSequence();
  }

  last_button_state_joy = debounced_button_state_joy;
  last_button_state_play = debounced_button_state_play;
  last_button_state_rst = debounced_button_state_rst;
  last_button_state_led = debounced_button_state_led;
}
