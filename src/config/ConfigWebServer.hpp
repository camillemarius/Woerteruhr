#ifndef CONFIG_WEBSERVER_HPP
#define CONFIG_WEBSERVER_HPP

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>


#define EEPROM_SIZE 128
#define CONFIG_MAGIC 0x12345678

#define WEB_TIMEOUT_MS 120000UL


struct ConfigData
{
    uint32_t magic;

    uint8_t sleepHour;
    uint8_t sleepMinute;

    uint8_t wakeupHour;
    uint8_t wakeupMinute;

    bool header1;
    bool header2;
};


class ConfigWebServer
{
public:

    ConfigWebServer();


    // -------------------------------------------------------------------------
    // Konfiguration laden
    // -------------------------------------------------------------------------

    void begin();


    // -------------------------------------------------------------------------
    // Webserver starten
    // -------------------------------------------------------------------------

    void start();


    // -------------------------------------------------------------------------
    // Webserver bearbeiten
    // -------------------------------------------------------------------------

    void handle();


    // -------------------------------------------------------------------------
    // Webserver stoppen
    // -------------------------------------------------------------------------

    void stop();


    bool isActive();


    // -------------------------------------------------------------------------
    // Konfiguration wurde geändert
    // -------------------------------------------------------------------------

    bool hasConfigChanged() const;

    void clearConfigChanged();


    // -------------------------------------------------------------------------
    // Sleep
    // -------------------------------------------------------------------------

    uint8_t getSleepHour() const;

    uint8_t getSleepMinute() const;


    // -------------------------------------------------------------------------
    // Wakeup
    // -------------------------------------------------------------------------

    uint8_t getWakeupHour() const;

    uint8_t getWakeupMinute() const;


    // -------------------------------------------------------------------------
    // Header
    // -------------------------------------------------------------------------

    bool getHeader1() const;

    bool getHeader2() const;


private:

    void handleRoot();

    void handleSave();

    void load();

    void save();

    String htmlPage(bool saved);


    ESP8266WebServer server;


    unsigned long startTime;


    bool active;

    bool configChanged;


    ConfigData config;
};


#endif
