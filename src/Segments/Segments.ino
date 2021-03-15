#include <Adafruit_NeoPixel.h>
#include "NeoPixelHelper.h"

#define LED_PIN   2
#define LED_COUNT 150
#define SNAKE_LEN 10

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
const uint8_t n_segments = 10;
const uint8_t longest_segment = 15;
stripSegment segments[n_segments] = {{0, 15}, 
                                     {15, 15}, 
                                     {30, 15},
                                     {45, 15},
                                     {60, 15},
                                     {75, 15},
                                     {90, 15},
                                     {105, 15},
                                     {120, 15},
                                     {135, 15}};

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t tempBrightness = 100;
  Serial.println(tempBrightness);
  
  // blinkPoliceSegments(tempBrightness);
  // blinkSegments(RED(tempBrightness), BLUE(tempBrightness), 500);
  // animateSegments(RED(tempBrightness), 500);
  // animateSegments(BLUE(tempBrightness), 500);
  // animateSegment(0, RED(tempBrightness), 500);
  // animateSegment(0, BLUE(tempBrightness), 500);
  animateAllSegments(RED(tempBrightness), 500);
  animateAllSegments(BLUE(tempBrightness), 500);
}

void blinkPoliceSegments(uint8_t brightness) {
  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(RED(brightness), segments[i].first, segments[i].count);
    }
    else {
      strip.fill(BLUE(brightness), segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(500);

  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(BLUE(brightness), segments[i].first, segments[i].count);
    }
    else {
      strip.fill(RED(brightness), segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(500);
}

void blinkSegments(uint32_t color1, uint32_t color2, uint32_t delayVal) {
  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(color1, segments[i].first, segments[i].count);
    }
    else {
      strip.fill(color2, segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(delayVal);

  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(color2, segments[i].first, segments[i].count);
    }
    else {
      strip.fill(color1, segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(delayVal);
}

// set all LEDs Segment by Segment
void animateSegments(uint32_t color, uint32_t delayVal) {
  for(int i=0; i<n_segments; i++) {
    strip.fill(color, segments[i].first, segments[i].count);
    strip.show();
    delay(delayVal);
  }
}

// set LED by LED of a single Segment
void animateSegment(uint8_t segment, uint32_t color, uint32_t delayVal) {
    for(int i=segments[segment].first; i<segments[segment].count; i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(delayVal);
  }
}

// set LED by LED of all Segments
void animateAllSegments(uint32_t color, uint32_t delayVal) {
  for(int i=0; i<longest_segment; i++) {
    for(int j=0; j<n_segments; j++) {
      if(i < segments[j].count){
        strip.setPixelColor(segments[j].first + i, color);
      }
    }
    strip.show();
    delay(delayVal);
  }
}
