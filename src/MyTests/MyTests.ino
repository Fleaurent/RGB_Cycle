#include <Adafruit_NeoPixel.h>

#define PIN 2 
#define NUMPIXELS 150 

// When setting up the NeoPixel library, we tell it how many pixles,
// and which pin to use to send signals
Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// colors
#define BRIGHTNESS 150
uint32_t tempBrightness = 0;

// fixed brightness
#define RED strip.Color(BRIGHTNESS, 0, 0)
#define GREEN strip.Color(0, BRIGHTNESS, 0)
#define BLUE strip.Color(0, 0, BRIGHTNESS)
#define YELLOW strip.Color(BRIGHTNESS, BRIGHTNESS, 0)
#define MAGENTA strip.Color(BRIGHTNESS, 0, BRIGHTNESS)
#define CYAN strip.Color(0, BRIGHTNESS, BRIGHTNESS)
#define WHITE strip.Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS)

// macro brightness
#define RED(brightness) strip.Color(brightness, 0, 0)
#define GREEN(brightness) strip.Color(0, brightness, 0)
#define BLUE(brightness) strip.Color(0, 0, brightness)
#define YELLOW(brightness) strip.Color(brightness, brightness, 0)
#define MAGENTA(brightness) strip.Color(brightness, 0, brightness)
#define CYAN(brightness) strip.Color(0, brightness, brightness)
#define WHITE(brightness) strip.Color(brightness, brightness, brightness)

#define DELAYVAL 50 // Time (in milliseconds) to pause between strip
#define DELAYLOOP 1000

// function declaration
void animation(uint32_t color, uint32_t delayval);
void setStripe(uint32_t color);
void setSegment(uint8_t startPixel, uint8_t endPixel, uint32_t color);

void setup() {
  Serial.begin(115200);
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}

void loop() {
  tempBrightness = 10;
  Serial.println(tempBrightness);
//  testStripe(tempBrightness);
  testSegment(tempBrightness);
//  testAnimation(tempBrightness, DELAYVAL);
  

  tempBrightness = 50;
  Serial.println(tempBrightness);
//  testStripe(tempBrightness);
  testSegment(tempBrightness);
//  testAnimation(tempBrightness, DELAYVAL);

  tempBrightness = 100;
  Serial.println(tempBrightness);
//  testStripe(tempBrightness);
//  testAnimation(tempBrightness, DELAYVAL);
  testSegment(tempBrightness);

  tempBrightness = 150;
  Serial.println(tempBrightness);
//  testStripe(tempBrightness);
//  testAnimation(tempBrightness, DELAYVAL);
  testSegment(tempBrightness);

  tempBrightness = 200;
  Serial.println(tempBrightness);
//  testStripe(tempBrightness);
//  testAnimation(tempBrightness, DELAYVAL);
  testSegment(tempBrightness);
}

void testStripe(uint8_t brightness){
  Serial.println("setStripe(RED));");
  strip.clear();
  setStripe(RED(brightness));
  delay(DELAYLOOP); 
  
  Serial.println("setStripe(GREEN(BRIGHTNESS));");
  strip.clear(); 
  setStripe(GREEN(brightness));
  delay(DELAYLOOP);  

  Serial.println("setStripe(BLUE(BRIGHTNESS));");
  strip.clear();
  setStripe(BLUE(brightness));
  delay(DELAYLOOP); 

  Serial.println("setStripe(YELLOW(BRIGHTNESS));");
  strip.clear(); 
  setStripe(YELLOW(brightness));
  delay(DELAYLOOP);

  Serial.println("setStripe(MAGENTA(BRIGHTNESS));");
  strip.clear(); 
  setStripe(MAGENTA(brightness));
  delay(DELAYLOOP); 

  Serial.println("setStripe(CYAN(BRIGHTNESS));");
  strip.clear(); 
  setStripe(CYAN(brightness));
  delay(DELAYLOOP); 

  Serial.println("setStripe(WHITE(BRIGHTNESS));");
  strip.clear(); 
  setStripe(WHITE(brightness));
  delay(DELAYLOOP); 
}

void testAnimation(uint8_t brightness, uint32_t delayval){
  Serial.println("animation(RED(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(RED(brightness), DELAYVAL);
  delay(DELAYLOOP); 
  
  Serial.println("animation(GREEN(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(GREEN(brightness), DELAYVAL);
  delay(DELAYLOOP);  

  Serial.println("animation(BLUE(BRIGHTNESS), DELAYVAL);");
  strip.clear();
  animation(BLUE(brightness), DELAYVAL);
  delay(DELAYLOOP); 

  Serial.println("animation(YELLOW(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(YELLOW(brightness), DELAYVAL);
  delay(DELAYLOOP);

  Serial.println("animation(MAGENTA(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(MAGENTA(brightness), DELAYVAL);
  delay(DELAYLOOP); 

  Serial.println("animation(CYAN(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(CYAN(brightness), DELAYVAL);
  delay(DELAYLOOP); 

  Serial.println("animation(WHITE(BRIGHTNESS), DELAYVAL);");
  strip.clear(); 
  animation(WHITE(brightness), DELAYVAL);
  delay(DELAYLOOP);  
}



void testSegment(uint8_t brightness){
  setSegment(0, 50, RED(brightness));
  setSegment(50, 100, GREEN(brightness));
  setSegment(100, 150, BLUE(brightness));
  delay(DELAYLOOP); 

  setSegment(0, 50, GREEN(brightness));
  setSegment(50, 100, BLUE(brightness));
  setSegment(100, 150, RED(brightness));
  delay(DELAYLOOP);

  setSegment(0, 50, BLUE(brightness));
  setSegment(50, 100, RED(brightness));
  setSegment(100, 150, GREEN(brightness));
  delay(DELAYLOOP);
}


void setStripe(uint32_t color) {
  for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    strip.setPixelColor(i, color);
  }
  strip.show();   // Send the updated pixel colors to the hardware.
}

void animation(uint32_t color, uint32_t delayval) {
   for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
    strip.setPixelColor(i, color);
    strip.show();   // Send the updated pixel colors to the hardware.
    delay(delayval); // Pause before next pass through loop
  }
}

void setSegment(uint8_t startPixel, uint8_t endPixel, uint32_t color) {
   for(int i=startPixel; i<endPixel; i++) { // For each pixel in the segment
    strip.setPixelColor(i, color);
  }
  strip.show();   // Send the updated pixel colors to the hardware.
}

void animateSegment(uint8_t startPixel, uint8_t endPixel, uint32_t color, uint32_t delayval) {
   for(int i=startPixel; i<endPixel; i++) { // For each pixel in the segment
    strip.setPixelColor(i, color);
    delay(DELAYVAL); // Pause before next pass through loop
  }
  strip.show();   // Send the updated pixel colors to the hardware.
}
