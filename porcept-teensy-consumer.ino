#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <AVR/pgmspace.h>
#include <Arduino.h>

#define STRAND 2

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 2

#define LED_WIDTH  90
#define LED_HEIGHT 3
#define NUM_LEDS   (LED_WIDTH * LED_HEIGHT)

DMAMEM int displayMemory[NUM_LEDS * 4];
char drawingMemory[NUM_LEDS * 4];

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRBW + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() {
  pinMode(12, INPUT_PULLUP); // Frame Sync
  Serial.setTimeout(1000);
  pinMode(13, OUTPUT);
  strip.begin();
  strip.show();

}

void loop() {
  int startChar = Serial.read();

  if (startChar == '*') {
    // receive a "master" frame
    int count = Serial.readBytes((char *)drawingMemory, sizeof(drawingMemory));
    if (count == sizeof(drawingMemory)) {
      Serial.print('*');
      
      for (int i = 0; i < NUM_LEDS; i++) {
         strip.setPixelColor(i, drawingMemory[(i * 4)], drawingMemory[(i * 4) + 1], drawingMemory[(i * 4) + 2], drawingMemory[(i * 4) + 3]);
      }
      
      strip.show();
      
    } else { Serial.print('#'); }
  
  } else if (startChar == '?') {
    Serial.println("You've reached the dodecatron, please leave a message ;-)");

  } else if (startChar >= 0) {
    // discard unknown characters
  }
}
