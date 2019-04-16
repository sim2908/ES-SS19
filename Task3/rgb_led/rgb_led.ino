#include <vector>

std::vector<String> splitString(String s, String delimiter) {
  int len = s.length();
  std::vector<String> ss;
  int idx = -1;
  int last_idx = 0;
  
  while ((idx = s.indexOf(delimiter, last_idx)) != -1 && last_idx < len) {
    String part = s.substring(last_idx, idx);
    
    ss.push_back(part);

    last_idx = idx + 1;
  }

  if (last_idx < len) {
    String part = s.substring(last_idx, len);

    ss.push_back(part);
  }

  return ss;
}

void parseInput(String s) {
  s.trim();
  s.replace(" ", "");

  int len = s.length();

  if (s.startsWith("setRGB(") && s.endsWith(")") && len > 8) {
    String params = s.substring(7, len - 1);
  } else if (s.equals("RGBon()")) {
    ;
  } else if (s.equals("RGBoff()")) {
    ;
  }
}

void setup() {
  Serial.begin(9600);
}

void loop() {
  while (Serial.available() > 0) {
    String s = Serial.readStringUntil('\n');

    //parseInput(s);

    Serial.write(">");

    std::vector<String> parts = splitString(s, ",");

    for (std::vector<String>::iterator i = parts.begin(); i < parts.end(); i++) {
      Serial.write((*i).c_str());
    }

    Serial.write("\n");
  }
}

