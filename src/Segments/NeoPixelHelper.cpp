/* https://github.com/adafruit/Adafruit_NeoPixel/blob/master/Adafruit_NeoPixel.cpp */

#include "NeoPixelHelper.h"

/* class: SegmentedStrip */
/* constructor */
/**
 * @brief   NeoPixelHelper constructor when length, pin, pixel type and segments are known
 *          at compile-time.
 *          -> LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800, segment_starts, n_segments
 * @param   n  Number of RGB-LEDs in strip.
 * @param   p  Arduino pin number which will drive the NeoPixel data in.
 * @param   t  Pixel type e.g. NEO_GRB+NEO_KHZ800
 * @param   segment_starts[] array of led numbers: each entry defines a new segment
 * @param   n_segments number of segments i.e. length of segments_starts array
 * @return  Adafruit_NeoPixel object. Call the begin() function before use.
 */
SegmentedStrip::SegmentedStrip(uint16_t n, uint16_t p, neoPixelType t, uint8_t segment_starts[], uint8_t n_segments): Adafruit_NeoPixel(n, p, t) {
  this->n_segments = n_segments;
  this->segments = new Segment[n_segments];

  update_segments(segment_starts);
}


/* public methods */
/**
 * @brief   update all pixels of the strip: 
 *          transmit pixel data in RAM to NeoPixels
 */
void SegmentedStrip::update(void) {
  frame_counter++;
  show();
}

/**
 * @brief   update all pixels of the strip: 
 *          transmit pixel data in RAM to NeoPixels
 * @param   color set all leds to the provided color
 */
void SegmentedStrip::setStripe(uint32_t color) {
  fill(color, 0, numLEDs);
}

/**
 * @brief   reset all pixels of the strip: 
 *          transmit pixel data in RAM to NeoPixels
 *          set all leds OFF
 */
void SegmentedStrip::resetStripe() {
  setStripe(OFF);
}


// segments specific methods
/**
 * @brief   set all leds of the active_segments to the provided color
 *          all other leds stay the same! i.e. reset segments before 
 * @param   color set segments to the provided color
 * @param   active_segments update only the specified segments 
 *                          e.g. active segments = 0x0F == 0b0001111 -> last 4 Segments active
 */
void SegmentedStrip::setSegments(uint32_t color, uint32_t active_segments) {
  for(int i=0; i<n_segments; i++) {
    if(active_segments & (1 << i)) {
      fill(color, segments[i].first, segments[i].count);
    }
  }
}

/**
 * @brief   set all leds of the active_segments OFF 
 * @param   active_segments update only the specified segments 
 */
void SegmentedStrip::resetSegments(uint32_t active_segments) {
  setSegments(OFF, active_segments);
}

/**
 * @brief   set all leds of the all segments to the provided color 
 * @param   color set segments to the provided color
 * @note    this method is slow, use setStripe(uint32_t color) instead
 */
void SegmentedStrip::setAllSegments(uint32_t color) {
  setSegments(color, getAllSegments());
}

/**
 * @brief   set all leds of even segments to the provided color 
 *          i.e. active_segements = 0xAA = 0b10101010 
 * @param   color set segments to the provided color
 */
void SegmentedStrip::setEvenSegments(uint32_t color) {
  setSegments(color, getEvenSegments());
}

/**
 * @brief   set all leds of odd segments to the provided color 
 *          i.e. active_segements = 0x55 = 0b01010101 
 * @param   color set segments to the provided color
 */
void SegmentedStrip::setOddSegments(uint32_t color) {
  setSegments(color, getOddSegments());
}

/**
 * @brief   set all leds of the first n segments to the provided color 
 *          i.e. first 5 of 8 segments: active_segments = 0x1F = 0b00011111 
 * @param   color set segments to the provided color
 */
void SegmentedStrip::setFirstSegments(uint32_t color, uint8_t n) {
  setSegments(color, getFirstSegments(n));
}

/**
 * @brief   set all leds of the first n segments to the provided color 
 *          i.e. last 5 of 8 segments: active_segments = 0xF8 = 0b11111000 
 * @param   color set segments to the provided color
 */
void SegmentedStrip::setLastSegments(uint32_t color, uint8_t n) {
  setSegments(color, getLastSegments(n));
}

