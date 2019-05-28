void setup() {
  setupDisplay();


  Serial.println(getColor(0,0,1), BIN);
  Serial.println(getColor(1,0,1), BIN);
  Serial.println(getColor(1,1,1), BIN);
}

void loop() {
  writeBuffer();
  delay(2000);
}
