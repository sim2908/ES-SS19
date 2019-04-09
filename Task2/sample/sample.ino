#include <DueTimer.h>

const uint8_t led_pin = 7;
volatile int led_state = LOW;
DueTimer timer;

void changeLedState(void) {
  led_state = !led_state;
}

void setup() {
  
  pinMode(led_pin, OUTPUT);
  
  if (timer.configure(1, changeLedState)) {
    timer.start();
  }
}

void loop() {
  digitalWrite(led_pin, led_state);
}



