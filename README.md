# Arduino Nano

# 1. Connection
install correct driver for the CH340 serial communication chip  
&rarr; https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all  

Board: Arduino Nano  
Processor: ATmega328P Old Bootloader  
&rarr; Port should be available  

same as Arduino Uno  
&rarr; 5V pins

Library:  
- Adafruit NeoPixel  
- APA102
- NeoPixelBus by Makuna  

RGB Stripes:  
- ws2812b (3 Wire, PWM)  
- APA102 (4 Wire, SPI)  


# 2. Measurements  
## ws2812b  
https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use  

**Power Demand:**  
150 LEDS: every LED ~40ma  

| COLOR/BRIGHTNESS | 10   | 50   | 100  | 150 | 200 | 250 |  
|------------------|------|------|------|-----|-----|-----|  
| RED              | 0.16 | 0.44 | 0.77 | 1.1 | 1.4 | 1.8 |  
| GREEN            | 0.16 | 0.44 | 0.77 | 1.1 | 1.4 | 1.8 |  
| BLUE             | 0.16 | 0.44 | 0.77 | 1.1 | 1.4 | 1.8 |  
| YELLOW           | 0.23 | 0.77 | 1.44 | 2.0 | 2.4 | 2.8 |  
| MAGENTA          | 0.23 | 0.77 | 1.44 | 2.0 | 2.4 | 2.7 |  
| CYAN             | 0.23 | 0.77 | 1.44 | 2.0 | 2.4 | 2.6 |  
| WHITE            | 0.3  | 1.1  | 2.0  | 2.5 | 2.9 | 3.1 |  

**Timings Salae:**  
transmit data takes ~30us per LED  
&rarr; updating 150 LEDs takes ~0.8ms set and ~4.5ms transmit = ~5.3ms per show() command   
&rarr; ~190 updates per second!!!  

optimize code: call show() only when all LED updated!  

## AP102  
60 LEDs: 100ma!!!
  
# 3. Code
## 3.1 WS2812B
fixed brightness
```cpp
// colors
#define BRIGHTNESS 150
#define RED strip.Color(BRIGHTNESS, 0, 0)
#define GREEN strip.Color(0, BRIGHTNESS, 0)
#define BLUE strip.Color(0, 0, BRIGHTNESS)
#define YELLOW strip.Color(BRIGHTNESS, BRIGHTNESS, 0)
#define MAGENTA strip.Color(BRIGHTNESS, 0, BRIGHTNESS)
#define CYAN strip.Color(0, BRIGHTNESS, BRIGHTNESS)
#define WHITE strip.Color(BRIGHTNESS, BRIGHTNESS, BRIGHTNESS)
```

macro brightness  
```cpp
// macro brightness
#define RED(brightness) strip.Color(brightness, 0, 0)
#define GREEN(brightness) strip.Color(0, brightness, 0)
#define BLUE(brightness) strip.Color(0, 0, brightness)
#define YELLOW(brightness) strip.Color(brightness, brightness, 0)
#define MAGENTA(brightness) strip.Color(brightness, 0, brightness)
#define CYAN(brightness) strip.Color(0, brightness, brightness)
#define WHITE(brightness) strip.Color(brightness, brightness, brightness)
```

todo: set brightness dynamically  
```cpp
typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} color;

```