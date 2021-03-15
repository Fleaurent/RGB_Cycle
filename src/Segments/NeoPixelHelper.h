#ifndef NEOPIXELHELPER_H
#define NEOPIXELHELPER_H

#include <Adafruit_NeoPixel.h>

#define OFF strip.Color(0, 0, 0)

// macro brightness
#define RED(brightness) strip.Color(brightness, 0, 0)
#define GREEN(brightness) strip.Color(0, brightness, 0)
#define BLUE(brightness) strip.Color(0, 0, brightness)
#define YELLOW(brightness) strip.Color(brightness, brightness, 0)
#define MAGENTA(brightness) strip.Color(brightness, 0, brightness)
#define CYAN(brightness) strip.Color(0, brightness, brightness)
#define WHITE(brightness) strip.Color(brightness, brightness, brightness)

struct stripSegment {
  uint8_t first;
  uint8_t count;
};

// ToDo: Class
// members: n_segments, list of segments, LED_COUNT
// methods: animate_all segments, only selected segments, ...
class stripSegments {
  
};
#endif
