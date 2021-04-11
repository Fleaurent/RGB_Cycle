//////////////////////////////
/* 1. Segmented RGB Strip */
#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN_1 2
#define LED_PIN_2 3

// declare segments
#define LED_COUNT 75
uint8_t segment_starts[] = {0,
                            15, 
                            30, 
                            45, 
                            60};
uint8_t n_segments = sizeof(segment_starts) / sizeof(segment_starts[0]);

// declare strip object
SegmentedStrip segmentStrip1(LED_COUNT, LED_PIN_1, NEO_GRB + NEO_KHZ800, segment_starts, n_segments);
SegmentedStrip segmentStrip2(LED_COUNT, LED_PIN_2, NEO_GRB + NEO_KHZ800, segment_starts, n_segments);
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
  // init hardware
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);

  // init strips: set all pixels 'off'
  segmentStrip1.begin();
  segmentStrip2.begin();
  
  segmentStrip1.show(); 
  segmentStrip2.show(); 
  
  printStripInformation(&segmentStrip1);
  printStripInformation(&segmentStrip2);

  // ready to rumble!
  Serial.println("Running...");
}


void loop() {
  /* 1. get input, set brightness */
  updateKeypad();
  
  /* 2. apply pattern based on input */
  applyPattern();
  // testPattern();
  
  /* 3. update strip at the end of the loop */
  segmentStrip1.update();
  segmentStrip2.update();
  // delay(segmentStrip.getDelay());

  /* optional: blink led */
  blink();
}

void blink(void) {
  const uint16_t blinkInterval = 500;
  static unsigned long previousMillis = millis(); 

  unsigned long currentMillis = millis();
  if(currentMillis - previousMillis > blinkInterval) {
    previousMillis = currentMillis;
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
}

void printStripInformation(SegmentedStrip *strip) {
  Serial.println(strip->getNSegments());
  Serial.println(strip->getLongestSegment());
  Serial.println(strip->getBrightness());

  for(uint8_t i = 0; i < strip->getNSegments(); i++) {
    Serial.print(strip->getSegments()[i].first);
    Serial.print('\t');
    Serial.println(strip->getSegments()[i].count);
  }
}

void updateKeypad(void) {
  // 1. get keypad input
  char customKey = customKeypad.getKey();
  
  // 2. only update settings if key pressed
  if (customKey) {
    // 2.1 print keypad debug output:
    Serial.print(segmentStrip1.getFrameCounter());
    Serial.print(' ');
    Serial.print(customKey);
    Serial.print(' ');

    // 2.2. update settings
    switch(customKey) {
      case '*':
        // */#: update brightenss or frequency
        applyAsteriks();
        break;
      case '#':
        // */#: update brightenss or frequency
        applyHashkey();
        break;
      case 'A':
      case 'B':
      case 'C':
      case 'D':
        // A-D: update animationSet
        animationSet = customKey;
        break;
      case '0':
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
      case '9':
        // 0-9: update animationMode
        animationMode = customKey;
        break;
      default:
        // do nothing
        break;
    }

    // reset current pixels
    segmentStrip1.resetStripe();
    segmentStrip2.resetStripe();

    Serial.println();  // keypad debug output: end line
  }
}

void applyAsteriks(void) {
  switch(animationSet) {
    case 'A':
    case 'B':
    case 'C':
      // A/B/C: decrease brightness
      // segmentStrip1.decreaseBrightness(10);
      // segmentStrip2.decreaseBrightness(10);
      segmentStrip1.decreaseBrightnessStep();
      segmentStrip2.decreaseBrightnessStep();
      Serial.print(segmentStrip1.getBrightness());
      break;
    case 'D':
      // D: decrease delay
      segmentStrip1.decreaseDelay(1);
      Serial.print(segmentStrip1.getDelay());
      break;
    default:
      // do nothing
      break;
  }
}

void applyHashkey(void) {
  switch(animationSet) {
    case 'A':
    case 'B':
    case 'C':
      // A/B/C: increase brightness
      // segmentStrip1.increaseBrightness(10);
      // segmentStrip2.increaseBrightness(10);
      segmentStrip1.increaseBrightnessStep();
      segmentStrip2.increaseBrightnessStep();
      Serial.print(segmentStrip1.getBrightness());
      break;
    case 'D':
      // D: increase delay
      segmentStrip1.increaseDelay(1);
      Serial.print(segmentStrip1.getDelay());
      break;
    default:
      // do nothing
      break;
  }
}

void applyPattern(void) {
  switch(animationSet) {
    case 'A':
      applyPatternA();
      break;
    case 'B':
      applyPatternB();
      break;
    case 'C':
      applyPatternC();
      break;
    case 'D':
      applyPatternD();
      break;
    default:
      // do nothing
      break;
  }
}

/*
 Patterns:  
 - blink each segment after another
 - run animation on each segment after another
 - run animation on all segments at the same time
 - light up all pixels after another on each segment after another
 - light up all pixels after another on all segments at the same time
 - blink different on each side
 - animate rolling on 1-4 segment
*/
void applyPatternA(void) {
  // 1. blink complete segments
  /*
    // blink all Segments (background)
    segmentStrip1.blinkAllSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 200);
    segmentStrip2.blinkAllSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), 200);

    // blink even Segments (background)
    segmentStrip1.blinkEvenSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 200);
    segmentStrip2.blinkEvenSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), 200);

    // blink odd Segments (background)
    segmentStrip1.blinkOddSegments(segmentStrip1.BLUE(), segmentStrip1.RED(), 200);
    segmentStrip2.blinkOddSegments(segmentStrip2.BLUE(), segmentStrip2.RED(), 200);

    // blink first 3 Segments (background)
    segmentStrip1.blinkFirstSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 3, 200);
    segmentStrip2.blinkFirstSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), 3, 200);

    // blink last 3 Segments (background)
    segmentStrip1.blinkLastSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 3, 200);
    segmentStrip2.blinkLastSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), 3, 200);

    // animate Segments (background)
    segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 0x1, 1, 250, 50);
    segmentStrip2.shiftSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 0x1, 1, 250, 50);

    // animate Segments (background, reversed)
    segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastSegments(1), -1, 250, 50);
    segmentStrip2.shiftSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getLastSegments(1), -1, 250, 50);
  */
  switch(animationMode) {
    case '0':
      // reset all Segments
      segmentStrip1.resetStripe();
      segmentStrip2.resetStripe();
      break;
    case '1':
      // blink all Segments
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.getAllSegments(), 200);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.getAllSegments(), 200);
      break;
    case '2':
      // blink even Segments
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.getEvenSegments(), 200);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.getEvenSegments(), 200);
      break;
    case '3':
      // blink odd Segments
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.getOddSegments(), 200);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.getOddSegments(), 200);
      break;
    case '4':
      // blink first 3 Segments
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.getFirstSegments(3), 200);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.getFirstSegments(3), 200);
      break;
    case '5':
      // blink last 3 Segments
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.getLastSegments(3), 200);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.getLastSegments(3), 200);
      break;
    case '6':
      // blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
      segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 200, 50);
      segmentStrip2.blinkSegments(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 200, 50);
      break;
    case '7':
      // blink alternating on each side
      segmentStrip1.blinkAllSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 200);
      segmentStrip2.blinkAllSegments(segmentStrip2.BLUE(), segmentStrip2.RED(), 200);
      break;
    case '8':
      // animate segments without background
      segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.getAllSegments(), 0x1, 1, 250, 50); 
      segmentStrip2.shiftSegments(segmentStrip2.RED(), segmentStrip2.getAllSegments(), 0x1, 1, 250, 50);
      break;
    case '9':
      // animate segments without background (reversed)
      segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.getAllSegments(), segmentStrip1.getLastSegments(1), -1, 250, 50); 
      segmentStrip2.shiftSegments(segmentStrip2.RED(), segmentStrip2.getAllSegments(), segmentStrip2.getLastSegments(1), -1, 250, 50);
      break;
    default:  // do nothing
      break;  
  }
}

