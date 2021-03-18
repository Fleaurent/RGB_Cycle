#include "NeoPixelHelper.h"

// class
SegmentedStrip::SegmentedStrip(uint8_t n_segments, uint8_t longest_segment, Segment *segments) {
  this->n_segments = n_segments;
  this->longest_segment = longest_segment;
  this->segments = segments;
}

// methods
void blinkPoliceSegments(uint8_t brightness, Adafruit_NeoPixel *strip, SegmentedStrip *segmentStrip) {
  // todo
  // - save state: toggle based on state
  // - provide array: stripes color a/color b/ unchanged
  // - only call strip show at the end of loop
  for(int i=0; i<segmentStrip->n_segments; i++) {
    if(i%2) {
      strip->fill(RED(brightness), segmentStrip->segments[i].first, segmentStrip->segments[i].count);
    }
    else {
      strip->fill(BLUE(brightness), segmentStrip->segments[i].first, segmentStrip->segments[i].count);
    }
  }
  strip->show();
  delay(500);

  for(int i=0; i<segmentStrip->n_segments; i++) {
    if(i%2) {
      strip->fill(BLUE(brightness), segmentStrip->segments[i].first, segmentStrip->segments[i].count);
    }
    else {
      strip->fill(RED(brightness), segmentStrip->segments[i].first, segmentStrip->segments[i].count);
    }
  }
  strip->show();
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