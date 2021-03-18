#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define LED_COUNT 150
#define SNAKE_LEN 10
#define N_SEGMENTS 10
#define LONGEST_SEGMENT 15


// declare NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// declare segments
Segment segments[N_SEGMENTS] = {{0, 15},   
                                {15, 15}, 
                                {30, 15}, 
                                {45, 15},
                                {60, 15},  
                                {75, 15}, 
                                {90, 15}, 
                                {105, 15},
                                {120, 15}, 
                                {135, 15}};

SegmentedStrip segmentStrip(N_SEGMENTS, LONGEST_SEGMENT, segments);




void setup() {
  segmentStrip.n_segments = 10;
  segmentStrip.longest_segment = 15;
  segmentStrip.segments = segments;

  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t tempBrightness = 100;
  Serial.println(tempBrightness);

  blinkPoliceSegments(tempBrightness, &strip, &segmentStrip);
  // blinkSegments(RED(tempBrightness), BLUE(tempBrightness), 500);
  // animateSegments(RED(tempBrightness), 500);
  // animateSegments(BLUE(tempBrightness), 500);
  // animateSegment(0, RED(tempBrightness), 500);
  // animateSegment(0, BLUE(tempBrightness), 500);
  // animateAllSegments(RED(tempBrightness), 500);
  // animateAllSegments(BLUE(tempBrightness), 500);
}
