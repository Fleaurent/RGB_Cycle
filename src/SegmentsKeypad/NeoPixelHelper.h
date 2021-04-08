/* https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp */

#ifndef NEOPIXELHELPER_H
#define NEOPIXELHELPER_H

#include <Adafruit_NeoPixel.h>

#define HUE_DEGREE 182
#define OFF Adafruit_NeoPixel::Color(0, 0, 0)

struct Segment {
  uint8_t first;
  uint8_t count;
};

class SegmentedStrip : public Adafruit_NeoPixel {
  public:
    // constructor
    SegmentedStrip(uint16_t n, uint16_t p, neoPixelType t, uint8_t segment_starts[], uint8_t n_segments);

    // public attributes
    // numLEDs -> given by parent class

    // public methods
    // defaults always go in the header file!
    // complete strip
    void update(void);
    void setStripe(uint32_t color);
    void resetStripe(void);

    // segments
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

    void shiftSegments(uint32_t color, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t frames_shift);
    void shiftSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t frames_shift);

    // segments pixel pattern
    void setPattern(uint32_t color, uint32_t active_segments, uint32_t active_pixel);
  
    void blinkPattern(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t active_pixel, uint16_t frames, uint16_t frame_color_switch=0);

    void shiftPattern(uint32_t color, uint32_t active_segments, uint32_t init_pixel, int8_t shift_pixel, uint16_t frames, uint16_t frames_shift);
    void shiftPattern(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_pixel, int8_t shift_pixel, uint16_t frames, uint16_t frames_shift);
    void shiftPatternInit(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t pixel_pattern, int8_t init_shift_pixel, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames);
    void shiftPatternInit(uint32_t color, uint32_t active_segments, uint32_t pixel_pattern, int8_t init_shift_pixel, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames);

    // play with colors
    void setSegmentsRainbow(uint16_t color_degree_start, uint16_t color_degree_led_step, uint32_t active_segments);
    void animateSegmentsRainbow(uint16_t color_degree_start, uint16_t color_degree_led_step, uint32_t active_segments, uint16_t color_degree_frame_step, uint16_t animation_frames);
    void animateRainbowStripe(uint16_t color_degree_start, uint16_t color_degree_frame_step, uint16_t animation_frames);
    void animateRainbowLEDs(uint16_t color_degree_start, uint16_t color_degree_led_step, uint16_t color_degree_frame_step, uint16_t animation_frames);
    

    // colors
    uint32_t color(uint16_t degree);
    uint32_t WHITE(void);
    uint32_t RED(void);
    uint32_t YELLOW(void);
    uint32_t GREEN(void);
    uint32_t CYAN(void);
    uint32_t BLUE(void);
    uint32_t MAGENTA(void);

    // public getters
    Segment* getSegments();
    uint8_t getNSegments();
    uint8_t getLongestSegment();
    uint32_t getFrameCounter();
    uint8_t getBrightness();
    uint8_t getSaturation();

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
    
    // public setters
    void resetFrameCounter(void);
    void setBrightness(uint8_t);
    void increaseBrightness(uint8_t);
    void decreaseBrightness(uint8_t);
    void increaseBrightnessStep(void);
    void decreaseBrightnessStep(void);
    void setSaturation(uint8_t);


  private:
    // private attributes
    Segment *segments;  // extract array of Segments elements from array of segment_starts
    uint8_t n_segments;
    uint8_t longest_segment;  // n_pixels in the longest segment
    uint32_t frame_counter = 0;
    uint8_t brightness = 100;
    uint8_t brightnessSteps[11] = {0, 5, 10, 20, 30, 50, 75, 100, 150, 200, 255};
    uint8_t nBrightnessSteps = 10;  // -1: do not count 0!
    uint8_t brightnessStep = 5;
    uint8_t saturation = 255;

    uint8_t MAX_NUMBER_SEGMENTS       = 32;
    uint8_t MAX_NUMBER_SEGMENT_PIXELS = 32;
    uint32_t ALL_SEGMENTS;  // binary representations
    uint32_t EVEN_SEGMENTS;
    uint32_t ODD_SEGMENTS;
    uint32_t ALL_PIXELS;  // given by longest_segment!!!
    uint32_t EVEN_PIXELS;
    uint32_t ODD_PIXELS;

    // private methods
    void update_segments(uint8_t segment_starts[]);
    void update_longest_segment();
};

// methods

#endif
