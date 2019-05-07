const uint8_t pwm_pin = 5;
const uint8_t in2_pin = 49;
const uint8_t in1_pin = 47;

bool directionMode = false;

void changeDirection(bool dir) {
  ;
}

void decreaseOutput() {
  ;
}

void increaseOutput() {
  ;
}

void button1() {
  if (directionMode) {
    changeDirection(false);
  } else {
    decreaseOutput();
  }
}

void button2() {
  if (directionMode) {
    changeDirection(true);
  } else {
    increaseOutput();
  }
}

void switchMode() {
  directionMode = !directionMode;
}

