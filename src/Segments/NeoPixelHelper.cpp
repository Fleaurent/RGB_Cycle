/* https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp */

#include "NeoPixelHelper.h"

/* class: SegmentedStrip */
/* constructor */
SegmentedStrip::SegmentedStrip(uint16_t n, uint16_t p, neoPixelType t, uint8_t segment_starts[], uint8_t n_segments): Adafruit_NeoPixel(n, p, t) {
  // LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800, segment_starts, n_segments
  this->n_segments = n_segments;
  this->segments = new Segment[n_segments];

  update_segments(segment_starts);
}


/* public methods */
// complete strip
void SegmentedStrip::update(void) {
  frame_counter++;
  show();
}

void SegmentedStrip::setStripe(uint32_t color) {
  fill(color, 0, numLEDs);
}

void SegmentedStrip::resetStripe() {
  setStripe(OFF);
}


// segments
void SegmentedStrip::setSegments(uint32_t color, uint32_t active_segments) {
  for(int i=0; i<n_segments; i++) {
    if(active_segments & (1 << i)) {
      fill(color, segments[i].first, segments[i].count);
    }
  }
}

void SegmentedStrip::resetSegments(uint32_t active_segments) {
  setSegments(OFF, active_segments);
}

void SegmentedStrip::setAllSegments(uint32_t color) {
  setSegments(color, getAllSegments());
}

void SegmentedStrip::setEvenSegments(uint32_t color) {
  setSegments(color, getEvenSegments());
}

void SegmentedStrip::setOddSegments(uint32_t color) {
  setSegments(color, getOddSegments());
}

void SegmentedStrip::setFirstSegments(uint32_t color, uint8_t n) {
  setSegments(color, getFirstSegments(n));
}

void SegmentedStrip::setLastSegments(uint32_t color, uint8_t n) {
  setSegments(color, getLastSegments(n));
}

void SegmentedStrip::blinkSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint16_t frames, uint16_t frame_color_switch) {
  if(frame_color_switch == 0){
    frame_color_switch = frames / 2;
  }
  
  // always set colors (improve: only set when changing?)
  if((frame_counter%frames) < frame_color_switch) {
    setSegments(color1, active_segments);
  }
  else {
    setSegments(color2, active_segments);
  }
}

void SegmentedStrip::blinkAllSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getAllSegments(), frames, frame_color_switch);
}

void SegmentedStrip::blinkEvenSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getEvenSegments(), frames, frame_color_switch);
}

void SegmentedStrip::blinkOddSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getOddSegments(), frames, frame_color_switch);
}

void SegmentedStrip::blinkFirstSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getFirstSegments(n), frames, frame_color_switch);
}

void SegmentedStrip::blinkLastSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getLastSegments(n), frames, frame_color_switch);
}

void SegmentedStrip::blinkPoliceSegments(uint16_t frames) {
  // todo
  // - provide array of segments to apply to
  // - more general function: color a/color b
  static bool redFirst = false;

  // 0. check if color changes
  if(frame_counter%frames == 0){
    // 1. set color status
    redFirst = !redFirst;

    // 2. write colors
    if(redFirst) {
      for(int i=0; i<n_segments; i++) {
        if(i%2) {
          fill(RED(), segments[i].first, segments[i].count);
        }
        else {
          fill(BLUE(), segments[i].first, segments[i].count);
        }
      }
    }
    else {
      for(int i=0; i<n_segments; i++) {
        if(i%2) {
          fill(BLUE(), segments[i].first, segments[i].count);
        }
        else {
          fill(RED(), segments[i].first, segments[i].count);
        }
      }
    }
    
  }
}

void SegmentedStrip::animateSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t frames_shift) {
  uint8_t step = (frame_counter%frames) / frames_shift; 
  uint32_t temp_active_segments = (shift_segments >= 0) ? active_segments & (init_segments << (shift_segments*step)) : active_segments & (init_segments >> (abs(shift_segments)*step));
  setSegments(color2, active_segments);  
  setSegments(color1, temp_active_segments);
}

void SegmentedStrip::animateSegments(uint32_t color, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t frames_shift) {
  animateSegments(color, OFF, active_segments, init_segments, shift_segments, frames, frames_shift);
}


// segments pixel
void SegmentedStrip::setSegmentsPixel(uint32_t color, uint32_t active_segments, uint32_t active_pixel) {
  for(int i=0; i<n_segments; i++) {
    if(active_segments & (1 << i)) {
      for(int j=0; j<segments[i].count; j++) {
        if(active_pixel & (1 << j)) {
          setPixelColor(segments[i].first+j, color); 
        }
      }
    }
  }
}

void SegmentedStrip::blinkSegmentsPixel(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t active_pixel, uint16_t frames, uint16_t frame_color_switch) {
  if(frame_color_switch == 0){
    frame_color_switch = frames / 2;
  }
  
  // always set colors (improve: only set when changing?)
  if((frame_counter%frames) < frame_color_switch) {
    setSegmentsPixel(color1, active_segments, active_pixel);
  }
  else {
    setSegmentsPixel(color2, active_segments, active_pixel);
  }
}

