#ifndef WIFIHOST_HPP
#define WIFIHOST_HPP

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>


class WifiHost
{
public:

    WifiHost(
        int configPortalTimeout,
        int connectTimeout
    );


    // ------------------------------------------------------------------------
    // Mit gespeichertem WLAN verbinden
    //
    // Rückgabe:
    // false = WLAN verbunden
    // true  = keine Verbindung
    // ------------------------------------------------------------------------

    bool connectToWifi();


    // ------------------------------------------------------------------------
    // WLAN trennen
    // ------------------------------------------------------------------------

    bool disconnectFromWifi();


    // ------------------------------------------------------------------------
    // Eigenen Konfigurations-Access-Point starten
    // ------------------------------------------------------------------------

    bool startAccessPoint();


    // ------------------------------------------------------------------------
    // Access Point stoppen
    // ------------------------------------------------------------------------

    bool stopAccessPoint();


    // ------------------------------------------------------------------------
    // Ist der AP aktiv?
    // ------------------------------------------------------------------------

    bool isAccessPointActive() const;


    // ------------------------------------------------------------------------
    // Aktuelle SSID des verbundenen WLAN
    // ------------------------------------------------------------------------

    String getSSID() const;


    // ------------------------------------------------------------------------
    // IP-Adresse des verbundenen WLAN
    // ------------------------------------------------------------------------

    IPAddress getLocalIP() const;


    // ------------------------------------------------------------------------
    // IP-Adresse des Konfigurations-AP
    // ------------------------------------------------------------------------

    IPAddress getAccessPointIP() const;


private:

    WiFiManager manager;

    int this_configPortalTimeout;
    int this_connectTimeout;

    bool accessPointActive;
};

#endif