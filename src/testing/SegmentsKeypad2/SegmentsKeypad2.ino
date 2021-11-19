//////////////////////////////
/* 1. Segmented RGB Strip */
#include "NeoPixelHelper.h"

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

byte rowPins[ROWS] = {12, 11, 10, 9};  // {5, 6, 7, 8};
byte colPins[COLS] = {8, 7, 6, 5};  // {9, 10, 11, 12};

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

char animationMode = '0';
char animationSet  = 'A';

#define COLORDEGREECHANGE 15
#define DELAYCHANGE 1
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
  delay(segmentStrip1.getDelay());

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
      // A: decrease color foreground
      segmentStrip1.decreaseColorDegreeForeground(COLORDEGREECHANGE);
      segmentStrip2.decreaseColorDegreeForeground(COLORDEGREECHANGE);
      break;
    case 'B':
      // B: decrease brightness
      // segmentStrip1.decreaseBrightness(10);
      // segmentStrip2.decreaseBrightness(10);
      segmentStrip1.decreaseBrightnessStep();
      segmentStrip2.decreaseBrightnessStep();
      Serial.print(segmentStrip1.getBrightness());
      break;
    case 'C':
      // C: decrease color background
      segmentStrip1.decreaseColorDegreeBackground(COLORDEGREECHANGE);
      segmentStrip2.decreaseColorDegreeBackground(COLORDEGREECHANGE);
      break;
    case 'D':
      // D: decrease delay
      segmentStrip1.decreaseDelay(DELAYCHANGE);
      segmentStrip2.decreaseDelay(DELAYCHANGE);
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
      // A: increase color foreground
      segmentStrip1.increaseColorDegreeForeground(COLORDEGREECHANGE);
      segmentStrip2.increaseColorDegreeForeground(COLORDEGREECHANGE);
      break;
    case 'B':
      // B: increase brightness
      // segmentStrip1.increaseBrightness(10);
      // segmentStrip2.increaseBrightness(10);
      segmentStrip1.increaseBrightnessStep();
      segmentStrip2.increaseBrightnessStep();
      Serial.print(segmentStrip1.getBrightness());
      break;
    case 'C':
      // C: increase color background
      segmentStrip1.increaseColorDegreeBackground(COLORDEGREECHANGE);
      segmentStrip2.increaseColorDegreeBackground(COLORDEGREECHANGE);
      break;
    case 'D':
      // D: increase delay
      segmentStrip1.increaseDelay(DELAYCHANGE);
      segmentStrip2.increaseDelay(DELAYCHANGE);
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
  switch(animationMode) {
    case '0':
      // reset all Segments
      // segmentStrip1.resetStripe();
      // segmentStrip2.resetStripe();
      segmentStrip1.setStripeForeground();
      segmentStrip2.setStripeForeground();
      break;
    case '1':
      // shift pattern back and forth (foreground+background)
      shiftPatternForwardBackwards(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '2':
      // shift pattern back and forth (foreground only)
      shiftPatternForwardBackwards(segmentStrip1.getColorForeground(), OFF);
      break;
    case '3':
      // shift pattern back and forth (only background)
      shiftPatternForwardBackwards(OFF, segmentStrip1.getColorBackground());
      break;
    case '4':
      // animateEvenOdd();
      // animateEvenOdd(600, 300);
      // animateEvenOddInit();
      animateEvenOddInit(720, 360);
      break;
    case '5':
      shiftPattern1(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '6':
      shiftPattern2(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '7':
      shiftPattern3(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '8':
      shiftPattern4(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '9':
      shiftPattern5(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    default:  
      // do nothing
      break;  
  }
}

void applyPatternB(void) {
  switch(animationMode) {
    case '0':
      // reset all Segments
      // segmentStrip1.resetStripe();
      // segmentStrip2.resetStripe();
      segmentStrip1.setStripeForeground();
      segmentStrip2.setStripeForeground();
      break;
    case '1':
      shiftPatternComplete1(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '2':
      shiftPatternComplete2(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '3':
      shiftPatternComplete3(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '4':
      shiftPatternComplete4(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '5':
      shiftPatternComplete5(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground());
      break;
    case '6':
      break;
    case '7':
      break;
    case '8':
      break;
    case '9':
      break;
    default:  
      // do nothing
      break;  
  }
}

void applyPatternC(void) {
  switch(animationMode) {
    case '0':
      // reset all Segments
      // segmentStrip1.resetStripe();
      // segmentStrip2.resetStripe();
      segmentStrip1.setStripeBackground();
      segmentStrip2.setStripeBackground();
      break;
    case '1':
      break;
    case '2':
      blinkGo();
      break;
    case '3':
      break;
    case '4':
      blinkLeft();
      break;
    case '5':
      blinkWarning();
      break;
    case '6':
      blinkRight();
      break;
    case '7':
      break;
    case '8':
      blinkStop();
      break;
    case '9':
      break;
    default:  
      // do nothing
      break;  
  }
}

void applyPatternD(void) {
  // 4. combined animations
  // ToDo: animate increasing speed?
  switch(animationMode) {
    case '0':
      segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 0x7FFF, 100);  // all segments: 0x7FFF = all pixels
      segmentStrip2.blinkPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), 0x3FF, 0x7FFF, 100);  // all segments: 0x7FFF = all pixels
      // segmentStrip1.resetStripe();
      // segmentStrip2.resetStripe();
      break;
    case '1':
      // shift color complete stripe (increasing)
      colorComplete(1, 5, 360);
      break;
    case '2':
      // shift color complete stripe (decreasing)
      colorComplete(-1, 5, 360);
      break;
    case '3':
      // color wheel (increasing)
      colorWheel(1, 1, 360);
      // colorWheel(1, 2, 360); // slower
      break;
    case '4':
      // color wheel (decreasing)
      colorWheel(-1, 1, 360);
      // colorWheel(-2, 1, 360); // faster
      break;
    case '5':
      animateSegmentsRainbow();
      break;
    case '6':
      animateSegmentsRainbow(600, 300);
      break;
    case '7':
      break;
    case '8':
      break;
    case '9':
      break;
    default:  
      // do nothing
      break;  
  }
}


/* animations */
/**
 * @brief   test methods provided directly by NeoPixelHelper
 */
void testPattern(void) {
  // 1. blink complete segments
  // a) blink 50/50 duty cycle
  // segmentStrip1.blinkSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 200);
  // segmentStrip1.blinkSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), 200);
  // segmentStrip1.blinkAllSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 200);
  // segmentStrip1.blinkEvenSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 200);
  // segmentStrip1.blinkOddSegments(segmentStrip1.getColorBackground(), segmentStrip1.getColorForeground(), 200);
  // segmentStrip1.blinkFirstSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 5, 200);
  // segmentStrip1.blinkLastSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 5, 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip1.blinkSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 200, 50);
  
  // 2. blink pixel of segments
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 1 << (segmentStrip1.getLongestSegment() - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getOddPixels(), 200);
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getEvenPixels(), 200);
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip1.blinkPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getAllPixels(), 200, 50);

  // 3. animations
  // segmentStrip1.shiftSegments(segmentStrip1.getColorForeground(), segmentStrip1.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip1.shiftSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip1.shiftSegments(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getLastSegments(1), -1, 500, 50);
  // segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  // segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), 0x7, 1, 300, 20); 
  // segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
  // segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
  // segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20); 

  // 4. play with colors
  // segmentStrip1.animateRainbowStripe(0, 1, 5, 15);
  // segmentStrip1.animateRainbowLEDs(0, 2, 1, 5, 15);
  // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 24, 20, 15);
  // segmentStrip1.animateSegmentsRainbow(0, -24, segmentStrip1.getAllSegments(), 24, 20, 15);
  // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), -24, 20, 15);

  // 5. combined animations
  // animateEvenOdd();
  // animateEvenOdd(600, 300);
  // animateEvenOddInit();
  // animateEvenOddInit(720, 360);
  // animateSegmentsRainbow();
  animateSegmentsRainbow(600, 300);
}

/**
 * @brief   shift simple pattern across the each segment of the complete stripe forward an backwards
 * @param   foreground_color main color shifted across the segments 
 * @param   background_color background color filling all other leds
 */
void shiftPatternForwardBackwards(uint32_t foreground_color, uint32_t background_color) {
  const uint32_t frames = 720;
  const uint32_t frameswitch = 360;

  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    // 1. shift pattern forward
    segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20);  // 360 / 20 = 18 steps
    segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);  // 360 / 20 = 18 steps
    segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20); 
  }
}

