#ifndef SWISS_TIME_HPP
#define SWISS_TIME_HPP

#include <Arduino.h>
#include <time.h>


class SwissTime
{
public:

    SwissTime();
    

    // Aktuelle System-/NTP-Zeit lesen
    //
    // false = erfolgreich
    // true  = Fehler
    bool getTime();
    void awaitNextMinuteBoundary();

    int s;
    int m;
    int h;


private:

    time_t now;
    struct tm tm_data;
};

#endif