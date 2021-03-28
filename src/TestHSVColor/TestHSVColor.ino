#include <Adafruit_NeoPixel.h>

#define PIN 2 
#define NUMPIXELS 150 

#define BRIGHTNESS 150
#define SATURATION 255
#define HUE_DEGREE 182  // 65535 / 360 = 182.0417

#define OFF     Adafruit_NeoPixel::ColorHSV(0, 0, 0)
#define RED     Adafruit_NeoPixel::ColorHSV(0*HUE_DEGREE, SATURATION, BRIGHTNESS) 
#define YELLOW  Adafruit_NeoPixel::ColorHSV(60*HUE_DEGREE, SATURATION, BRIGHTNESS)   // 10920 vs. 10922
#define GREEN   Adafruit_NeoPixel::ColorHSV(120*HUE_DEGREE, SATURATION, BRIGHTNESS)  // 21840 vs. 21845
#define CYAN    Adafruit_NeoPixel::ColorHSV(180*HUE_DEGREE, SATURATION, BRIGHTNESS)  // 32760 vs. 32767
#define BLUE    Adafruit_NeoPixel::ColorHSV(240*HUE_DEGREE, SATURATION, BRIGHTNESS)  // 43680 vs. 43690
#define MAGENTA Adafruit_NeoPixel::ColorHSV(300*HUE_DEGREE, SATURATION, BRIGHTNESS)  // 54600 vs. 54612
#define WHITE   Adafruit_NeoPixel::ColorHSV(0, 0, BRIGHTNESS)
#define COLOR(degree) Adafruit_NeoPixel::ColorHSV(degree*HUE_DEGREE, SATURATION, BRIGHTNESS)

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
// strip.clear(); 
// strip.setPixelColor(i, color);

uint16_t hue = 0;
uint16_t degree = 0;

void setup() {
  Serial.begin(115200);
  strip.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}

void loop() {
  // colorWheel();
  // colorWheelGamma(); 
  // colorSwitch();
  colorSwitchGamma();
}

void colorWheel() {
  // 1 degree: ~182 (0-65535)
  // hue: red: 0 -> Orange -> Yellow: 60  -> Green: 120 -> cyan: 180 -> blue: 240 -> pink: 300 -> red: 360
  // sat: 0=white
  hue += 32;
  Serial.println(hue);
  uint32_t temp_color = strip.ColorHSV(hue, SATURATION, BRIGHTNESS); 
  strip.fill(temp_color, 0, NUMPIXELS);
  strip.show();
}

void colorWheelGamma() {
  hue += 32;
  Serial.println(hue);
  uint32_t temp_color = strip.ColorHSV(hue, SATURATION, BRIGHTNESS); 
  temp_color = strip.gamma32(temp_color);  // gamma-correction 
  strip.fill(temp_color, 0, NUMPIXELS);
  strip.show();
}

void colorSwitch() {
  degree++;
  strip.fill(COLOR(degree%360), 0, NUMPIXELS);
  strip.show();
  delay(10);
}

void colorSwitchGamma() {
  degree++;
  uint32_t temp_color = COLOR(degree%360);
  temp_color = strip.gamma32(temp_color);  // gamma-correction 
  strip.fill(temp_color, 0, NUMPIXELS);
  strip.show();
  delay(10);
}

/*
ColorHSV()
uint32_t Adafruit_NeoPixel::ColorHSV	(	
  uint16_t 	hue,
  uint8_t 	sat = 255,
  uint8_t 	val = 255 
)		

Convert hue, saturation and value into a packed 32-bit RGB color that can be passed to setPixelColor() or other RGB-compatible functions.

Parameters:
- **hue**  	
  An unsigned 16-bit value, 0 to 65535, representing one full loop of the color wheel, which allows 16-bit hues to "roll over" while still doing the expected thing (and allowing more precision than the wheel() function that was common to prior NeoPixel examples).
- **sat =	Saturation**  
  8-bit value, 0 (min or pure grayscale) to 255 (max or pure hue). Default of 255 if unspecified.
- **val =	brightness**  
  8-bit value, 0 (min / black / off) to 255 (max or full brightness). Default of 255 if unspecified.

Returns 32-bit RGB color:  
Result is linearly but not perceptually correct  
&rarr; pass the result through the gamma32() function   
Diffusing the LEDs also really seems to help when using low-saturation colors.
*/
