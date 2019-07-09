const int CS_PIN = 4;

void setupSDCard() {
  pinMode(CS_PIN, OUTPUT);

  if (!SD.begin(CS_PIN)) {
    Serial.println("SD initialization failed");
    while (1);
  }
  Serial.println("SD initialized");
}

String readFileToString(File f) {
  unsigned long fileSize = f.size();
  char data[fileSize + 1];

  char buf = f.read();

  int index = 0;

  while (buf != -1 && index < fileSize) {
    data[index] = buf;
    buf = f.read();
    index++;
  }

  data[fileSize] = '\0';

  String line = String(data);

  return line;
}

String readFileToString(String fileName) {
  if (!doesFileExist(fileName)) {
    Serial.println("File not found");

    return "";
  }

  File f = SD.open(fileName);
  String line = readFileToString(f);

  return line;
}

void outputFileToSerial(String fileName) {
  String content = readFileToString(fileName);

  Serial.println(content);
}


boolean doesFileExist(String fileName) {
  return SD.exists(fileName);
}

void listDirectory(String fileName) {
  int y = 1;

  File root = SD.open(fileName);

  if (!root) {
    printErrorMessage("Directory not found");

    return;
  }

  File file = root.openNextFile();

  while (file) {
    String name = file.name();

    if (file.isDirectory()) {
      name.concat("/");
    } else {
      long size = file.size();

      name.concat("   ");
      name.concat(size);
    }

    writeString(1, y, name);
    y += 8;

    file = root.openNextFile();
  }
}

void outputFileToLCD(String fileName) {
  String content = readFileToString(fileName);

  if (fileName.endsWith(".txt") || fileName.endsWith(".TXT")) {
    content.replace("\n", "");
    outputTextToLCD(content);
  } else if (fileName.endsWith(".img") || fileName.endsWith(".IMG")) {
    outputImageToLCD(content);
  } else {
    printErrorMessage("Unrecognized file format");
  }
}

void outputTextToLCD(String s) {
  bool worked = writeStringWithBreaks(1, 1, s);

  if (!worked) {
    writeStringCentered("File too large", displayHeight / 2);
  }
}

void outputImageToLCD(String image_data) {
  String lines[2];
  String dimensions[2];

  if (splitString(image_data, "\n", lines, 2) == -1) {
    printErrorMessage("Invalid image data");

    return;
  }

  if (splitString(lines[0], ",", dimensions, 2) == -1) {
    printErrorMessage("Invalid image data");

    return;
  }

  String data = lines[1];
  int width = dimensions[0].toInt();
  int height = dimensions[1].toInt();
  int startX = displayWidth / 2 - width / 2;
  int startY = displayHeight / 2 - height / 2;
  int len = data.length();
  int index = 0;

  for (int i = 0; i < len; i++) {
    char c = data[i];

    if (c == ',') {
      continue;
    }

    int x = index % width;
    int y = index / width;
    int val = String(c).toInt();
    int color = white;

    if (val == 0) {
      color = black;
    }

    setPixel(startX + x, startY + y, color);

    index++;
  }
}

