
const uint16_t black = 0x0000;
const uint16_t white = 0xFFFF;


void setup() {
  setupDisplay();
  initBuffer();
  writeBuffer();
}

void loop() {
  delay(2000);
  writeString(5,75,"Press F in chat");
  writeBuffer();
}
