
String getSerial() {
  String result;

  while (result == "") {
    result = Serial.readString();
  }
  return result;
}

void updateRGB() {
  if (led_enabled == false) {
    setRGB(0, 0, 0);
  } else {
    setRGB(red, green, blue);
  }
}

void setRGB(int r, int g, int b) {

  char logString[100];

  sprintf(logString,"<| r:%d g:%d b:%d |>", r, g, b);
  
  Serial.println(logString);
  
  analogWrite(led_red, r);
  analogWrite(led_green, g);
  analogWrite(led_blue, b);
}


void printError(String methodString, String inputString) {

  const char* method = methodString.c_str();
  const char* input = inputString.c_str();
  
  char logString[250];
  sprintf(logString, "'%s' matched method name %s but had unexpected length or did not end with brackets.\nExpected was: '%s()'\nRecieved was: '%s'", input, method, method, input);
  Serial.println(logString);
  }
