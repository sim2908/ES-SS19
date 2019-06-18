#include <SD.h>

void setup() {

  Serial.begin(9600);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
  }
  Serial.println("initialization complete!");

}


void loop () {

}