/**
 * @brief   spin color wheel on inner 4 Segments
 * @param   color_degree_frame_step color_degree added to all leds every frame_step 
 * @param   animation_frames number of frames the led colors are held constant
 */
void colorWheel(uint16_t color_degree_frame_step, uint16_t color_const_frames, int16_t animation_frames) {
  segmentStrip1.shiftColorSteps(0, 6, 0b00001, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip1.shiftColorSteps(90, 6, 0b00010, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip1.shiftColorSteps(180, 6, 0b00100, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip1.shiftColorSteps(270, 6, 0b01000, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip2.shiftColorSteps(0, 6, 0b00001, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip2.shiftColorSteps(90, 6, 0b00010, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip2.shiftColorSteps(180, 6, 0b00100, color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip2.shiftColorSteps(270, 6, 0b01000, color_degree_frame_step, color_const_frames, animation_frames);
}

/**
 * @brief   color complete stripe in the same color
 * @param   color_degree_frame_step color_degree added to all leds every frame_step 
 * @param   animation_frames number of frames the led colors are held constant
 */
void colorComplete(uint16_t color_degree_frame_step, uint16_t color_const_frames, int16_t animation_frames) {
  segmentStrip1.shiftColorStripe(color_degree_frame_step, color_const_frames, animation_frames);
  segmentStrip2.shiftColorStripe(color_degree_frame_step, color_const_frames, animation_frames);
}

/**
 * @brief   shift simple pattern across each segment of the complete stripe
 * @param   foreground_color main color shifted across the segments 
 * @param   background_color background color filling all other leds
 */
void shiftPattern1(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b11011011011011011, -2, 1, 90, 30);  // 60 / 20 = 3 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b11011011011011011, -2, 1, 90, 30); 
}

void shiftPattern2(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b110011001100110011, -3, 1, 120, 30);  // 80 / 20 = 4 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b110011001100110011, -3, 1, 120, 30); 
}

void shiftPattern3(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b1110111011101110111, -3, 1, 120, 30);  // 80 / 20 = 4 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b1110111011101110111, -3, 1, 120, 30); 
}

void shiftPattern4(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b11100111001110011100, -4, 1, 150, 30);  // 100 / 20 = 5 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b11100111001110011100, -4, 1, 150, 30); 
}

void shiftPattern5(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b111100111100111110011, -5, 1, 180, 30);  // 120 / 20 = 6 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b111100111100111110011, -5, 1, 180, 30); 
}

/**
 * @brief   shift pattern across the complete stripe
 * @param   foreground_color main color shifted across the segments 
 * @param   background_color background color filling all other leds
 */
void shiftPatternComplete1(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b111111111111111, -15, 1, 320, 20);  // 150 / 10 = 15 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b111111111111111, -15, 1, 320, 20); 
}

void shiftPatternComplete2(uint32_t foreground_color, uint32_t background_color) {
  segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b111111111111111, -15, 1, 600, 20);  // 150 / 10 = 15 steps
  segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b111111111111111, -15, 1, 600, 20); 
}

