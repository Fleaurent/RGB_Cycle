#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define LED_COUNT 150
#define SNAKE_LEN 10

// declare segments
uint8_t segment_starts[] = {0,
                            15, 
                            30, 
                            45, 
                            60, 
                            75, 
                            90, 
                            105,
                            120,
                            135};

SegmentedStrip segmentStrip(segment_starts, LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);




void setup() {
  Serial.begin(115200);
  segmentStrip.begin();
  segmentStrip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t tempBrightness = 100;
  Serial.println(tempBrightness);

  blinkPoliceSegments(tempBrightness, &segmentStrip);
  // blinkSegments(RED(tempBrightness), BLUE(tempBrightness), 500);
  // animateSegments(RED(tempBrightness), 500);
  // animateSegments(BLUE(tempBrightness), 500);
  // animateSegment(0, RED(tempBrightness), 500);
  // animateSegment(0, BLUE(tempBrightness), 500);
  // animateAllSegments(RED(tempBrightness), 500);
  // animateAllSegments(BLUE(tempBrightness), 500);
}
