/* https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp */

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

    // public attributes
    uint32_t frame_counter = 0;
    uint8_t brightness = 100;
    // numLEDs

    // public getters
    uint8_t getNSegments();
    uint8_t getLongestSegment();
    Segment* getSegments();
    uint32_t getAllSegments();
    uint32_t getEvenSegments();
    uint32_t getOddSegments();
    uint32_t getFirstSegments(uint8_t n);
    uint32_t getLastSegments(uint8_t n);
    uint32_t getAllPixels();
    uint32_t getEvenPixels();
    uint32_t getOddPixels();
    uint32_t getFirstPixels(uint8_t n);
    uint32_t getLastPixels(uint8_t n);

    // public methods
    // defaults always go in the header file!
    void update_segments(uint8_t segment_starts[]);

    void resetStripe(void);
    void setStripe(uint32_t color);

    void setSegments(uint32_t color, uint32_t active_segments);
    void resetSegments(uint32_t active_segments);
    void setAllSegments(uint32_t color);  
    void setEvenSegments(uint32_t color);  
    void setOddSegments(uint32_t color);  
    void setFirstSegments(uint32_t color, uint8_t n);
    void setLastSegments(uint32_t color, uint8_t n);

    void blinkSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkAllSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkEvenSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkOddSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkFirstSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkLastSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch=0);
    void blinkPoliceSegments(uint16_t frames);

    void setSegmentsPixel(uint32_t color, uint32_t active_segments, uint32_t active_pixel);
    
    void blinkSegmentsPixel(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t active_pixel, uint16_t frames, uint16_t frame_color_switch=0);

    void animateSegments(uint32_t color, uint32_t active_segments, uint32_t init_segments, uint8_t shift_segments, uint16_t frames, uint16_t frames_shift);
    void animateSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_segments, uint8_t shift_segments, uint16_t frames, uint16_t frames_shift);
    void animateSegmentsPixel(uint32_t color, uint32_t active_segments, uint32_t init_pixel, uint8_t shift_pixel, uint16_t frames, uint16_t frames_shift);
    void animateSegmentsPixel(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_pixel, uint8_t shift_pixel, uint16_t frames, uint16_t frames_shift);
    

  private:
    // private attributes
    uint8_t n_segments;
    uint8_t longest_segment;  // n_pixels in the longest segment
    Segment *segments;  // extract array of Segments elements from array of segment_starts

    uint8_t MAX_NUMBER_SEGMENTS       = 32;
    uint8_t MAX_NUMBER_SEGMENT_PIXELS = 32;
    uint32_t ALL_SEGMENTS;  // binary representations
    uint32_t EVEN_SEGMENTS;
    uint32_t ODD_SEGMENTS;
    uint32_t ALL_PIXELS;  // given by longest_segment!!!
    uint32_t EVEN_PIXELS;
    uint32_t ODD_PIXELS;

    // private methods
    void update_longest_segment();
};

// methods

#endif