void shiftPatternComplete3(uint32_t foreground_color, uint32_t background_color) {
  const uint32_t frames = 600;
  const uint32_t frameswitch = 300;

  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b111111111111111, -15, 1, 300, 20);  // 150 / 10 = 15 steps
    segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b111111111111111, -15, 1, 300, 20); 
  }
  else {
    segmentStrip1.shiftPatternInit(foreground_color, background_color, segmentStrip1.getAllSegments(), 0b111111111111111, 0, -1, 300, 20);  // 150 / 10 = 15 steps
    segmentStrip2.shiftPatternInit(foreground_color, background_color, segmentStrip2.getAllSegments(), 0b111111111111111, 0, -1, 300, 20); 
  }
}

void shiftPatternComplete4(uint32_t foreground_color, uint32_t background_color) {
  const uint32_t frames = 640;
  const uint32_t frameswitch1 = 160;
  const uint32_t frameswitch2 = 320;
  const uint32_t frameswitch3 = 480;

  if(segmentStrip1.getFrameCounter() % frames < frameswitch1) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b10001, 0b111111111111111, -15, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b10001, 0b111111111111111, -15, 1, 160, 10); 
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, 0, 1, 160, 10); 
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch2) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, -15, 1, 160, 10); 
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b10001, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b10001, 0b111111111111111, 0, 1, 160, 10);
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch3) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, -15, 1, 160, 10); 
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, 0, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, 0, 1, 160, 10);
  }
  else {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, -15, 1, 160, 10); 
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, 0, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, 0, 1, 160, 10); 
  }
}

