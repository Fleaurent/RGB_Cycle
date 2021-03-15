# RGB Bicycle  
https://learn.adafruit.com/adafruit-neopixel-uberguide/  
https://www.instructables.com/NeoPixel-Party-Bike-Music-Reactive-Animations-With/  

[# 1. Hardware Setup](#1-hardware-setup)  
[# 2. Measurements](#2-measurements)  
[# 3. Code](#3-code)  

---
# 1. Hardware Setup
Board: Arduino Nano  
install correct driver for the CH340 serial communication chip  
&rarr; https://learn.sparkfun.com/tutorials/how-to-install-ch340-drivers/all  

Processor: ATmega328P Old Bootloader  
&rarr; Port should be available  

same as Arduino Uno  
&rarr; 5V pins

RGB Stripes:  
- **ws2812b** (3 Wire, PWM)  
- APA102 (4 Wire, SPI)  

best practices:  
- add a large capacitor (>1000uF) between +/- of the power source  
- add a 300-500Ohm Restistor to the data line  


---
# 2. Measurements  
## 2.1 WS2812B   
https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use  

**Power Demand:**  
150 LEDS: every LED ~40ma  
+ each NeoPixel requires about 3 bytes of RAM: 450Bytes  

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

## 2.2 AP102  
60 LEDs: 100ma!!!


--- 
# 3. Code
Libraries:  
- APA102
- NeoPixelBus by Makuna  
- **Adafruit NeoPixel**  


## 3.1 Adafruit NeoPixel Basics
https://adafruit.github.io/Adafruit_NeoPixel/html/index.html  

1. basic setup
```cpp
#include <Adafruit_NeoPixel.h>

#define LED_PIN   2
#define LED_COUNT 150

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  Serial.println("Running...");
}
```

2. set pixel color  
```cpp
// n = pixel number along the strip starting with 0
// pixel color rgb brightness levels: 0 is dimmest (off) and 255 maximum brightness  
strip.setPixelColor(n, red, green, blue);

// alternative: sometimes easier and faster: color 32-bit type
uint32_t magenta = strip.Color(255, 0, 255);
strip.setPixelColor(n, color);  
```

3. update pixel  
```cpp
// push color data to the strip
// -> update the whole strip at once
strip.show();

// set multiple pixel to the same 32-bit RGB color value:  
strip.fill(color, first, count);

// turn LEDs “off”
strip.clear();
```

other methods: query existing strips objects  
```cpp
// query the color of a previously-set pixel
uint32_t color = strip.getPixelColor(11);

// number of pixels in a previously-declared strip
uint16_t n = strip.numPixels();
```

## 3.2 FastLED Basics
http://fastled.io/  


## 3.3 Color Brightness  

fixed brightness
```cpp
// colors
#define BRIGHTNESS 150
#define OFF strip.Color(0, 0, 0)
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