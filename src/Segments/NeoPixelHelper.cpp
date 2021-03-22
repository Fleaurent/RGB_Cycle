#include "NeoPixelHelper.h"

/* class: SegmentedStrip */
/* constructor */
SegmentedStrip::SegmentedStrip(uint16_t n, uint16_t p, neoPixelType t, uint8_t segment_starts[], uint8_t n_segments): Adafruit_NeoPixel(n, p, t) {
  this->n_segments = n_segments;
  this->segments = new Segment[n_segments];

  update_segments(segment_starts);
  update_longest_segment();
}


/* public methods */
void SegmentedStrip::setSegments(uint32_t color, uint32_t active_segments) {
  for(int i=0; i<n_segments; i++) {
    if(active_segments & (1 << i)) {
      fill(color, segments[i].first, segments[i].count);
    }
  }
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

// time based animations
void SegmentedStrip::blinkSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint16_t frames, uint16_t frame_color_switch) {
  if(frame_color_switch == NULL){
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
          fill(RED(brightness), segments[i].first, segments[i].count);
        }
        else {
          fill(BLUE(brightness), segments[i].first, segments[i].count);
        }
      }
    }
    else {
      for(int i=0; i<n_segments; i++) {
        if(i%2) {
          fill(BLUE(brightness), segments[i].first, segments[i].count);
        }
        else {
          fill(RED(brightness), segments[i].first, segments[i].count);
        }
      }
    }
    
  }
}

/* getter methods */
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

/* setter methods */


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
void blinkSegments(uint32_t color1, uint32_t color2, uint32_t delayVal) {
  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(color1, segments[i].first, segments[i].count);
    }
    else {
      strip.fill(color2, segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(delayVal);

  for(int i=0; i<n_segments; i++) {
    if(i%2) {
      strip.fill(color2, segments[i].first, segments[i].count);
    }
    else {
      strip.fill(color1, segments[i].first, segments[i].count);
    }
  }
  strip.show();
  delay(delayVal);
}

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