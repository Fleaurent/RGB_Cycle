#define DECODE_NEC
#include <IRremote.h>
#define IR_RECEIVE_PIN 2

// remote control codes
#define CHM 0x45
#define CH  0x46
#define CHP 0x47
#define LEFT 0x44
#define RIGHT 0x40
#define PLAY_PAUSE 0x43
#define MINUS 0x7
#define PLUS 0x15
#define EQ 0x9
#define ZERO 0x16
#define HUNDRED 0x19
#define HUNDRED2 0xD
#define ONE 0xC
#define TWO 0x18
#define THREE 0x5E
#define FOUR 0x8
#define FIVE 0x1C
#define SIX 0x5A
#define SEVEN 0x42
#define EIGHT 0x52
#define NINE 0x4A


#include "NeoPixelHelper.h"
#include <Adafruit_NeoPixel.h>

#define LED_PIN 3
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

  IrReceiver.begin(IR_RECEIVE_PIN);

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
  // 0. check IR
  if (IrReceiver.decode()) {
    IrReceiver.resume(); // Enable receiving of the next value
    IRState(IrReceiver.decodedIRData.command);
  }

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
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x7FFF, 200);  // all segments: 0x7FFF = all pixels
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 0, 200);  // all segments: 0x1 = first pixel
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << 14, 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 1 << (segmentStrip.longest_segment - 1), 200);  // all segments: 0x4000 = last pixel 
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), 0x3FF, 0x03C0, 200);  // all segments: 0x03C0 = inner 4 pixels
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getOddPixels(), 200);
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getEvenPixels(), 200);
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200);

  // b) blink 25/75  duty cycle -> repeat after 200 ticks: 50 ticks color1, remaining 150 ticks color2
  // segmentStrip.blinkSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getAllPixels(), 200, 50);

  // 2.3 animations
  // segmentStrip.animateSegments(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);  // 10 stripes
  // segmentStrip.animateSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x1, 1, 500, 50);
  // segmentStrip.animateSegments(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastSegments(1), -1, 500, 50);
  // segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20);  // 15 LEDs per Stripe
  // segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), 0x7, 1, 300, 20); 
  // segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getAllSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 

  // 2.4 combined animations
  // animateEvenOdd();
  animateEvenOdd(600, 300);

  /* 3. update strip at the end of the loop */
  segmentStrip.update();
  delay(10);
}

void IRState(uint16_t command) {
  switch(command) {
      case CHM:
        Serial.println("CH-");
        break;
      case CH:
        Serial.println("CH");
        break;
      case CHP:
        Serial.println("CHP");
        break;
      case LEFT:
        Serial.println("|<<");
        break;
      case RIGHT:
        Serial.println(">>|");
        break;
      case PLAY_PAUSE:
        Serial.println(">||");
        break;
      case MINUS:
        Serial.println("-");
        break;
      case PLUS:
        Serial.println("+");
        break;
      case EQ:
        Serial.println("EQ");
        break;
      case ZERO:
        Serial.println("0");
        break;
      case HUNDRED:
        Serial.println("100+");
        break;
      case HUNDRED2:
        Serial.println("200+");
        break;
      case ONE:
        Serial.println("1");
        break;
      case TWO:
        Serial.println("2");
        break;
      case THREE:
        Serial.println("3");
        break;
      case FOUR:
        Serial.println("4");  
        break;
      case FIVE:
        Serial.println("5");
        break;
      case SIX:
        Serial.println("6");
        break;
      case SEVEN:
        Serial.println("7");
        break;
      case EIGHT:
        Serial.println("8");
        break;
      case NINE:
        Serial.println("9");
        break;
      default:
        Serial.print(command);
        Serial.println("?");
        break;
    }
}

/* combined animations */
void animateEvenOdd(void) {
  segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
}

void animateEvenOdd(uint32_t frames, uint32_t frameswitch) {
  if(segmentStrip.getFrameCounter() % frames < frameswitch) {
    segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
    segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
  }
  else {
    segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getEvenSegments(), segmentStrip.getFirstPixels(3), 1, 300, 20); 
    segmentStrip.animateSegmentsPixel(segmentStrip.RED(), segmentStrip.BLUE(), segmentStrip.getOddSegments(), segmentStrip.getLastPixels(3), -1, 300, 20); 
  }
}