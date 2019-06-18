#include <DueTimer.h>

/*
   Display = 128 * 160
   Buchstabe = 6 * 8


   Mitte = 64, 80

   Mitte 1 Bst: 61, 76

*/

const uint16_t height = 160;
const uint16_t width = 128;

const uint16_t black = 0x0000;
const uint16_t white = 0xFFFF;

DueTimer timer;
int tCounter;


void setup() {
  setupDisplay();
  initBuffer();
  writeBuffer();
}

void loop() {
  task4();
}





void task2() {
  for (int i = 0; i < 100; i++) {
    displayPattern(i, 80);
    writeBuffer();
    delay(10);
  }

  writeString(13, 65, "ERROR RESTARTING!");
  writeBuffer();
  delay(2500);

  initBuffer();
}

void task22() {
  for (int i = 0; i < width; i++) {
    fillColumn(i, black);
    writeBuffer();
    delay(20);
  }

  delay(1000);

  for (int i = 0; i < width; i++) {
    fillColumn(i, white);
    writeBuffer();
    delay(20);
  }
}

void startTask3() {
  tCounter = 0;
  if (timer.configure(10, task3)) {
    timer.start();
  }
}

void task3() {

  switch (tCounter) {
    case 0:
      initBuffer();
      runStudentIdDemo("Alexander Herms", "13374269");
      break;
    case 10:
      initBuffer();
      runStudentIdDemo("Katharina Goetz", "12345611");
      break;
    case 20:
      initBuffer();
      runStudentIdDemo("Johannes Boczek", "4206969420");
      break;
  }

  tCounter = (tCounter + 1) % 30;
}

void stopTimers() {
  timer.stop();
}

void task4() {

  String readInput;
  if (Serial.available() > 0) {
    readInput = Serial.readString();
  }

  if (readInput.length() > 0) {
    Serial.println("________________________________________");
    Serial.println("INPUT: " + readInput);
    handleInput(readInput);
  }

  writeBuffer();
}

void startRotatingBarDemo() {
  tCounter = 0;
  if (timer.configure(10, rotatingBarDemo)) {
    timer.start();
  }
}

void rotatingBarDemo() {

  int offsetY1 = 68;
  int offsetY2 = 76;
  int offsetY3 = 84;

  String s1 = "";
  String s2 = "";
  String s3 = "";

  initBuffer();

  switch (tCounter) {
    case 0:
      s1 = " | ";
      s2 = " | ";
      s3 = " | ";
      break;
    case 1:
      s1 = "  /";
      s2 = " / ";
      s3 = "/  ";
      break;
    case 2:
      s1 = "   ";
      s2 = "---";
      s3 = "   ";
      break;
    case 3:
      s1 = "\\  ";
      s2 = " \\ ";
      s3 = "  \\";
      break;
  }

  writeStringCentered(s1, offsetY1);
  writeStringCentered(s2, offsetY2);
  writeStringCentered(s3, offsetY3);

  tCounter = (tCounter + 1) % 4;

}
