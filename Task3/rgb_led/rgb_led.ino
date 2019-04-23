
const String methods[3] = {"setRGB", "RGBon", "RGBoff"};

const uint8_t led_red = 11;
const uint8_t led_green = 8;
const uint8_t led_blue = 7;

int red = 128;
int blue = 128;
int green = 128;

boolean led_enabled = true;

void setup() {
  Serial.begin(9600);
  updateRGB();
  Serial.println("-----Start-----");
}

void loop() {

  String input = getSerial();

  input.trim();

  if (input.startsWith(methods[0])) {
    Serial.println(input + " matches " + methods[0] + "");


  } else if (input.startsWith(methods[1])) {
    if (input.length() != 7 || !input.endsWith("()")) {
      printError(methods[1], input);
      } else {
      Serial.println("'" + input + "' matched method name " + methods[1]);
      led_enabled = true;
    }

  } else if (input.startsWith(methods[2])) {
    if (input.length() != 8 || !input.endsWith("()")) {
      printError(methods[2], input);
    } else {
      Serial.println("'" + input + "' matched method name " + methods[2]);
      led_enabled = false;
    }

  } else {
    Serial.println(input + " matches no method");
  }

  updateRGB();

}

