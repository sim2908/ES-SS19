#include <DueTimer.h>

const uint8_t led_pin = 7;
const uint8_t b1_pin = 3;
const uint8_t b2_pin = 5;

const int dimm_step = 50;

volatile int led_state = LOW;
volatile int debounced_button1_state = HIGH;
volatile int debounced_button2_state = HIGH;

int led_intensity = 255;
int last_button1_state = HIGH;
int last_button2_state = HIGH;

DueTimer timer;


void setup() {

  pinMode(b1_pin, INPUT_PULLUP);
  pinMode(b2_pin, INPUT_PULLUP);
  // ANALOG pinMode(led_pin, OUTPUT);

  if (timer.configure(100, debounceButtons)) {
    timer.start();
  }
}

void loop() {
  
  updateIntesity();
  analogWrite(led_pin, led_intensity);
}

void updateIntesity() {
  if (debounced_button1_state == LOW && last_button1_state == HIGH) {
    led_intensity = min(led_intensity + dimm_step, 255);
  }
  last_button1_state = debounced_button1_state;


  if (debounced_button2_state == LOW && last_button2_state == HIGH) {
    led_intensity = max(led_intensity - dimm_step, 0);
  }
  last_button2_state = debounced_button2_state;
}

