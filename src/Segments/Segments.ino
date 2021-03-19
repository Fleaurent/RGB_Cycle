#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define LED_COUNT 150

uint32_t frame_counter = 0;
uint8_t tempBrightness = 100;

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
uint8_t n_segments = sizeof(segment_starts) / sizeof(segment_starts[0]);

// declare strip object
SegmentedStrip segmentStrip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800, segment_starts, n_segments);


void setup() {
  Serial.begin(115200);
  segmentStrip.begin();
  segmentStrip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");

  /* Optional: print strip information */
  Serial.println(segmentStrip.n_segments);
  Serial.println(segmentStrip.longest_segment);
  Serial.println(segmentStrip.brightness);

  for(uint8_t i = 0; i < n_segments - 1; i++) {
    Serial.print(segmentStrip.segments[i].first);
    Serial.print('\t');
    Serial.println(segmentStrip.segments[i].count);
  }
}


void loop() {
  /* 
    ToDo:  
    - add statemachine: run specific function based on selection
    - add static framecounter passed to function
    - add selection of segments to function
    - update strip at the end of loop
    - set brightness based on poti
  */
  
  /* implemented methods */
  // segmentStrip.blinkPoliceSegments(100);
  segmentStrip.blinkSegments(100, RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF);

  /* old functions */
  // blinkSegments(RED(tempBrightness), BLUE(tempBrightness), 500);
  // animateSegments(RED(tempBrightness), 500);
  // animateSegments(BLUE(tempBrightness), 500);
  // animateSegment(0, RED(tempBrightness), 500);
  // animateSegment(0, BLUE(tempBrightness), 500);
  // animateAllSegments(RED(tempBrightness), 500);
  // animateAllSegments(BLUE(tempBrightness), 500);

  /* update strip at the end of the loop */
  segmentStrip.show();
  segmentStrip.frame_counter++;
}
