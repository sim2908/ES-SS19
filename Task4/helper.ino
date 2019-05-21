void debounceButtons(void) {
  debounceButton1();
  debounceButton2();
  debounceButton3();
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

void debounceButton3(void) {
  int read_state;

  if (digitalRead(b2_pin) == LOW && digitalRead(b1_pin) == LOW) {
    read_state = LOW;
  }
  else {
    read_state = HIGH;
  }

  if (read_state != debounced_button_state3) {
    debounce_counter3++;

    if (debounce_counter3 > 5) {
      debounce_counter3 = 0;

      debounced_button_state3 = read_state;
    }
  } else {
    debounce_counter3 = 0;
  }

  if (debounced_button_state3 == LOW || read_state == LOW) {
    debounce_counter1 = 0;
    debounce_counter2 = 0;
  }
}


boolean handleButtons() {

  boolean res = true;

  if (last_button_state3 == HIGH && debounced_button_state3 == LOW) {
    switchMode();
  } else if (last_button_state1 == HIGH && debounced_button_state1 == LOW) {
    button1();
  } else if (last_button_state2 == HIGH && debounced_button_state2 == LOW) {
    button2();
  } else {
    res = false;
  }

  last_button_state1 = debounced_button_state1;
  last_button_state2 = debounced_button_state2;
  last_button_state3 = debounced_button_state3;

  return res;
}
