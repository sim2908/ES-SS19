


volatile int debounce_counter1 = 0;
volatile int debounce_counter2 = 0;



void debounceButtons(void) {
  debounceButton1();
  debounceButton2();
}


void debounceButton1() {
  int read_state = digitalRead(b1_pin);

  if (read_state != debounced_button1_state) {
    debounce_counter1++;

    if (debounce_counter1 > 5) {
      debounce_counter1 = 0;

      debounced_button1_state = read_state;
    }
  } else {
    debounce_counter1 = 0;
  }
}

void debounceButton2() {
  int read_state = digitalRead(b2_pin);

  if (read_state != debounced_button2_state) {
    debounce_counter2++;

    if (debounce_counter2 > 5) {
      debounce_counter2 = 0;

      debounced_button2_state = read_state;
    }
  } else {
    debounce_counter2 = 0;
  }
}
