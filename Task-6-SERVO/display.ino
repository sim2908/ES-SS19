//pin declarationswriteBuffer
#define TFT_CS     10   //display: CS-pin
#define TFT_RST     9   //display: reset
#define TFT_DC      8   //display: Data/Command (D/C)
#define SD_CS     4             //SD-CSpin


#define TFT_DC_HIGH()           digitalWrite(TFT_DC, HIGH)
#define TFT_DC_LOW()            digitalWrite(TFT_DC, LOW)


//SPI-Settings
#define SPI_DEFAULT_FREQ        8000000      ///< Default SPI data clock frequency
SPISettings settingsTFT(SPI_DEFAULT_FREQ, MSBFIRST, SPI_MODE0);


//TFT-area of 128 x 160 (1.8") TFT
const uint8_t FIRST_COL = 2;
const uint8_t FIRST_ROW = 1;
const uint8_t LAST_COL = 129;
const uint8_t LAST_ROW = 160;

//TFT's commands
const uint8_t SWRESET = 0x01;           // Software reset
const uint8_t SLPOUT = 0x11;            //Sleep out & booster on
const uint8_t DISPOFF = 0x28;       //Display off
const uint8_t DISPON = 0x29;            //Display on
const uint8_t CASET = 0x2A;                     //Column adress set
const uint8_t RASET = 0x2B;                     //Row adress set
const uint8_t RAMWR = 0x2C;                     //Memory write
const uint8_t MADCTL = 0x36;            //Memory Data Access Control
const uint8_t COLMOD = 0x3A;            //RGB-format, 12/16/18bit
const uint8_t INVOFF = 0x20;            // Display inversion off
const uint8_t INVON = 0x21;                     // Display inversion on
const uint8_t INVCTR = 0xB4;            //Display Inversion mode control
const uint8_t NORON = 0x13;                     //Partial off (Normal)

const uint8_t PWCTR1 = 0xC0;            //Power Control 1
const uint8_t PWCTR2 = 0xC1;            //Power Control 2
const uint8_t PWCTR3 = 0xC2;            //Power Control 3
const uint8_t PWCTR4 = 0xC3;            //Power Control 4
const uint8_t PWCTR5 = 0xC4;            //Power Control 5
const uint8_t VMCTR1 = 0xC5;            //VCOM Voltage setting

const uint16_t displayHeight = 160;
const uint16_t displayWidth = 128;

const uint16_t black = 0x0000;
const uint16_t white = 0xFFFF;

//global variables
uint8_t invState = 0;

uint16_t dBuffer[128][160];

void setContrast(byte val) {
  SPI.beginTransaction(TFT_CS, settingsTFT);
  TFTwriteCommand(VMCTR1);
  SPI.transfer(TFT_CS, val);
  SPI.endTransaction();
}

void initBuffer() {
  for (uint16_t x = 0; x < 128; x++) {
    for (uint16_t y = 0; y < 160; y++) {
      dBuffer[x][y] = white;
    }
  }
}

void writeBuffer() {
  uint8_t xs = 2;
  uint8_t xe = 129;
  uint8_t ys = 1;
  uint8_t ye = 160;

  SPI.beginTransaction(TFT_CS, settingsTFT);
  TFTwriteCommand(CASET); //define writewindow column-range
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, xs);
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, xe);
  TFTwriteCommand(RASET); //define writewindow row-range
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, ys);
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, ye);
  TFTwriteCommand(RAMWR);  //assigne background-color to every element of writewindow
  for (uint16_t y = 0; y < 160; y++) {
    for (uint16_t x = 0; x < 128; x++) {

      uint16_t pixel = getPixel(x, y);

      byte low = pixel & 0xFF;
      byte high = (pixel >> 8) & 0xFF;

      SPI.transfer(TFT_CS, high);
      SPI.transfer(TFT_CS, low);

    }
  }
  SPI.endTransaction();
}

uint16_t getPixel(int x, int y) {
  return dBuffer[x][y];
}

void  setPixel(int x, int y, uint16_t color) {
  dBuffer[x][y] = color;
}

