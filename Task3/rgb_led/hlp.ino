
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

  sprintf(logString, "<| r:%d g:%d b:%d |>", r, g, b);

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

void printSetREGError(String methodString, String inputString) {
  const char* method = methodString.c_str();
  const char* input = inputString.c_str();

  char logString[250];
  sprintf(logString, "'%s' matched method name %s but had unexpected length or did not end with brackets.\nExpected was: '%s(float, float, float)'\nRecieved was: '%s'", input, method, method, input);
  Serial.println(logString);
}

boolean isValid(String input) {


  if (!input.endsWith(")")) {
    printSetREGError(methods[0], input);
    return false;
  }

  String tr = input.substring(7, input.length() - 1);

  String args[3];
  if (!splitString(args, tr, ",")) {
    printSetREGError(methods[0], input);
    return false;
  }

  int iargs[3];
  for (int i = 0; i < 3; i++) {
    float f = toValidatedFloat(args[i]);
    if (f < 0 || f > 1) {
      const char* method = methods[0].c_str();
      const char* inputCA = input.c_str();
      char logString[250];
      sprintf(logString, "'%s' matched method name %s but paramaters could not be parsed.\nExpected was: '%s(float, float, float)' with all floats between 0.0 and 1.0\nRecieved was: '%s'", inputCA, method, method, inputCA);
      Serial.println(logString);
      return false;
    }
    iargs[i] = (int) (f * 255);
  }

  red = iargs[0];
  green = iargs[1];
  blue = iargs[2];

  return true;
}

float toValidatedFloat(String s) {

  boolean sep = false;

  for (int i = 0; i < s.length(); i++) {
    char c = s[i];

    if (!(c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9')) {
      if (c != '.' || sep) {
        return -1;
      }
    }
  }
  return s.toFloat();
}

boolean splitString(String result[], String s, String delimiter) {
  int len = s.length();
  int idx = s.indexOf(delimiter);
  int last_idx = 0;
  int pointer = 0;

  while (idx != -1 && last_idx < len) {
    String part = s.substring(last_idx, idx);

    result[pointer++] = part;

    last_idx = idx + 1;

    idx = s.indexOf(delimiter, last_idx);

    if (pointer > 2) {
      return false;
    }

  }
  if (last_idx < len) {
    String part = s.substring(last_idx, len);
    result[pointer++] = part;
  }

  if (pointer != 3) {
    return false;
  }
  return true;
}
