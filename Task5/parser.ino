const int PARAM_BUF_LEN = 10;

void handleInput(String s) {
  String param_buf[PARAM_BUF_LEN];
  String func = parseCall(s, param_buf, PARAM_BUF_LEN);

  if (func == "help") {
    //TODO: Implement
  } else if (func == "setContrast") {
    String param = param_buf[0];
    float val = toValidatedFloat(param);

    if (val == -1.0) {
      //TODO: Error invalid contrast param
    } else if (val < 0.0 || val > 1.0) {
      //TODO: Error contrast out of range
    }

    //TODO: Implement
  } else if (func == "clearDisplay") {
    initBuffer();
    writeBuffer();
    //TODO: Clear Message
  } else if (func == "runRotatingBarDemo") {
    //TODO: Implement
  } else if (func == "runStudentIdDemo") {
    task3();
    //TODO: Repeat? / Message
  } else if (func == "stopDemo") {
    //TODO: Implement
  } else if (func == "") {
    //TODO: Error couldn't parse
  } else {
    //TODO: Error no matching function
  }
}

String parseCall(String s, String param_buf[], int param_buf_len) {
  s.replace(" ", "");

  int name_end_idx = s.indexOf("(");
  int param_end_idx = s.indexOf(")");

  if (name_end_idx == -1 || param_end_idx == -1 || name_end_idx > param_end_idx) {
    //TODO: Error wrong parentheses

    return "";
  } else if (param_end_idx + 1 != s.length()) {
    //TODO: Error no characters after closing parentheses allowed

    return "";
  }

  String func = s.substring(0, name_end_idx);
  String params = s.substring(name_end_idx + 1, param_end_idx);

  int param_cnt = splitString(params, ",", param_buf, param_buf_len);

  if (param_cnt == -1) {
    //TODO: Error too many params

    return "";
  }

  return func;
}

int splitString(String s, String delimiter, String result[], int max_len) {
  int len = s.length();
  int idx = s.indexOf(delimiter);
  int last_idx = 0;
  int pointer = 0;

  while (idx != -1 && last_idx < len) {
    if (pointer >= max_len) {
      return -1;
    }

    String part = s.substring(last_idx, idx);

    result[pointer++] = part;

    last_idx = idx + 1;
    idx = s.indexOf(delimiter, last_idx);
  }

  if (last_idx < len) {
    if (pointer >= max_len) {
      return -1;
    }

    String part = s.substring(last_idx, len);
    result[pointer++] = part;
  }

  return pointer;
}

float toValidatedFloat(String s) {
  if (s.length() == 0) {
    return -1.0;
  }

  bool sep = false;

  for (int i = 0; i < s.length(); i++) {
    char c = s[i];

    if (c != '0' && c != '1' && c != '2' && c != '3' && c != '4' && c != '5' && c != '6' && c != '7' && c != '8' && c != '9') {
      if (sep) {
        return -1.0;
      } else if (c == '.') {
        sep = true;
      }
    }
  }

  return s.toFloat();
}

String getSerial() {
  String result;

  while (result == "") {
    result = Serial.readString();
  }
  return result;
}






//===================================================



void outputImageToLCD(String image_data) {
  String lines[2];
  String dimensions[2];
  String pixels[128 * 160];

  if (splitString(image_data
                  , "\n", lines, 2 == -1)) {
    //TODO: Error couldn't parse image data

    return;
  }

  if (splitString(lines[0], ",", dimensions, 2) != -1) {
    //TODO: Error couldn't parse image data

    return;
  }

  if (splitString(lines[1], ",", pixels, 128 * 160) != -1) {
    //TODO: Error couldn't parse image data

    return;
  }

  int width = dimensions[0].toInt();
  int height = dimensions[1].toInt();

  for (int x = 0; x < width; x++) {
    for (int y = 0; y < height; y++) {
      int idx = x + y * width;
      int val = pixels[idx].toInt();

      if (val == 1) {
        //TODO: Implement
      } else if (val == 0) {
        //TODO: Implement
      }
    }
  }
}
