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

//global variables
uint8_t invState = 0;

uint16_t dBuffer[128][160];

void setContrast(uint8_t val) {
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
