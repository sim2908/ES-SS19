#include <DueTimer.h>

const uint8_t b1_pin = 31;
const uint8_t b2_pin = 43;

volatile int debounced_button_state1 = HIGH;
volatile int debounced_button_state2 = HIGH;
int last_button_state1 = HIGH;
int last_button_state2 = HIGH;

DueTimer timer;
volatile int debounce_counter1 = 0;
volatile int debounce_counter2 = 0;

void debounceButtons(void) {
  debounceButton1();
  debounceButton2();
}

void debounceButton1(void) {
  int read_state = digitalRead(b1_pin);
  
  if (read_state != debounced_button_state1) {
    debounce_counter1++;

    if (debounce_counter1 > 5) {
      debounce_counter1 = 0;

      debounced_button_state1 = read_state;
    }
  } else {
    debounce_counter1 = 0;
  }
}

void debounceButton2(void) {
  int read_state = digitalRead(b2_pin);

  if (read_state != debounced_button_state2) {
    debounce_counter2++;

    if (debounce_counter2 > 5) {
      debounce_counter2 = 0;

      debounced_button_state2 = read_state;
    }
  } else {
    debounce_counter2 = 0;
  }
}

void setup() {
  Serial.begin(9600);
  
  pinMode(b1_pin, INPUT_PULLUP);
  pinMode(b2_pin, INPUT_PULLUP);
  
  if (timer.configure(100, debounceButtons)) {
    timer.start();
  }
}

void loop() {
  if (last_button_state1 == LOW && last_button_state2 == LOW && (debounced_button_state1 == HIGH || debounced_button_state2 == HIGH)) {
    switchMode();
  } else if (last_button_state1 == LOW && debounced_button_state1 == HIGH) {
    button1();
  } else if (last_button_state2 == LOW && debounced_button_state2 == HIGH) {
    button2();
  }

  last_button_state1 = debounced_button_state1;
  last_button_state2 = debounced_button_state2;
}

