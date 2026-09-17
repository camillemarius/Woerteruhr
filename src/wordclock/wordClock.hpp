#ifndef WordClock_HPP
#define WordClock_HPP

#include "Arduino.h"
#include "wordclock/led/WS2812.hpp"
#include <wordclock/frontplates/1.GEN/englisch_annasophia.h>
//#include "wordclock/frontplates/1.GEN/berndeutsch_jaya.h"
#include "brightnessControll/AmbientLightController.hpp"

class WordClock
{
    public:
        WordClock();
        void showTime(int s, int m, int h, bool header1, bool header2);
        void showAdvertisingAnimation(void);
        void showNoInternet(void);
        void allOff(void);

    private:  
        void setMin(int m);
        void setHour(int h);
    
    private:
        WS2812 *ws2812eco;

        AmbientLightController *lightMap;
        int brightness;

        

        
        
};

#endif