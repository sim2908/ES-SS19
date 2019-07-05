#include <SD.h>
#include <SPI.h>


void setup() {

  Serial.begin(9600);

  Serial.println("Initializing SD card...");

  pinMode(4, OUTPUT);

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization complete!");


  delay(1000);

}


void loop () {
  String bla = readFileToString("smile2.img");

  Serial.println(bla);

  delay(5000);
}

void outputLineToSerial(File f) {
  char buf = f.read();

  while(buf != '\n' && buf != -1) {
    Serial.print(buf);
    buf = f.read();
  }

  Serial.println();
}

String readFileToString(File f) {
  unsigned long fileSize = f.size();
  char data[fileSize];
  
  char buf = f.read();
  
  int index = 0;

  while(buf != -1 && index < fileSize) {
    data[index] = buf;
    buf = f.read();
    index++;
  }

  String line = String(data);

  return line;
}

String readFileToString(String fileName) {
  if (!doesFileExist(fileName)) {
    Serial.println("file not found");
    return "";
  }

  File f = SD.open(fileName);
  String line = readFileToString(f);

  return line;
}

void outputFileToSerial(String fileName) {
  if (!doesFileExist(fileName)) {
    Serial.println("file not found");
    return;
  }

  File f = SD.open(fileName);

  if (fileName.endsWith(".txt")) {
    outputLineToSerial(f);
  } else if (fileName.endsWith(".img")) {
    outputLineToSerial(f);
    outputLineToSerial(f);
  }
}


boolean doesFileExist(String fileName) {
  return SD.exists(fileName);
}

void printFullDirectory() {
  printDirectory(SD.open("/"), 0);
}

void printDirectory(File dir, int numTabs) {

  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++) {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory()) {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    } else {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}










