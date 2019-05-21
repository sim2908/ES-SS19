#include <DueTimer.h>

const uint8_t b1_pin = 13;
const uint8_t b2_pin = 12;

volatile int debounced_button_state1 = HIGH;
volatile int debounced_button_state2 = HIGH;
volatile int debounced_button_state3 = HIGH;

int last_button_state1 = HIGH;
int last_button_state2 = HIGH;
int last_button_state3 = HIGH;

volatile int debounce_counter1 = 0;
volatile int debounce_counter2 = 0;
volatile int debounce_counter3 = 0;

int tails = 1;
int sonic = 128;
bool directionMode = false;

DueTimer timer;

void setup() {
  Serial.begin(9600);

  pinMode(b1_pin, INPUT_PULLUP);
  pinMode(b2_pin, INPUT_PULLUP);

  if (timer.configure(100, debounceButtons)) {
    timer.start();
  }
}

void loop() {
  if (handleButtons()) {
    processChanges();
  }
}

void processChanges() {
  char logString[100];

  sprintf(logString, "[MODE: %9s] [DIRECTION: %4s] [SONIC: %3s%%]", getMode().c_str(), getDirection().c_str(), getSonic().c_str());

  Serial.println(logString);

  if (directionMode) {
    setRGB(0, 255, 0);
  } else {
    setRGB(255, 0, 0);
  }

  setGeschwindigkeit(sonic);
  setDrehrichtung(tails);
}
