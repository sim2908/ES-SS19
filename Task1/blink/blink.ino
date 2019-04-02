int state = HIGH;

void setup() {
  // modeput your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop() {
  digitalWrite(LED_BUILTIN, state);
  delay(500);
  state = !state;
  digitalWrite(LED_BUILTIN, state);
  delay(2000);
  state = !state;
}
