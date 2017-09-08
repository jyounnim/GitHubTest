/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SSD1306.h>

// Initialize the OLED display using Wire library
SSD1306 display(0x3c, 5, 4); //GPIO 4 = D2 (SDA), GPIO 5 = D1(SCL)

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16 
#define LOGO16_GLCD_WIDTH  16 
static const char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000 };

void setup()   {                
  Serial.begin(115200);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.init();  // initialize with the I2C
  // init done
  display.println("***** ESP8266 OLED *****");
  
  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.
  display.display();

  // Clear the buffer.
  display.clear();

  // draw a single pixel
  display.setColor(WHITE);
  display.setPixel(10, 10);
  // Show the display buffer on the hardware.
  // NOTE: You _must_ call display after making any drawing commands
  // to make them visible on the display hardware!
  display.display();
  delay(2000);
  display.clear();

  // draw many lines
  testdrawline();
  display.display();
  delay(2000);
  display.clear();

  // draw rectangles
  testdrawrect();
  display.display();
  delay(2000);
  display.clear();

  // draw multiple rectangles
  testfillrect();
  display.display();
  delay(2000);
  display.clear();

  // draw mulitple circles
  testdrawcircle();
  display.display();
  delay(2000);
  display.clear();

#if (0)
  // draw the first ~12 characters in the font
  testdrawchar();
  display.display();
  delay(2000);
  display.clear();
#endif

  // text display tests
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  display.drawString(0,0, "Hello, world!");
  display.drawString(0,10, "3.141592");
  display.setFont(ArialMT_Plain_16);
  display.drawString(0,10+16,"0xDEADBEEF");
  display.display();
  delay(2000);
  display.clear();

  // miniature bitmap display
  display.drawFastImage(30, 16, 16, 16, logo16_glcd_bmp);
  display.display();
  // invert the display
  delay(1000); 
  display.invertDisplay();
  delay(1000); 
  display.normalDisplay();
  display.clear();

  // draw a bitmap icon and 'animate' movement
  testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
}


void loop() {
  
}


void testdrawbitmap(const char *bitmap, uint8_t w, uint8_t h) {
  uint8_t icons[NUMFLAKES][3];
 
  // initialize
  for (uint8_t f=0; f< NUMFLAKES; f++) {
    icons[f][XPOS] = random(DISPLAY_WIDTH);
    icons[f][YPOS] = 0;
    icons[f][DELTAY] = random(5) + 1;
    
    Serial.print("x: ");
    Serial.print(icons[f][XPOS], DEC);
    Serial.print(" y: ");
    Serial.print(icons[f][YPOS], DEC);
    Serial.print(" dy: ");
    Serial.println(icons[f][DELTAY], DEC);
  }

  while (1) {
    display.clear();
    // draw each icon
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawFastImage(icons[f][XPOS], icons[f][YPOS], w, h, bitmap);
    }
    display.display();
    delay(200);
    
    // then erase it + move it
    for (uint8_t f=0; f< NUMFLAKES; f++) {
      display.drawFastImage(icons[f][XPOS], icons[f][YPOS], w, h, bitmap);
      // move it
      icons[f][YPOS] += icons[f][DELTAY];
      // if its gone, reinit
      if (icons[f][YPOS] > DISPLAY_HEIGHT) {
        icons[f][XPOS] = random(DISPLAY_WIDTH);
        icons[f][YPOS] = 0;
        icons[f][DELTAY] = random(5) + 1;
      }
    }
   }
}

void testdrawchar(void) {
  display.setFont(ArialMT_Plain_10);
  display.setColor(WHITE);
  display.setPixel(0,0);

  for (uint8_t i=0, j; i < 168; i++) {
    if (i == '\n') 
      continue;
    
    display.write(i);
    
    if ((i > 0) && (i % 21 == 0))
      display.setPixel(0, 10*j++);
  }    
  display.display();
  
  delay(1);
}

void testdrawcircle(void) {
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=2) {
    display.drawCircle(DISPLAY_WIDTH/2, DISPLAY_HEIGHT/2, i);
    display.display();
    delay(1);
  }
}

void testfillrect(void) {
  for (int16_t i=0; i<DISPLAY_HEIGHT/2; i+=3) {
    // alternate colors
    display.fillRect(i, i, DISPLAY_WIDTH-i*2, DISPLAY_HEIGHT-i*2);
    display.display();
    delay(1);
    display.setColor(INVERSE);
  }
}
   
void testdrawrect(void) {
  for (int16_t i=0; i<DISPLAY_HEIGHT/2; i+=2) {
    display.drawRect(i, i, DISPLAY_WIDTH-2*i, DISPLAY_HEIGHT-2*i);
    display.display();
    delay(1);
  }
}

void testdrawline() {  
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(0, 0, i, DISPLAY_HEIGHT-1);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(0, 0, DISPLAY_WIDTH-1, i);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clear();
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(0, DISPLAY_HEIGHT-1, i, 0);
    display.display();
    delay(1);
  }
  for (int16_t i=DISPLAY_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(0, DISPLAY_HEIGHT-1, DISPLAY_WIDTH-1, i);
    display.display();
    delay(1);
  }
  delay(250);
  
  display.clear();
  for (int16_t i=DISPLAY_WIDTH-1; i>=0; i-=4) {
    display.drawLine(DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1, i, 0);
    display.display();
    delay(1);
  }
  for (int16_t i=DISPLAY_HEIGHT-1; i>=0; i-=4) {
    display.drawLine(DISPLAY_WIDTH-1, DISPLAY_HEIGHT-1, 0, i);
    display.display();
    delay(1);
  }
  delay(250);

  display.clear();
  for (int16_t i=0; i<DISPLAY_HEIGHT; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, 0, i);
    display.display();
    delay(1);
  }
  for (int16_t i=0; i<DISPLAY_WIDTH; i+=4) {
    display.drawLine(DISPLAY_WIDTH-1, 0, i, DISPLAY_HEIGHT-1); 
    display.display();
    delay(1);
  }
  delay(250);
}