/**
 * @brief   set all leds of the active_segments segments alternating to the provided colors
 * @param   color1 set segments to color1 when (frame_counter%frames) < frame_color_switch
 * @param   color2 set segments to color2 when (frame_counter%frames) >= frame_color_switch
 * @param   active_segments update only the specified segments
 * @param   frames number of frames one blink cycle takes
 * @param   frame_color_switch number of frames color1 is active
 */
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

/** blinkSegments: active_segments=getAllSegments() */
void SegmentedStrip::blinkAllSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getAllSegments(), frames, frame_color_switch);
}

/** blinkSegments: active_segments=getEvenSegments() */
void SegmentedStrip::blinkEvenSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getEvenSegments(), frames, frame_color_switch);
}

/** blinkSegments: active_segments=getOddSegments() */
void SegmentedStrip::blinkOddSegments(uint32_t color1, uint32_t color2, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getOddSegments(), frames, frame_color_switch);
}

/** blinkSegments: active_segments=getFirstSegments(n) */
void SegmentedStrip::blinkFirstSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getFirstSegments(n), frames, frame_color_switch);
}

/** blinkSegments: active_segments=getLastSegments(n) */
void SegmentedStrip::blinkLastSegments(uint32_t color1, uint32_t color2, uint8_t n, uint16_t frames, uint16_t frame_color_switch) {
  blinkSegments(color1, color2, getLastSegments(n), frames, frame_color_switch);
}

/**
 * @brief   animate all leds of the active_segments
 * @param   color1 set all temp_active_segments to color1 for the animation
 * @param   color2 set all remaining active segments to color2
 * @param   active_segments update only the specified segments
 * @param   init_segments initial segments set to color1
 * @param   shift_segments number of segments the animation is shifted forward e.g. shift_segments=1: shift init_segments 1 segment forward per animation frame
 * @param   frames number of frames one animation cycle takes
 * @param   animation_frames number of frames a single animation frame is shown
 */
void SegmentedStrip::shiftSegments(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t animation_frames) {
  uint8_t step = (frame_counter%frames) / animation_frames; 
  uint32_t temp_active_segments = (shift_segments >= 0) ? active_segments & (init_segments << (shift_segments*step)) : active_segments & (init_segments >> (abs(shift_segments)*step));
  setSegments(color2, active_segments);  
  setSegments(color1, temp_active_segments);
}

/** shiftSegments: color1=color, color2=OFF */
void SegmentedStrip::shiftSegments(uint32_t color, uint32_t active_segments, uint32_t init_segments, int8_t shift_segments, uint16_t frames, uint16_t animation_frames) {
  shiftSegments(color, OFF, active_segments, init_segments, shift_segments, frames, animation_frames);
}


// segments pixel pattern specific methods
/**
 * @brief   set active_pixels of active_segments to the provided color
 *          all other leds stay the same! i.e. reset segments before
 * @param   color set pixels of the segments to the provided color
 * @param   active_segments update only the specified segments 
 * @param   active_pixel update only the specified pixel of each segment
 */