void shiftPatternComplete5(uint32_t foreground_color, uint32_t background_color) {
  const uint32_t frames = 1600;
  const uint32_t frameswitch1 = 160;
  const uint32_t frameswitch2 = 320;
  const uint32_t frameswitch3 = 480;
  const uint32_t frameswitch4 = 640;
  const uint32_t frameswitch5 = 800;
  const uint32_t frameswitch6 = 960;
  const uint32_t frameswitch7 = 1120;
  const uint32_t frameswitch8 = 1280;
  const uint32_t frameswitch9 = 1440;

  if(segmentStrip1.getFrameCounter() % frames < frameswitch1) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00001, 0b111111111111111, -15, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00001, 0b111111111111111, -15, 1, 160, 10); 
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch2) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, -15, 1, 160, 10); 
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch3) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, -15, 1, 160, 10); 
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch4) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, -15, 1, 160, 10);  
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch5) {
    // a) shift primary color in
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b10000, 0b111111111111111, -15, 1, 160, 10); 
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b10000, 0b111111111111111, -15, 1, 160, 10); 
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch6) {
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00001, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00001, 0b111111111111111, 0, 1, 160, 10);
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch7) {
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00010, 0b111111111111111, 0, 1, 160, 10);
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch8) {
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b00100, 0b111111111111111, 0, 1, 160, 10);
  }
  else if(segmentStrip1.getFrameCounter() % frames < frameswitch9) {
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b01000, 0b111111111111111, 0, 1, 160, 10);
  }
  else {
    // b) shift primary color out
    segmentStrip1.shiftPatternInit(foreground_color, background_color, 0b10000, 0b111111111111111, 0, 1, 160, 10);
    segmentStrip2.shiftPatternInit(foreground_color, background_color, 0b10000, 0b111111111111111, 0, 1, 160, 10);
  }
}

/**
 * @brief   simple blink pattern to show the desired way
 */
void blinkGo() {
  segmentStrip1.blinkAllSegments(segmentStrip1.GREEN(), OFF, 200, 120);
  segmentStrip2.blinkAllSegments(segmentStrip2.GREEN(), OFF, 200, 120);
}

void blinkLeft() {
  segmentStrip1.blinkAllSegments(segmentStrip1.YELLOW(), OFF, 200, 120);
}

void blinkRight() {
  segmentStrip2.blinkAllSegments(segmentStrip2.YELLOW(), OFF, 200, 120);
}

void blinkWarning() {
  segmentStrip1.blinkAllSegments(segmentStrip1.YELLOW(), OFF, 200, 120);
  segmentStrip2.blinkAllSegments(segmentStrip2.YELLOW(), OFF, 200, 120);
}

void blinkStop() {
  segmentStrip1.blinkAllSegments(segmentStrip1.RED(), OFF, 200, 120);
  segmentStrip2.blinkAllSegments(segmentStrip2.RED(), OFF, 200, 120);
}

// evaluate
void animateEvenOdd(void) {
  // 1. shift pattern forward, jump back
  segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
  segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
  segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
  segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
}

void animateEvenOdd(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
    segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
	  segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
    segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), 1, 300, 20); 
    segmentStrip1.shiftPattern(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), -1, 300, 20); 
	  segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), 1, 300, 20); 
    segmentStrip2.shiftPattern(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), -1, 300, 20); 
  }
}

void animateEvenOddInit(void) {
  // 1. shift pattern forward, jump back
  segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);
  segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
  segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20);
}

void animateEvenOddInit(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
    segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20); 
	  segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20); 
    segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20); 
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getEvenSegments(), segmentStrip1.getLastPixels(3), 3, -1, 360, 20);
    segmentStrip1.shiftPatternInit(segmentStrip1.getColorForeground(), segmentStrip1.getColorBackground(), segmentStrip1.getOddSegments(), segmentStrip1.getFirstPixels(3), -3, 1, 360, 20); 
	  segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getEvenSegments(), segmentStrip2.getLastPixels(3), 3, -1, 360, 20);
    segmentStrip2.shiftPatternInit(segmentStrip2.getColorForeground(), segmentStrip2.getColorBackground(), segmentStrip2.getOddSegments(), segmentStrip2.getFirstPixels(3), -3, 1, 360, 20);
  }
}

void animateSegmentsRainbow(void) {
  // 1. shift pattern forward, jump back
  // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getEvenSegments(), 6, 5, 60);
  // segmentStrip1.animateSegmentsRainbow(0, -24, segmentStrip1.getOddSegments(), 6, 5, 60);
  // segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getEvenSegments(), 6, 5, 60);
  // segmentStrip2.animateSegmentsRainbow(0, -24, segmentStrip2.getOddSegments(), 6, 5, 60);
  segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 6, 5, 60);
  segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), 6, 5, 60);
}

void animateSegmentsRainbow(uint32_t frames, uint32_t frameswitch) {
  // 1. shift pattern forward
  if(segmentStrip1.getFrameCounter() % frames < frameswitch) {
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 6, 5, 60);
    segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), 6, 5, 60);
    // segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), -6, 5, 60);
  }
  else {
    // 2. shift pattern backwards
    segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), -6, 5, 60);
    segmentStrip2.animateSegmentsRainbow(0, 24, segmentStrip2.getAllSegments(), -6, 5, 60);
    // segmentStrip1.animateSegmentsRainbow(0, 24, segmentStrip1.getAllSegments(), 6, 5, 60);
  }
}
