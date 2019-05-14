const uint8_t pwm_pin = 5;
const uint8_t in2_pin = 49;
const uint8_t in1_pin = 47;

const uint8_t led_red = 11;
const uint8_t led_green = 8;
const uint8_t led_blue = 7;

int sonic_step = 16;


String getMode() {
  if (directionMode) {
    return "DIRECTION";
  } else {
    return "SONIC";
  }
}

String getDirection() {

  switch (tails) {
    case -1:
      return "CCW";
    case 0:
      return "STOP";
    case 1:
      return "CW";
    default:
      return "WHAT?;";
  }
}

String getSonic() {
  float power = (float) sonic;

  return String(round(power / 2.55));
}

void setRGB(int r, int g, int b) {
  analogWrite(led_red, 255 - r);
  analogWrite(led_green, 255 - g);
  analogWrite(led_blue, 255 - b);
}

void changeDirection(bool dir) {
  if (dir) {
    tails++;
  } else {
    tails--;
  }

  if (abs(tails) > 1) {
    tails = 0;
  }
}

void decreaseOutput() {
  sonic = max(0, sonic - sonic_step);
}

void increaseOutput() {
  sonic = min(255, sonic + sonic_step);
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

void setGeschwindigkeit(int geschwindigkeit) {
  analogWrite(pwm_pin, geschwindigkeit);
}

void setDrehrichtung(int drehrichtung) {
  if (drehrichtung == 0) {
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, LOW);
  }
  else if (drehrichtung == -1) {
    digitalWrite(in1_pin, LOW);
    digitalWrite(in2_pin, HIGH);
  }
  else if (drehrichtung == 1) {
    digitalWrite(in1_pin, HIGH);
    digitalWrite(in2_pin, LOW);
  }
}
