#include "WS2812.hpp"

/*============================================================================
== PUBLIC
============================================================================*/
WS2812::WS2812(int ledcnt)
{
  led_cnt = ledcnt;
  strip = new Adafruit_NeoPixel(ledcnt, LED_PIN, NEO_GRB + NEO_KHZ800);
  prevPixelColors = new bool[ledcnt];
  actualPixelColors = new bool[ledcnt];

  strip->begin();
}

/*-----------------------------------------
-- ON/OFF/DIM
-----------------------------------------*/
void WS2812::dimNewlyTurnedOn(int finalBrightness) {
  // Set brightness for all old Leds
  //strip->setBrightness(finalBrightness);

  for(int brightness = 5; brightness <= finalBrightness; brightness++) {
    for(int i = 0; i < led_cnt; i++) {
      if (prevPixelColors[i] != actualPixelColors[i]) {
        // Set brightness for all new Leds
          if(actualPixelColors[i]==true) {
            setBrightnessFor(i, brightness);
          } else if(actualPixelColors[i]==false) {
            setBrightnessFor(i, finalBrightness-brightness);
          }
      }
    }
    strip->show();
    delayMicroseconds(4000/finalBrightness); // Dimmvorgang gesamthaft: 4ms
  }
}

void WS2812::setBrightnessFor(int ledIndex, uint8_t brightness) {
    // Set the adjusted color for the LED
    strip->setPixelColor(ledIndex, strip->Color(brightness, brightness, brightness));
}

void WS2812::setBrightness(int _brighntess) {
  brightness = _brighntess;
}

void WS2812::dimOff(int actuallBrighntess) {
  for(int brightness = actuallBrighntess; brightness <= 3; brightness--) {
    strip->setBrightness(brightness);
    strip->show();
    delay(4000/actuallBrighntess);
  }
}

void WS2812::allOff(void) {
  strip->clear();
  clearPixelColorArray();
  strip->show();
}

/*-----------------------------------------
-- COLOUR EFFECTS
-----------------------------------------*/
void WS2812::rainbowWheel(const int* data, int wait) {
  uint16_t j;

  for(j=0; j<256; j++) {
    //for(i=0; i<strip.numPixels(); i++) 
    for (int i = 0; data[i] != -1; i++){
      strip->setPixelColor(data[i], colourWheel((i*15+j) & 255));
    }
    strip->show();
    delay(wait);
  }
}

uint32_t WS2812::colourWheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip->Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip->Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip->Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

void WS2812::setPixelColorArrayMap(const int* data) {
  for (int a = 0; data[a] != -1; a++)
  {
    strip->setPixelColor(data[a], strip->Color(brightness,brightness,brightness)); //  Set pixel's color (in RAM)
    actualPixelColors[data[a]] = true; //  Set pixel's color (in RAM)
  }
}

void WS2812::deletePixelColorArrayFromTo(int from, int to) {
    strip->fill(off, from, to);
}

void WS2812::clearPixelColorArray() {
    strip->fill(off, 0, led_cnt-1);
}

void WS2812::show(void) {
  strip->setBrightness(brightness);
  strip->show();
}

void WS2812::copyPrevPixelColorArrayMap(void) {
  for (int b = 0; b < led_cnt; b++)
  {
    prevPixelColors[b] = actualPixelColors[b];
  }
}

void WS2812::deleteActualPixelColorArrayMap(void) {
  for (int c = 0; c < led_cnt; c++)
  {
    actualPixelColors[c] = false;//  Set pixel's color (in RAM)
  }
}


