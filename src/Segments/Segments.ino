#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 2
#define LED_COUNT 150

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
  Serial.println(segmentStrip.getBrightness());

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
  // 2.1 blink complete segments
  // a) blink 50/50 duty cycle
  // segmentStrip.blinkSegments(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 200);
  // segmentStrip.blinkSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 200);
  // segmentStrip.blinkAllSegments(segmentStrip.RED(), segmentStrip.BLUE(), 200);
  // segmentStrip.blinkEvenSegments(segmentStrip.RED(), segmentStrip.BLUE(), 200);
  // segmentStrip.blinkOddSegments(segmentStrip.BLUE(), segmentStrip.RED(), 200);
  // segmentStrip.blinkFirstSegments(segmentStrip.RED(), segmentStrip.BLUE(), 5, 200);
  // segmentStrip.blinkLastSegments(segmentStrip.RED(), segmentStrip.BLUE(), 5, 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkSegments(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 200, 50);
  
  // 2.2 blink pixel of segments
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << (segmentStrip.longest_segment - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getOddPixels(), 200);
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getEvenPixels(), 200);
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200, 50);

  // 2.3 animations
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastSegments(1), -1, 500, 50);
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20); 
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  // segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
  // segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20); 

  // 2.4 combined animations
  // animateEvenOdd();
  // animateEvenOdd(600, 300);
  // animateEvenOddInit();
  animateEvenOddInit(720, 360);

  /* 3. update strip at the end of the loop */
  segmentStrip.update();
}


/* combined animations */
void animateEvenOdd(void) {
  // 1. shift pattern forward, jump back
  segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
}

void animateEvenOdd(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip.getFrameCounter() % frames < frameswitch) {
    segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
    segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
    segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  }
}

void animateEvenOddInit(void) {
  // 1. shift pattern forward, jump back
  segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20);
}

void animateEvenOddInit(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip.getFrameCounter() % frames < frameswitch) {
    segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20);
    segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
  }
}