void applyPatternB(void) {
  // 2. blink pixel of segments
  /*
    // all segments, all pixels
    segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200);  
    segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getAllPixels(), 200);

    // all segments, first pixel
    segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 1 << 0, 200);  
    segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 1 << 0, 200);  // all segments: 0x1 = first pixel

    // all segments, last pixel 
    segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 1 << (segmentStrip1.getLongestSegment() - 1), 200);  
    segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 1 << (segmentStrip2.getLongestSegment() - 1), 200);

    // blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
    segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200, 50);
    segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getAllPixels(), 200, 50);

    // shift three pixels in and out (background)
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20);  // 360 / 20 = 18 steps
    segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 

    // shift three pixels in and out (reversed, background) 
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);  // 360 / 20 = 18 steps
    segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20); 
  */
  switch(animationMode) {
    case '0':
      segmentStrip1.resetStripe();
      segmentStrip2.resetStripe();
      break;
    case '1':
      // all segments, even pixels
      segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getEvenPixels(), 200);
      segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getEvenPixels(), 200);
      break;
    case '2':
      // all segments, odd pixels
      segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getOddPixels(), 200);
      segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getOddPixels(), 200);
      break;
    case '3':
      // all segments, first 8 pixels
      segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(8), 200);
      segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getFirstPixels(8), 200);
      break;
    case '4':
      // all segments, last 8 pixels
      segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(8), 200);
      segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getLastPixels(8), 200);
      break;
    case '5':
      // all segments, inner pixels
      segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 0x03E0, 200); 
      segmentStrip2.blinkPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 0x03E0, 200); 
      break;
    case '6':
      // shift three pixels in and out
      segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20);  // 360 / 20 = 18 steps
      segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.getAllSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
      break;
    case '7':
      // shift three pixels in and out (reversed)
      segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);  // 360 / 20 = 18 steps
      segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.getAllSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20); 
      break;
    case '8':
      // todo: shift back and forth
      break;
    case '9':
      
      break;
    default:  // do nothing
      break;  
  }
}

