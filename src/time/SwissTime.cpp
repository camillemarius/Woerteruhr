#include "SwissTime.hpp"


// ============================================================================
// KONFIGURATION
// ============================================================================

#ifndef MY_TZ
#define MY_TZ "CET-1CEST,M3.5.0/2,M10.5.0/3"
#endif

#ifndef MY_NTP_SERVER
#define MY_NTP_SERVER "pool.ntp.org"
#endif


// ============================================================================
// KONSTRUKTOR
// ============================================================================

SwissTime::SwissTime()
    : s(0),
      m(0),
      h(0),
      now(0)
{
    configTime(
        MY_TZ,
        MY_NTP_SERVER
    );
}


// ============================================================================
// GET TIME
// ============================================================================

bool SwissTime::getTime()
{
    time(&now);


    // -------------------------------------------------------------------------
    // Prüfen, ob NTP bereits eine gültige Zeit geliefert hat
    // -------------------------------------------------------------------------

    if (now < 100000)
    {
        Serial.println(
            "[SwissTime] NTP time not valid"
        );

        return true;
    }


    // -------------------------------------------------------------------------
    // Unix-Zeit -> lokale Zeit
    // -------------------------------------------------------------------------

    localtime_r(
        &now,
        &tm_data
    );


    s = tm_data.tm_sec;
    m = tm_data.tm_min;
    h = tm_data.tm_hour;


    Serial.printf(
        "[SwissTime] Zeit: %02d:%02d:%02d\n",
        h,
        m,
        s
    );


    return false;
}

void SwissTime::awaitNextMinuteBoundary()
{
    // Sekunden bis zur nächsten vollen Minute
    int secondsToWait = 60 - s;

    // Falls wir bereits exakt auf einer vollen Minute sind,
    // eine komplette Minute warten.
    if (secondsToWait <= 0)
    {
        secondsToWait = 60;
    }

    Serial.printf(
        "[SwissTime] Warte %d Sekunden bis zur naechsten Minute\n",
        secondsToWait
    );

    delay((unsigned long)secondsToWait * 1000UL);
}