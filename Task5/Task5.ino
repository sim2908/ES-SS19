
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


void task3() {
  runStudentIdDemo("Alexander Herms", "13374269");
  writeBuffer();
  initBuffer();
  delay(5000);
  runStudentIdDemo("Katharina Goetz", "12345611");
  writeBuffer();
  initBuffer();
  delay(5000);
  runStudentIdDemo("Hannes Boczek", "4206969420");
  writeBuffer();
  initBuffer();
  delay(5000);
}

void task4() {

  String s = getSerial();
  handleInput(s);
  
  
  }
