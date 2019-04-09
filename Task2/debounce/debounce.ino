#include <DueTimer.h>

const uint8_t led_pin = 7;
const uint8_t b1_pin = 3;
const uint8_t b2_pin = 5;

volatile int led_state = LOW;
volatile int debounced_button_state = HIGH;
int last_button_state = HIGH;

DueTimer timer;
volatile int debounce_counter = 0;


void debounceButton(void) {
  int read_state = digitalRead(b1_pin);

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

void setup() {
  
  pinMode(b1_pin, INPUT_PULLUP);
  pinMode(led_pin, OUTPUT);
  
  if (timer.configure(100, debounceButton)) {
    timer.start();
  }
}

void loop() {
  
  if(debounced_button_state == LOW && last_button_state == HIGH) {
    digitalWrite(led_pin, led_state);
    led_state = !led_state;
  }

  last_button_state = debounced_button_state;
}

