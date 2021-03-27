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
  
  /* Optional: print strip information */
  Serial.println(segmentStrip.getNSegments());
  Serial.println(segmentStrip.getLongestSegment());
  Serial.println(segmentStrip.brightness);

  for(uint8_t i = 0; i < n_segments; i++) {
    Serial.print(segmentStrip.getSegments()[i].first);
    Serial.print('\t');
    Serial.println(segmentStrip.getSegments()[i].count);
  }

  Serial.println("Running...");
}


void loop() {
  /* 
    ToDo:  
    - add statemachine: run specific function based on selection
    - add static framecounter passed to function
    - add selection of segments to function
    - update strip at the end of loop
    - set brightness based on poti
    - shift per segment/per pixel
  */

  // 1. get input, set brightness
  // i.e. UART input

  // 2. run method based on last valid input
  // segmentStrip.blinkPoliceSegments(100);
  
  // 2.1 blink complete segments
  // a) blink 50/50 duty cycle
  // segmentStrip.blinkSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 200);
  // segmentStrip.blinkSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getAllSegments(), 200);
  // segmentStrip.blinkAllSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 200);
  // segmentStrip.blinkEvenSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 200);
  // segmentStrip.blinkOddSegments(BLUE(segmentStrip.brightness), RED(segmentStrip.brightness), 200);
  // segmentStrip.blinkFirstSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 5, 200);
  // segmentStrip.blinkLastSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 5, 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 200, 50);
  
  // 2.2 blink pixel of segments
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 1 << (segmentStrip.longest_segment - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getOddSegments(), segmentStrip.getOddPixels(), 200);
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getEvenSegments(), segmentStrip.getEvenPixels(), 200);
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200, 50);

  // 2.3 animations
  // segmentStrip.animateSegments(RED(segmentStrip.brightness), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip.animateSegments(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip.animateSegmentsPixel(RED(segmentStrip.brightness), segmentStrip.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  segmentStrip.animateSegmentsPixel(RED(segmentStrip.brightness), BLUE(segmentStrip.brightness), segmentStrip.getAllSegments(), 0x7, 1, 300, 20); 

  /* 3. update strip at the end of the loop */
  segmentStrip.show();
  segmentStrip.clear();
  segmentStrip.frame_counter++;
}


  /* old functions */
  // blinkSegments(RED(tempBrightness), BLUE(tempBrightness), 500);
  // animateSegments(RED(tempBrightness), 500);
  // animateSegments(BLUE(tempBrightness), 500);
  // animateSegment(0, RED(tempBrightness), 500);
  // animateSegment(0, BLUE(tempBrightness), 500);
  // animateAllSegments(RED(tempBrightness), 500);
  // animateAllSegments(BLUE(tempBrightness), 500);
