//////////////////////////////
/* 1. LED Segmented Strip */
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
//////////////////////////////

//////////////////////////////
/* 2. Keypad */
#include <Keypad.h>

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

byte rowPins[ROWS] = {12, 11, 10, 9}; 
byte colPins[COLS] = {8, 7, 6, 5}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char animationMode = '1';
char animationSet  = 'A';
//////////////////////////////


void setup() {
  Serial.begin(115200);

  // initialize strip: set all pixels 'off'
  segmentStrip.begin();
  segmentStrip.show(); 
  
  printStripInformation();
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
  updateKeypad();
  
  // 2. apply pattern based on input
  applyPattern();
  // testPattern();
  
  /* 3. update strip at the end of the loop */
  segmentStrip.update();
}

void printStripInformation(void) {
  Serial.println(segmentStrip.getNSegments());
  Serial.println(segmentStrip.getLongestSegment());
  Serial.println(segmentStrip.getBrightness());

  for(uint8_t i = 0; i < n_segments; i++) {
    Serial.print(segmentStrip.getSegments()[i].first);
    Serial.print('\t');
    Serial.println(segmentStrip.getSegments()[i].count);
  }
}

void updateKeypad(void){
  char customKey = customKeypad.getKey();
  
  if (customKey) {
    Serial.print(segmentStrip.getFrameCounter());
    Serial.print(' ');
    Serial.print(customKey);
    Serial.print(' ');

    switch(customKey) {
      case '*':
        // */#: update brightenss
        segmentStrip.decreaseBrightness(10);
        Serial.print(segmentStrip.getBrightness());
        break;
      case '#':
        // */#: update brightenss
        segmentStrip.increaseBrightness(10);
        Serial.print(segmentStrip.getBrightness());
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
        // A-D: update animation set
        animationSet = customKey;
        break;
      default:
        // 0-9: update animation animationMode
        animationMode = customKey;
        break;
    }

     Serial.println();
  }
}

void applyPattern(void){
  if(animationSet == 'A') {
    applyPatternA();
  }
  else if(animationSet == 'B') {
    applyPatternB();
  }
  else if(animationSet == 'C') {
    applyPatternC();
  }
  else if(animationSet == 'D') {
    applyPatternD();
  }
}

void applyPatternA(void) {
  // 1. blink complete segments
  switch(animationMode) {
    case '0':
      break;
    case '1':
      segmentStrip.blinkAllSegments(segmentStrip.RED(), segmentStrip.BLUE(), 200);
      break;
    case '2':
      segmentStrip.blinkEvenSegments(segmentStrip.RED(), segmentStrip.BLUE(), 200);
      break;
    case '3':
      segmentStrip.blinkOddSegments(segmentStrip.BLUE(), segmentStrip.RED(), 200);
      break;
    case '4':
      segmentStrip.blinkFirstSegments(segmentStrip.RED(), segmentStrip.BLUE(), 5, 200);
      break;
    case '5':
      segmentStrip.blinkLastSegments(segmentStrip.RED(), segmentStrip.BLUE(), 5, 200);
      break;
    case '6':
      // blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
      segmentStrip.blinkSegments(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 200, 50);
      break;
    case '7':
      break;
    case '8':
      break;
    case '9':
      break;
    default:  // do nothing
      break;  
  }
}

void applyPatternB(void) {
  // 2. blink pixel of segments
  switch(animationMode) {
    case '0':
      break;
    case '1':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
      break;
    case '2':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
      break;
    case '3':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
      break;
    case '4':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << (segmentStrip.getLongestSegment() - 1), 200);  // all segments: 0x4000 = last pixel 
      break;
    case '5':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
      break;
    case '6':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getOddPixels(), 200);
      break;
    case '7':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getEvenPixels(), 200);
      break;
    case '8':
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200);
      break;
    case '9':
      // blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
      segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200, 50);
      break;
    default:  // do nothing
      break;  
  }
}

void applyPatternC(void) {
  // 3. animations
  switch(animationMode) {
    case '0':
      break;
    case '1':
      segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
      break;
    case '2':
      segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);
      break;
    case '3':
      segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastSegments(1), -1, 500, 50);
      break;
    case '4':
      segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
      break;
    case '5':
      segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20); 
      break;
    case '6':
     segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
      break;
    case '7':
      segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
      break;
    case '8':
      segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20); 
      break;
    case '9':
      break;
    default:  // do nothing
      break;  
  }
}

void applyPatternD(void) {
  // 4. play with colors
  // 5. combined animations
  switch(animationMode) {
    case '0':
      break;
    case '1':
     segmentStrip.animateRainbowStripe(0, 1, 5);
      break;
    case '2':
      segmentStrip.animateRainbowLEDs(0, 2, 1, 5);
      break;
    case '3':
      segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getAllSegments(), 24, 20);
      break;
    case '4':
      segmentStrip.animateSegmentsRainbow(0, -24, segmentStrip.getAllSegments(), 24, 20);
      break;
    case '5':
      segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getAllSegments(), -24, 20);
      break;
    case '6':
      // animateEvenOdd();
      animateEvenOdd(600, 300);
      break;
    case '7':
      // animateEvenOddInit();
      animateEvenOddInit(720, 360);
      break;
    case '8':
      // animateEvenOddRainbow();
      animateEvenOddRainbow(600, 300);
      break;
    case '9':
      break;
    default:  // do nothing
      break;  
  }
}

void testPattern(void) {
  // 1. blink complete segments
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
  
  // 2. blink pixel of segments
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << (segmentStrip.getLongestSegment() - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getOddPixels(), 200);
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getEvenPixels(), 200);
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200, 50);

  // 3. animations
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip.shiftSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastSegments(1), -1, 500, 50);
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20); 
  // segmentStrip.shiftPattern(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  // segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getFirstPixels(3), -3, 1, 360, 20); 
  // segmentStrip.shiftPatternInit(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), 3, -1, 360, 20); 

  // 4. play with colors
  // segmentStrip.animateRainbowStripe(0, 1, 5);
  // segmentStrip.animateRainbowLEDs(0, 2, 1, 5);
  // segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getAllSegments(), 24, 20);
  // segmentStrip.animateSegmentsRainbow(0, -24, segmentStrip.getAllSegments(), 24, 20);
  // segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getAllSegments(), -24, 20);

  // 5. combined animations
  // animateEvenOdd();
  // animateEvenOdd(600, 300);
  // animateEvenOddInit();
  // animateEvenOddInit(720, 360);
  // animateEvenOddRainbow();
  animateEvenOddRainbow(600, 300);
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

void animateEvenOddRainbow(void) {
  // 1. shift pattern forward, jump back
  segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getEvenSegments(), 24, 20);
  segmentStrip.animateSegmentsRainbow(0, -24, segmentStrip.getOddSegments(), 24, 20);
}

void animateEvenOddRainbow(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip.getFrameCounter() % frames < frameswitch) {
    segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getEvenSegments(), 6, 5);
    segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getOddSegments(), -6, 5);
  }
  else {
    // 2. shift pattern backwards
    segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getEvenSegments(), -6, 5);
    segmentStrip.animateSegmentsRainbow(0, 24, segmentStrip.getOddSegments(), 6, 5);
  }
}