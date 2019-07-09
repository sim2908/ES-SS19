void debounceButtons(void) {

  if (playFlag) {

    if (timeCounter > ((pos_pointer - 1) * 50 * 3 )) {
      timeCounter = 0;
      reportPos();

      playFlag = false;
      return;
    }

    if (timeCounter % 3 == 0) {
      moveToPosition(timeCounter / 3);
    }

    timeCounter++;
    return;
  }
  /*
    LED - RST - JOY - PLAY
  */

  int led_state = digitalRead(ledButton);

  if (led_state != debounced_button_state_led) {
    debounce_counter_led++;

    if (debounce_counter_led > 5) {
      debounce_counter_led = 0;

      debounced_button_state_led = led_state;
    }
  } else {
    debounce_counter_led = 0;
  }


  int rst_state;

  if (digitalRead(playButton) == LOW && digitalRead(joyButton) == LOW) {
    rst_state = LOW;
  }
  else {
    rst_state = HIGH;
  }


  if (rst_state != debounced_button_state_rst) {
    debounce_counter_rst++;

    if (debounce_counter_rst > 5) {
      debounce_counter_rst = 0;

      debounced_button_state_rst = rst_state;
    }
  } else {
    debounce_counter_rst = 0;
  }

  if (debounced_button_state_rst == LOW || rst_state == LOW) {
    debounce_counter_play = 0;
    debounce_counter_joy = 0;
  }


  int play_state = digitalRead(playButton);

  if (play_state != debounced_button_state_play) {
    debounce_counter_play++;

    if (debounce_counter_play > 5) {
      debounce_counter_play = 0;

      debounced_button_state_play = play_state;
    }
  } else {
    debounce_counter_play = 0;
  }


  int joy_state = digitalRead(joyButton);

  if (joy_state != debounced_button_state_joy) {
    debounce_counter_joy++;

    if (debounce_counter_joy > 5) {
      debounce_counter_joy = 0;

      debounced_button_state_joy = joy_state;
    }
  } else {
    debounce_counter_joy = 0;
  }
}
