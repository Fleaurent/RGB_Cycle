#ifndef NEOPIXELHELPER_H
#define NEOPIXELHELPER_H

#include <Adafruit_NeoPixel.h>

#define OFF Adafruit_NeoPixel::Color(0, 0, 0)

// macro brightness
#define RED(brightness) Adafruit_NeoPixel::Color(brightness, 0, 0)
#define GREEN(brightness) Adafruit_NeoPixel::Color(0, brightness, 0)
#define BLUE(brightness) Adafruit_NeoPixel::Color(0, 0, brightness)
#define YELLOW(brightness) Adafruit_NeoPixel::Color(brightness, brightness, 0)
#define MAGENTA(brightness) Adafruit_NeoPixel::Color(brightness, 0, brightness)
#define CYAN(brightness) Adafruit_NeoPixel::Color(0, brightness, brightness)
#define WHITE(brightness) Adafruit_NeoPixel::Color(brightness, brightness, brightness)

struct Segment {
  uint8_t first;
  uint8_t count;
};

// ToDo: Class
// members: n_segments, list of segments, LED_COUNT
// methods: animate_all segments, only selected segments, ...
class SegmentedStrip {
  public:
    // constructor
    SegmentedStrip(uint8_t n_segments, uint8_t longest_segment, Segment *segments);

    // attributes
    uint8_t n_segments;
    uint8_t longest_segment;
    Segment *segments;

    // methods

  private:
    // attributes
    // methods
};


// methods
void blinkPoliceSegments(uint8_t brightness, Adafruit_NeoPixel *strip, SegmentedStrip *segments);
#endif
