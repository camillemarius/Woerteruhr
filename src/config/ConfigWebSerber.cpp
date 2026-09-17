#ifndef CONFIG_WEBSERVER_HPP
#define CONFIG_WEBSERVER_HPP

#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266WiFi.h>


#define EEPROM_SIZE 256

#define CONFIG_MAGIC 0x12345678

#define CONFIG_VERSION 2

#define WEB_TIMEOUT_MS 120000UL


struct ConfigData
{
    uint32_t magic;

    uint16_t version;


    uint8_t sleepHour;
    uint8_t sleepMinute;

    uint8_t wakeupHour;
    uint8_t wakeupMinute;

    bool header1;
    bool header2;


    // -------------------------------------------------------------------------
    // WLAN
    // -------------------------------------------------------------------------

    char wifiSSID[64];

    char wifiPassword[64];
};


class ConfigWebServer
{
public:

    ConfigWebServer();


    void begin();

    void start();

    void handle();

    void stop();

    bool isActive();


    bool hasConfigChanged() const;

    void clearConfigChanged();


    uint8_t getSleepHour() const;

    uint8_t getSleepMinute() const;


    uint8_t getWakeupHour() const;

    uint8_t getWakeupMinute() const;


    bool getHeader1() const;

    bool getHeader2() const;


    String getWifiSSID() const;

    String getWifiPassword() const;


    void setWifiCredentials(
        const String& ssid,
        const String& password
    );


private:

    void handleRoot();

    void handleSave();

    void handleWifi();

    void handleScan();

    void handleClose();


    void load();

    void save();


    String htmlPage(
        bool saved
    );


    String wifiList();


    ESP8266WebServer server;


    unsigned long startTime;


    bool active;

    bool configChanged;


    ConfigData config;
};


#endif