void SegmentedStrip::setPattern(uint32_t color, uint32_t active_segments, uint32_t active_pixel) {
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

/**
 * @brief   set active_pixels of active_segments in alternation to the provided colors
 * @param   color1 set active_pixels to color1 when (frame_counter%frames) < frame_color_switch
 * @param   color2 set active_pixels to color2 when (frame_counter%frames) >= frame_color_switch
 * @param   active_segments update only the specified segments
 * @param   active_pixel update only the specified pixel of each segment
 * @param   frames number of frames one blink cycle takes
 * @param   frame_color_switch number of frames color1 is active
 */
void SegmentedStrip::blinkPattern(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t active_pixel, uint16_t frames, uint16_t frame_color_switch) {
  if(frame_color_switch == 0){
    frame_color_switch = frames / 2;
  }
  
  // always set colors (improve: only set when changing?)
  if((frame_counter%frames) < frame_color_switch) {
    setPattern(color1, active_segments, active_pixel);
  }
  else {
    setPattern(color2, active_segments, active_pixel);
  }
}

/**
 * @brief   animate active_pixels of active_segments
 * @param   color1 set all temp_active_pixel of active_segments to color1 for the animation
 * @param   color2 set all remaining pixel of active_segments to color2
 * @param   active_segments update only the specified segments
 * @param   pixel_pattern initial pixel set to color1
 * @param   shift_pixel number of pixel the animation is shifted forward e.g. shift_pixel=1: shift pixel_pattern 1 pixel forward per animation frame
 * @param   frames number of frames one animation cycle takes
 * @param   animation_frames number of frames a single animation frame is shown
 */
void SegmentedStrip::shiftPattern(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t pixel_pattern, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames) {
  uint8_t step = (frame_counter%frames) / animation_frames; 
  uint32_t temp_active_pixel = (shift_pixel >= 0) ? pixel_pattern << (shift_pixel*step) : pixel_pattern >> (abs(shift_pixel)*step);
  setSegments(color2, active_segments);
  setPattern(color1, active_segments, temp_active_pixel);
}

/** shiftPattern: color1=color, color2=OFF */
void SegmentedStrip::shiftPattern(uint32_t color, uint32_t active_segments, uint32_t pixel_pattern, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames) {
  shiftPattern(color, OFF, active_segments, pixel_pattern, shift_pixel, frames, animation_frames);
}

/**
 * @brief   animate active_pixels of active_segments
 * @param   color1 set all temp_active_pixel of active_segments to color1 for the animation
 * @param   color2 set all remaining pixel of active_segments to color2
 * @param   active_segments update only the specified segments
 * @param   pixel_pattern initial pixel set to color1
 * @param   shift_pixel number of pixel the animation is shifted forward e.g. shift_pixel=1: shift pixel_pattern 1 pixel forward per animation frame
 * @param   init_shift_pixel 
 * @param   frames number of frames one animation cycle takes
 * @param   animation_frames number of frames a single animation frame is shown
 */
void SegmentedStrip::shiftPatternInit(uint32_t color1, uint32_t color2, uint32_t active_segments, uint32_t pixel_pattern, int8_t init_shift_pixel, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames) {
  // uint32_t temp_active_pixel = (shift_pixel >= 0) ? pixel_pattern << (shift_pixel*step) : pixel_pattern >> (abs(shift_pixel)*step);

  // 1. init pattern
  uint8_t step = (frame_counter%frames) / animation_frames; 
  int8_t shift_pattern = init_shift_pixel + shift_pixel*step;
  uint32_t temp_active_pixel = (shift_pattern >= 0) ? pixel_pattern << shift_pattern : pixel_pattern >> abs(shift_pattern);
  // uint32_t temp_active_pixel;
  // if (shift_pattern >= 0) {
  //   temp_active_pixel = pixel_pattern << shift_pattern;
  // } 
  // else {
  //   temp_active_pixel = pixel_pattern >> abs(shift_pattern);
  // } 

  // 3. update strip
  setSegments(color2, active_segments);
  setPattern(color1, active_segments, temp_active_pixel);
}

/** shiftPatternInit: color1=color, color2=OFF */
void SegmentedStrip::shiftPatternInit(uint32_t color, uint32_t active_segments, uint32_t pixel_pattern, int8_t init_shift_pixel, int8_t shift_pixel, uint16_t frames, uint16_t animation_frames) {
  shiftPatternInit(color, OFF, active_segments, pixel_pattern, init_shift_pixel, shift_pixel, frames, animation_frames);
}

// play with colors
/**
 * @brief   set all leds of the active_segments to different colors
 * @param   color_degree_start color_degree of the first led segments to the provided color
 * @param   color_degree_led_step color_degree added to each subsequent led
 * @param   active_segments update only the specified segments 
 */
void SegmentedStrip::setSegmentsRainbow(uint16_t color_degree_start, uint16_t color_degree_led_step, uint32_t active_segments) {
  for(int i=0; i<n_segments; i++) {
    if(active_segments & (1 << i)) {
      for(int led_i = 0; led_i < segments[i].count ; led_i++) {
        uint32_t color_degree_led_i = color(color_degree_start + led_i*color_degree_led_step);
        setPixelColor(segments[i].first + led_i, color_degree_led_i);
      }
    }
  }
}

/**
 * @brief   set all leds of each active_segments to different colors, led colors are shifted per frame for the animation
 * @param   color_degree_start color_degree of the first led in each active_segment
 * @param   color_degree_led_step color_degree added to each subsequent led
 * @param   active_segments update only the specified segments 
 * @param   color_degree_frame_step color_degree the first led gets increased every frame_step 
 * @param   animation_frames number of frames the led colors are held constant
 */
void SegmentedStrip::animateSegmentsRainbow(uint16_t color_degree_start, uint16_t color_degree_led_step, uint32_t active_segments, uint16_t color_degree_frame_step, uint16_t animation_frames) {
  uint32_t frame_step = frame_counter / animation_frames;
  uint16_t color_degree = color_degree_start + frame_step*color_degree_frame_step;
  setSegmentsRainbow(color_degree, color_degree_led_step, active_segments);
}

/**
 * @brief   set all leds of the strip to different colors, led colors are shifted per frame for the animation
 * @param   color_degree_start color_degree of all leds
 * @param   color_degree_frame_step color_degree all led gets increased every frame_step
 * @param   animation_frames number of frames the colors are held constant
 */
void SegmentedStrip::animateRainbowStripe(uint16_t color_degree_start, uint16_t color_degree_frame_step, uint16_t animation_frames) {
  uint32_t frame_step = frame_counter / animation_frames;
  uint32_t color_degree = color(color_degree_start + frame_step*color_degree_frame_step);
  fill(color_degree, 0, numLEDs);
}

/**
 * @brief   set all leds of the strip to different colors, led colors are shifted per frame for the animation
 * @param   color_degree_start color_degree of the first led
 * @param   color_degree_led_step color_degree added to each subsequent led
 * @param   color_degree_frame_step color_degree the first led gets increased every frame_step 
 * @param   animation_frames number of frames the led colors are held constant
 */
void SegmentedStrip::animateRainbowLEDs(uint16_t color_degree_start, uint16_t color_degree_led_step, uint16_t color_degree_frame_step, uint16_t animation_frames) {
  uint32_t frame_step = frame_counter / animation_frames;
  for(int led_i = 0; led_i < numLEDs; led_i++) {
    uint32_t color_degree_led_i = color(color_degree_start + frame_step*color_degree_frame_step + led_i*color_degree_led_step);
    setPixelColor(led_i, color_degree_led_i);
  }
}

/* colors */
/**
 * @brief   calculate HSV color for a given degree
 *          using the private saturation and brightness settings
 * @param   degree HSV color degree setting
 * @return  packed 32-bit RGB color
 *          optional: apply additional gamma correction function to the returned 32bit value!
 */
uint32_t SegmentedStrip::color(uint16_t degree) {
  // Adafruit_NeoPixel::gamma32(temp_color);
  return Adafruit_NeoPixel::ColorHSV(degree*HUE_DEGREE, saturation, brightness);
}

/** return 32-bit RGB color white using the private brightness settings */
uint32_t SegmentedStrip::WHITE() {
  // Adafruit_NeoPixel::Color(0xFF, 0xFF, 0xFF)
  return Adafruit_NeoPixel::ColorHSV(0, 0, brightness);
}

/** return 32-bit RGB color red using the private saturation and brightness settings */
uint32_t SegmentedStrip::RED() {
  return color(0);
}

/** return 32-bit RGB color yellow using the private saturation and brightness settings */
uint32_t SegmentedStrip::YELLOW() {
  return color(60);
}

/** return 32-bit RGB color green using the private saturation and brightness settings */
uint32_t SegmentedStrip::GREEN() {
  return color(120);
}

/** return 32-bit RGB color cyan using the private saturation and brightness settings */
uint32_t SegmentedStrip::CYAN() {
  return color(180);
}

/** return 32-bit RGB color blue using the private saturation and brightness settings */
uint32_t SegmentedStrip::BLUE() {
  return color(240);
}

/** return 32-bit RGB color magenta using the private saturation and brightness settings */
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
/**
 * @brief   update the segments of the led strip
 *          each segment is defined by:
 *          1. the first led number of the segment in the strip
 *          2. the total number of leds in the segment
 * @param   segment_starts[] array of led numbers: each entry defines a new segment
 */
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

/** extract the n_leds of the longest segment in the strip */
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
void shiftSegments(uint32_t color, uint32_t delayVal) {
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