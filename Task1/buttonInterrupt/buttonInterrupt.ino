//TODO A-1.4 Button with interrupt

const uint8_t led_pin = 7;
const uint8_t button_pin = 3;

volatile int led_state = LOW;

void setup() {
  pinMode(led_pin, OUTPUT);
  pinMode(button_pin, INPUT);

  attachInterrupt(button_pin, blink_led, RISING);

}

void loop() {
  digitalWrite(led_pin, led_state);
}

void blink_led(){
  led_state = ! led_state;
}
