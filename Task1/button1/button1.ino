
const int led_pin = 7;
const int button_pin = 3;

int ledState = LOW;
int lastBState = HIGH;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);
}

void loop() {
  int bState = digitalRead(button_pin);

  if(bState == LOW && lastBState == HIGH) {
    digitalWrite(led_pin, ledState);
    ledState = !ledState;
  }

  lastBState = bState;
}
