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
class SegmentedStrip : public Adafruit_NeoPixel {
  public:
    // constructor
    SegmentedStrip(uint16_t n, uint16_t p, neoPixelType t, uint8_t segment_starts[], uint8_t n_segments);

    // attributes
    uint8_t n_segments;
    uint8_t longest_segment;
    Segment *segments;
    uint32_t frame_counter = 0;
    uint8_t brightness = 100;

    // methods
    void blinkPoliceSegments(uint16_t frames);
    void blinkSegments(uint16_t frames, uint32_t color1, uint32_t color2, uint32_t active_segments);
    void setSegments(uint32_t color, uint32_t active_segments);

  private:
    // attributes
    // methods
    void update_segments(uint8_t segment_starts[]);
    void update_longest_segment();
};


// methods

#endif
