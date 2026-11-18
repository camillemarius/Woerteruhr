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
    // ssid/password = WLAN-Daten aus der Konfigurationsseite (EEPROM).
    // Ist die SSID leer, werden die im ESP-SDK gespeicherten Daten verwendet.
    //
    // Rückgabe:
    // false = WLAN verbunden
    // true  = keine Verbindung
    // ------------------------------------------------------------------------

    bool connectToWifi(
        const String& ssid,
        const String& password
    );


    // ------------------------------------------------------------------------
    // Verbindung im Normalbetrieb halten
    //
    // Blockiert nicht. Ist das WLAN weg, wird höchstens alle
    // RECONNECT_INTERVAL_MS ein neuer Verbindungsversuch gestartet.
    // ------------------------------------------------------------------------

    void maintainConnection(
        const String& ssid,
        const String& password
    );


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

    static const unsigned long RECONNECT_INTERVAL_MS = 60000UL;


    // ------------------------------------------------------------------------
    // Verbindungsaufbau starten (ohne Warten)
    // ------------------------------------------------------------------------

    void beginConnection(
        const String& ssid,
        const String& password
    );


    WiFiManager manager;

    unsigned long lastReconnectAttempt;

    int this_configPortalTimeout;
    int this_connectTimeout;

    bool accessPointActive;
};

#endif