void applyPatternC(void) {
  // 3. animations
  switch(animationMode) {
    case '0':
      segmentStrip1.resetStripe();
      segmentStrip2.resetStripe();
      break;
    case '1':
      
      break;
    case '2':
      
      break;
    case '3':
      
      break;
    case '4':
      segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
      segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
      break;
    case '5':
      segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20); 
      segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), 0x7, 1, 300, 20); 
      break;
    case '6':
     segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
     segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getAllSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
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

void applyPatternD(void) {
  // 4. play with colors
  // 5. combined animations
  switch(animationMode) {
    case '0':
      segmentStrip1.resetStripe();
      segmentStrip2.resetStripe();
      break;
    case '1':
      segmentStrip1.animateRainbowStripe(0, 1, 5);
      segmentStrip2.animateRainbowStripe(0, 1, 5);
      break;
    case '2':
      segmentStrip1.animateRainbowLEDs(0, 2, 1, 5);
      segmentStrip2.animateRainbowLEDs(0, 2, 1, 5);
      break;
    case '3':
      segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 24, 20);
      segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), 24, 20);
      break;
    case '4':
      segmentStrip1.animateSegmentsRainbow(0, -24, segmentStrip1.getAllSegments(), 24, 20);
      segmentStrip2.animateSegmentsRainbow(0, -24, segmentStrip2.getAllSegments(), 24, 20);
      break;
    case '5':
      segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), -24, 20);
      segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), -24, 20);
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
  // segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 200);
  // segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 200);
  // segmentStrip1.blinkAllSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 200);
  // segmentStrip1.blinkEvenSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 200);
  // segmentStrip1.blinkOddSegments(segmentStrip1.BLUE(), segmentStrip1.RED(), 200);
  // segmentStrip1.blinkFirstSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 5, 200);
  // segmentStrip1.blinkLastSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 5, 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip1.blinkSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 200, 50);
  
  // 2. blink pixel of segments
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 1 << (segmentStrip1.getLongestSegment() - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getOddPixels(), 200);
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getEvenPixels(), 200);
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip1.blinkPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200, 50);

  // 3. animations
  // segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip1.shiftSegments(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastSegments(1), -1, 500, 50);
  // segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  // segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20); 
  // segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
  // segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
  // segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20); 

  // 4. play with colors
  // segmentStrip1.animateRainbowStripe(0, 1, 5);
  // segmentStrip1.animateRainbowLEDs(0, 2, 1, 5);
  // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 24, 20);
  // segmentStrip1.animateSegmentsRainbow(0, -24, segmentStrip1.getAllSegments(), 24, 20);
  // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), -24, 20);

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
  segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
  segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
  segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
  segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
}

void animateEvenOdd(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
    segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
	  segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
    segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
    segmentStrip1.shiftPattern(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
	  segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
    segmentStrip2.shiftPattern(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
  }
}

void animateEvenOddInit(void) {
  // 1. shift pattern forward, jump back
  segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);
  segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20);
}

void animateEvenOddInit(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20); 
	  segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
    segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);
    segmentStrip1.shiftPatternInit(segmentStrip1.RED(), segmentStrip1.BLUE(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
	  segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20);
    segmentStrip2.shiftPatternInit(segmentStrip2.RED(), segmentStrip2.BLUE(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20);
  }
}

void animateEvenOddRainbow(void) {
  // 1. shift pattern forward, jump back
  segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getEvenSegments(), 24, 20);
  segmentStrip1.animateSegmentsRainbow(0, -24, segmentStrip1.getOddSegments(), 24, 20);
  segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getEvenSegments(), 24, 20);
  segmentStrip2.animateSegmentsRainbow(0, -24, segmentStrip2.getOddSegments(), 24, 20);
}

void animateEvenOddRainbow(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getEvenSegments(), 6, 5);
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getOddSegments(), -6, 5);
	  segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getEvenSegments(), 6, 5);
    segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getOddSegments(), -6, 5);
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getEvenSegments(), -6, 5);
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getOddSegments(), 6, 5);
	segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getEvenSegments(), -6, 5);
    segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getOddSegments(), 6, 5);
  }
}