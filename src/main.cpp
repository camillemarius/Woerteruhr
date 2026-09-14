/*=================================================================================
--Includes
=================================================================================*/
#include <Arduino.h>
#include <WordClock/WordClock.hpp>
#include <time/SwissTime.hpp>

/*=================================================================================
--Variablen
=================================================================================*/
const int sleeptime = 24; 
const int wakeuptime = 6;

const double wakeupdelay_ms = 60*1000; // ms

SwissTime swissTime;
WordClock wordClock;



/*=================================================================================
--Setup
=================================================================================*/
void setup()
{
  // Serial Init
  Serial.begin(9600);

  // Init WS2812
  wordClock.showAdvertisingAnimation();
}

/*=================================================================================
--Main
=================================================================================*/
void loop()
{
  /*---------------------------------------------------------------------------
  -- Update Time
  ---------------------------------------------------------------------------*/
  bool error = swissTime.getTime();
  if (error == false)
  {
    /*---------------------------------------------------------------------------
    -- Turn qlocktwo off by night
    ---------------------------------------------------------------------------*/
    if ((swissTime.h >= sleeptime) || (swissTime.h < wakeuptime))
    {
      wordClock.allOff();
      if(swissTime.h >= wakeuptime-1) {
        // check time more often one showHour before wakeup
        delay(wakeupdelay_ms);
      }
      else {
        //delay(wakeupdelay_ms*60);
        delay(wakeupdelay_ms);
      }
    }
    else
    {
      wordClock.showTime(swissTime.s, swissTime.m, swissTime.h);
      // If await next Miniute Boundary is bigger zero, show Time again.
      //if(swissTime.awaitNextMinuteBoundary()) {
      //  ws2812.showTime(swissTime.s, swissTime.m, swissTime.h);
      //}
      (void) swissTime.awaitNextMinuteBoundary();
      //delay(wakeupdelay_ms);
    } 
  } else {
    wordClock.showNoInternet();
  }
}