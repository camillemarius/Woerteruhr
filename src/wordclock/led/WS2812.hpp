#ifndef WS2812_h
#define WS2812_h

#include "Arduino.h"
#include <Adafruit_NeoPixel.h>

class WS2812
{ 
    public:
        WS2812(int led_cnt);
        
        void setPixelColorArrayMap(const int* data);
        void deletePixelColorArrayFromTo(int from, int to);
        void show(void);
        void setBrightnessFor(int ledIndex, uint8_t brightness);
        void setBrightness(int _brighntess);
        void copyPrevPixelColorArrayMap();
        void deleteActualPixelColorArrayMap();
        void clearPixelColorArray();
        
        void dimOn(int finalBrightness);
        void dimNewlyTurnedOn(int finalBrightness);
        void dimOff(int actuallBrighntess);
        void allOff(void);
        void rainbowWheel(const int* data, int wait);
        uint32_t colourWheel(byte WheelPos);
   

    private:
        WS2812(Adafruit_NeoPixel *m_strip)
            : strip(m_strip) 
            {

            }
        Adafruit_NeoPixel *strip;
        
        bool *prevPixelColors;
        bool *actualPixelColors;

        #define LED_PIN D4 //GPIO5
        int led_cnt;
        int brightness;

        #define white 0xFFFFFFFF
        #define off   0x00000000
};

#endif
