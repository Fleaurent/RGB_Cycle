#include "NeoPixelHelper.h"

// class: SegmentedStrip
// constructor
SegmentedStrip::SegmentedStrip(uint8_t segment_starts[], uint8_t n_segments, uint16_t n, uint16_t p, neoPixelType t): Adafruit_NeoPixel(n, p, t) {
  this->n_segments = n_segments;
  update_segments(segment_starts);
  update_longest_segment();
}

// public methods

// private methods
void SegmentedStrip::update_segments(uint8_t segment_starts[]) {
  // update segments: first&count
  // -> get longest segment
  this->segments = new Segment[this->n_segments];
  for(uint8_t i = 0; i < (this->n_segments - 1); i++) {
    this->segments[i].first = segment_starts[i];
    this->segments[i].count = this->segments[i+1].first - this->segments[i].first;
  }
  // calculate last segment length based on number of leds!
  this->segments[this->n_segments].first = segment_starts[this->n_segments];
  this->segments[this->n_segments].count = numLEDs - this->segments[this->n_segments].first;
}

void SegmentedStrip::update_longest_segment() {
  this->longest_segment = this->segments[0].count;
  for(uint8_t i = 1; i < this->n_segments; i++) {
    if(this->segments[i].count > this->longest_segment) {
      this->longest_segment = this->segments[i].count;
    }
  }  
}


// methods
void SegmentedStrip::blinkPoliceSegments(uint8_t brightness) {
  // todo
  // - save state: toggle based on state
  // - provide array: stripes color a/color b/ unchanged
  // - only call strip show at the end of loop
  for(int i=0; i<this->n_segments; i++) {
    if(i%2) {
      this->fill(RED(brightness), this->segments[i].first, 15); // this->segments[i].count);
    }
    else {
      this->fill(BLUE(brightness), this->segments[i].first, 15); //this->segments[i].count);
    }
  }
  this->show();
  delay(500);

  for(int i=0; i<this->n_segments; i++) {
    if(i%2) {
      this->fill(BLUE(brightness), this->segments[i].first, 15); //this->segments[i].count);
    }
    else {
      this->fill(RED(brightness), this->segments[i].first, 15); //this->segments[i].count);
    }
  }
  this->show();
  delay(500);
}

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