uint16_t getColor(double r, double g, double b) {
  int red = (int) (r * 31);
  int green = (int) (g * 63);
  int blue = (int) (b * 31);

  uint16_t color = 0;

  color += (red << 11);
  color += (green << 5);
  color += blue;

  return color;
}




//-------------------------------------





void TFTwriteCommand(uint8_t cmd) {
  TFT_DC_LOW();
  SPI.transfer(TFT_CS, cmd);
  TFT_DC_HIGH();
}

void TFTinit(void) {
  Serial.begin(9600);
  //minimal configuration: only settings which are different from Reset Default Value
  //or not affected by HW/SW-reset
  SPI.beginTransaction(TFT_CS, settingsTFT);

  TFTwriteCommand(SWRESET);
  delay(120);                     //mandatory delay
  TFTwriteCommand(SLPOUT);        //turn off sleep mode.
  delay(120);
  TFTwriteCommand(PWCTR1);
  SPI.transfer(TFT_CS, 0xA2);
  SPI.transfer(TFT_CS, 0x02);
  SPI.transfer(TFT_CS, 0x84);
  TFTwriteCommand(PWCTR4);
  SPI.transfer(TFT_CS, 0x8A);
  SPI.transfer(TFT_CS, 0x2A);
  TFTwriteCommand(PWCTR5);
  SPI.transfer(TFT_CS, 0x8A);
  SPI.transfer(TFT_CS, 0xEE);
  TFTwriteCommand(VMCTR1);
  SPI.transfer(TFT_CS, 0x0E);                    //VCOM = -0.775V

  //Memory Data Access Control D7/D6/D5/D4/D3/D2/D1/D0
  //                                                       MY/MX/MV/ML/RGB/MH/-/-
  // MY- Row Address Order; ‘0’ =Increment, (Top to Bottom)
  // MX- Column Address Order; ‘0’ =Increment, (Left to Right)
  // MV- Row/Column Exchange; '0’ = Normal,
  // ML- Scan Address Order; ‘0’ =Decrement,(LCD refresh Top to Bottom)
  //RGB - '0'= RGB color fill order
  // MH - '0'= LCD horizontal refresh left to right
  TFTwriteCommand(MADCTL);
  SPI.transfer(TFT_CS, 0x08);

  TFTwriteCommand(COLMOD);        //RGB-format, color mode
  SPI.transfer(TFT_CS, 0x55); //16-bit/pixel; high nibble don't care

  TFTwriteCommand(CASET);    //set column adress range: full range
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, FIRST_COL);
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, LAST_COL);
  TFTwriteCommand(RASET);   //set row adress range: full range
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, FIRST_ROW);
  SPI.transfer(TFT_CS, 0x00); SPI.transfer(TFT_CS, LAST_ROW);

  TFTwriteCommand(NORON);
  TFTwriteCommand(DISPON);

  SPI.endTransaction();
}

void setupDisplay() {
  // set pin-modes
  pinMode(TFT_RST, OUTPUT);
  pinMode(TFT_DC, OUTPUT);

  // initialize serial port 0
  Serial.begin(9600);

  Serial.println("Exercise no.5 template\n");
  // initialize SPI:
  // several devices: multiple SPI.begin(nn_CS) possible
  SPI.begin(TFT_CS);
  delay(10);

  //power-on-reset of Display
  digitalWrite(TFT_RST, HIGH);
  delay(100);
  digitalWrite(TFT_RST, LOW);
  delay(100);
  digitalWrite(TFT_RST, HIGH);
  delay(100);

  TFTinit();
  Serial.println("Display Initialized");
  delay(100);
}