void SegmentedStrip::animateSegmentsPixel(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_pixel, int8_t shift_pixel, uint16_t frames, uint16_t frames_shift) {
  uint8_t step = (frame_counter%frames) / frames_shift; 
  uint32_t temp_active_pixel = (shift_pixel >= 0) ? init_pixel << (shift_pixel*step) : init_pixel >> (abs(shift_pixel)*step);
  setSegments(color2, active_segments);
  setSegmentsPixel(color1, active_segments, temp_active_pixel);
}

void SegmentedStrip::animateSegmentsPixel(uint32_t  color, uint32_t active_segments, uint32_t init_pixel, int8_t shift_pixel, uint16_t frames, uint16_t frames_shift) {
  animateSegmentsPixel(color, OFF, active_segments, init_pixel, shift_pixel, frames, frames_shift);
}

/* colors */
uint32_t SegmentedStrip::color(uint16_t degree) {
  return Adafruit_NeoPixel::ColorHSV(degree*HUE_DEGREE, saturation, brightness);
}

uint32_t SegmentedStrip::WHITE() {
  return Adafruit_NeoPixel::ColorHSV(0, 0, brightness);
}

uint32_t SegmentedStrip::RED() {
  return color(0);
}

uint32_t SegmentedStrip::YELLOW() {
  return color(60);
}

uint32_t SegmentedStrip::GREEN() {
  return color(120);
}

uint32_t SegmentedStrip::CYAN() {
  return color(180);
}

uint32_t SegmentedStrip::BLUE() {
  return color(240);
}

uint32_t SegmentedStrip::MAGENTA() {
  return color(300);
}


/* getter methods */
Segment* SegmentedStrip::getSegments() {
  return segments;
}

uint8_t SegmentedStrip::getNSegments() {
  return n_segments;
}

uint8_t SegmentedStrip::getLongestSegment() {
  return longest_segment;
}

uint8_t SegmentedStrip::getBrightenss() {
  return brightness;
}

uint8_t SegmentedStrip::getSaturation() {
  return saturation;
}

uint32_t SegmentedStrip::getAllSegments() {
  return ALL_SEGMENTS;
}

uint32_t SegmentedStrip::getEvenSegments() {
  return EVEN_SEGMENTS;
}

uint32_t SegmentedStrip::getOddSegments() {
  return ODD_SEGMENTS;
}

uint32_t SegmentedStrip::getFirstSegments(uint8_t n) {
  // ALL_SEGMENTS = 0xFFFFFFFF >> (uint8_t)(MAX_NUMBER_SEGMENTS-n_segments);
  return ALL_SEGMENTS >> (uint8_t)(n_segments-n);
}

uint32_t SegmentedStrip::getLastSegments(uint8_t n) {
  return ALL_SEGMENTS & ~getFirstSegments(n_segments-n);
}

uint32_t SegmentedStrip::getAllPixels() {
  return ALL_PIXELS;
}

uint32_t SegmentedStrip::getEvenPixels() {
  return EVEN_PIXELS;
}

uint32_t SegmentedStrip::getOddPixels() {
  return ODD_PIXELS;
}

uint32_t SegmentedStrip::getFirstPixels(uint8_t n) {
  // ALL_SEGMENTS = 0xFFFFFFFF >> (uint8_t)(MAX_NUMBER_SEGMENT_PIXELS-longest_segment);
  return ALL_PIXELS >> (uint8_t)(longest_segment-n);
}

uint32_t SegmentedStrip::getLastPixels(uint8_t n) {
  return ALL_PIXELS & ~getFirstPixels(longest_segment-n);
}

uint32_t SegmentedStrip::getFrameCounter() {
  return frame_counter;
}

/* setter methods */
void SegmentedStrip::resetFrameCounter(void) {
  frame_counter = 0;
}

void SegmentedStrip::setBrightness(uint8_t b) {
  brightness = b;
}

void SegmentedStrip::setSaturation(uint8_t s) {
  saturation = s;
}

/* private methods */
void SegmentedStrip::update_segments(uint8_t segment_starts[]) {
  // update segments (first&count) + get longest segment
  for(uint8_t i = 0; i < (n_segments - 1); i++) {
    segments[i].first = segment_starts[i];
    segments[i].count = segment_starts[i+1] - segment_starts[i];
  }
  // calculate last segment length based on number of leds!
  segments[n_segments-1].first = segment_starts[n_segments-1];
  segments[n_segments-1].count = numLEDs - segment_starts[n_segments-1];

  // specific segment settings
  ALL_SEGMENTS = 0xFFFFFFFF >> (uint8_t)(MAX_NUMBER_SEGMENTS-n_segments);
  EVEN_SEGMENTS = ALL_SEGMENTS & 0xAAAAAAAA;
  ODD_SEGMENTS  = ALL_SEGMENTS & 0x55555555;

  // pixel specific settings
  update_longest_segment();
  ALL_PIXELS = 0xFFFFFFFF >> (uint8_t)(MAX_NUMBER_SEGMENT_PIXELS-longest_segment);
  EVEN_PIXELS = ALL_PIXELS & 0xAAAAAAAA;
  ODD_PIXELS  = ALL_PIXELS & 0x55555555;
}

void SegmentedStrip::update_longest_segment() {
  longest_segment = segments[0].count;
  for(uint8_t i = 1; i < n_segments; i++) {
    if(segments[i].count > longest_segment) {
      longest_segment = segments[i].count;
    }
  }  
}


/* methods to be replaced */
/*
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
*/