const unsigned char font[95][6] =
{
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // space
  { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 }, // !
  { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, // "
  { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 }, // #
  { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 }, // $
  { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, // %
  { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, // &
  { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 }, // '
  { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 }, // (
  { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 }, // )
  { 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00 }, // *
  { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, // +
  { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, // ,
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, // -
  { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, // .
  { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, // slash
  { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 }, // 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 }, // 1
  { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, // 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 }, // 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 }, // 4
  { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, // 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 }, // 6
  { 0x03, 0x71, 0x09, 0x05, 0x03, 0x00 }, // 7
  { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, // 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 }, // 9
  { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, // :
  { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, // ;
  { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, // <
  { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, // =
  { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
  { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, // ?
  { 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00 }, // @
  { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 }, // A
  { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, // B
  { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 }, // C
  { 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // D
  { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, // E
  { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, // F
  { 0x3E, 0x41, 0x41, 0x49, 0x7A, 0x00 }, // G
  { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, // H
  { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, // I
  { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 }, // J
  { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, // K
  { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, // L
  { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 }, // M
  { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, // N
  { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // O
  { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, // P
  { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 }, // Q
  { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, // R
  { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 }, // S
  { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 }, // T
  { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, // U
  { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, // V
  { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 }, // W
  { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // X
  { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Y
  { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Z
  { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 }, // [
  { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, // backslash
  { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
  { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, // ^
  { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, // _
  { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, // `
  { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, // a
  { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 }, // b
  { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, // c
  { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 }, // d
  { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, // e
  { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 }, // f
  { 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00 }, // g
  { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 }, // h
  { 0x00, 0x48, 0x7D, 0x40, 0x00, 0x00 }, // i
  { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 }, // j
  { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 }, // k
  { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 }, // l
  { 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 }, // m
  { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 }, // n
  { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, // o
  { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 }, // p
  { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 }, // q
  { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 }, // r
  { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, // s
  { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 }, // t
  { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 }, // u
  { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 }, // v
  { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 }, // w
  { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, // x
  { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 }, // y
  { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 }, // z
  { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, // {
  { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 }, // |
  { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, // }
  { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 } // ~
};

const int offset = 32;

bool writeString(int x, int y, String s) {
  return writeString(x, y, s, black, white);
}

bool writeString(int x, int y, String s, uint16_t fgColor, uint16_t bgColor) {
  int len = s.length();

  for (int i = 0; i < len; i++) {
    if (!writeChar(x + (i * 6), y, s[i], fgColor, bgColor)) {
      return false;
    }
  }

  return true;
}

bool writeStringWithBreaks(int x, int y, String s) {
  return writeStringWithBreaks(x, y, s, black, white);
}

bool writeStringWithBreaks(int x, int y, String s, uint16_t fgColor, uint16_t bgColor) {
  int len = s.length();
  int currentX = x;
  int currentY = y;

  for (int i = 0; i < len; i++) {
    if (currentY > displayHeight) {
      return false;
    }
    
    if (!writeChar(currentX, currentY, s[i], fgColor, bgColor)) {
      currentX = x;
      currentY += 8;
      i--;
    } else {
      currentX += 6;
    }
  }

  return true;
}

bool writeChar(int x, int y, char c) {
  return writeChar(x, y, c, white, black);
}

bool writeChar(int x, int y, char c, uint16_t fgColor, uint16_t bgColor) {
  if (x > 122 || y > 154) {
    return false;
  }

  unsigned char f[6];

  memcpy(f, font[c - offset], 6);

  for (uint16_t i = 0; i < 6; i++) {
    for (uint16_t j = 0; j < 8; j++) {

      byte d = f[i];

      uint16_t color = white;

      if (bitRead(d, j) == 1) {
        color = fgColor;
      } else {
        color = bgColor;
      }

      setPixel(i + x, j + y, color);
    }
  }

  return true;
}

//Mitte = 64, 80

void displayPattern(int offsetX, int offsetY) {
  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      setPixel(i + offsetX, j + offsetY, black);
    }
  }
}

void fillColumn(int column, uint16_t color) {
  for (int i = 0; i < displayHeight; i++) {
    setPixel(column, i, color);
  }
}

void fillRow(int row, uint16_t color) {
  for (int i = 0; i < displayWidth; i++) {
    setPixel(i, row, color);
  }
}

void runStudentIdDemo(String n, String id) {
  int offsetY1 = 70;
  int offsetY2 = 83;

  writeStringCentered(n, offsetY1);
  writeStringCentered(id, offsetY2);

}

void writeStringCentered(String s, int offsetY) {
  int x = (displayWidth / 2) - (s.length() * 6 / 2);
  writeString(x, offsetY, s);
}

