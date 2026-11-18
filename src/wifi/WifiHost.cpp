#include "WifiHost.hpp"


// ============================================================================
// KONSTRUKTOR
// ============================================================================

WifiHost::WifiHost(
    int configPortalTimeout,
    int connectTimeout
)
    : lastReconnectAttempt(0),
      this_configPortalTimeout(configPortalTimeout),
      this_connectTimeout(connectTimeout),
      accessPointActive(false)
{
}


// ============================================================================
// VERBINDUNGSAUFBAU STARTEN
//
// Der ESP8266-Core speichert WLAN-Daten seit Version 3 standardmässig nicht
// mehr im Flash (WiFi.persistent ist false). WiFi.begin() ohne Daten kennt
// deshalb nach einem Neustart die auf der Konfigurationsseite gesetzten
// Daten nicht. Darum werden hier die Daten aus dem EEPROM übergeben.
// ============================================================================

void WifiHost::beginConnection(
    const String& ssid,
    const String& password
)
{
    if (ssid.length() > 0)
    {
        WiFi.begin(
            ssid.c_str(),
            password.c_str()
        );
    }
    else
    {
        // Keine Daten im EEPROM: im SDK gespeicherte Daten verwenden
        WiFi.begin();
    }
}


// ============================================================================
// WLAN VERBINDEN
// ============================================================================

bool WifiHost::connectToWifi(
    const String& ssid,
    const String& password
)
{
    Serial.println();
    Serial.println(
        "[WIFI] Versuche gespeichertes WLAN..."
    );


    manager.setConfigPortalTimeout(
        this_configPortalTimeout
    );


    manager.setConnectTimeout(
        this_connectTimeout
    );


    /*
     * autoConnect():
     *
     * - verwendet gespeicherte WLAN-Daten
     * - wenn vorhanden, wird automatisch verbunden
     * - falls keine Verbindung möglich ist,
     *   kann WiFiManager sein eigenes Portal öffnen
     *
     * Da wir den Konfigurations-AP separat verwenden wollen,
     * wird hier zunächst versucht, direkt zu verbinden.
     */

    WiFi.mode(WIFI_STA);


    // Nach einem Verbindungsabbruch selbst neu verbinden
    WiFi.setAutoReconnect(true);


    beginConnection(
        ssid,
        password
    );


    unsigned long start = millis();

    lastReconnectAttempt = start;


    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - start <
        ((unsigned long)this_connectTimeout * 1000UL)
    )
    {
        delay(250);

        Serial.print(".");
    }


    Serial.println();


    if (WiFi.status() == WL_CONNECTED)
    {
        Serial.println(
            "[WIFI] WLAN verbunden"
        );


        Serial.print(
            "[WIFI] SSID: "
        );

        Serial.println(
            WiFi.SSID()
        );


        Serial.print(
            "[WIFI] IP: "
        );

        Serial.println(
            WiFi.localIP()
        );


        return false;
    }


    Serial.println(
        "[WIFI] Keine Verbindung zum gespeicherten WLAN"
    );


    return true;
}


// ============================================================================
// VERBINDUNG HALTEN
//
// Nach einem WLAN-Ausfall (z.B. Router-Neustart oder Stromausfall, bei dem
// der ESP schneller startet als der Router) wird regelmässig ein neuer
// Verbindungsversuch gestartet, bis das WLAN wieder da ist.
// ============================================================================

void WifiHost::maintainConnection(
    const String& ssid,
    const String& password
)
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return;
    }


    unsigned long now = millis();


    if (now - lastReconnectAttempt < RECONNECT_INTERVAL_MS)
    {
        return;
    }


    lastReconnectAttempt = now;


    Serial.println(
        "[WIFI] Keine Verbindung -> neuer Verbindungsversuch"
    );


    beginConnection(
        ssid,
        password
    );
}


// ============================================================================
// WLAN TRENNEN
// ============================================================================

bool WifiHost::disconnectFromWifi()
{
    return WiFi.disconnect();
}


// ============================================================================
// KONFIGURATIONS-ACCESS-POINT
// ============================================================================

bool WifiHost::startAccessPoint()
{
    if (accessPointActive)
    {
        return true;
    }


    Serial.println();
    Serial.println(
        "[WIFI] Starte Konfigurations-Access-Point..."
    );


    /*
     * STA-Verbindung zuerst trennen.
     *
     * Danach arbeitet der ESP ausschließlich
     * als eigener Access Point.
     */

    WiFi.disconnect();

    delay(100);


    WiFi.mode(WIFI_AP);


    bool result =
        WiFi.softAP(
            "WordClock"
        );


    if (!result)
    {
        Serial.println(
            "[WIFI] Fehler beim Starten des Access-Points"
        );

        return false;
    }


    accessPointActive = true;


    Serial.println(
        "[WIFI] Access-Point gestartet"
    );


    Serial.print(
        "[WIFI] SSID: "
    );

    Serial.println(
        "WordClock"
    );


    Serial.print(
        "[WIFI] IP: "
    );

    Serial.println(
        WiFi.softAPIP()
    );


    return true;
}


// ============================================================================
// ACCESS-POINT STOPPEN
// ============================================================================

bool WifiHost::stopAccessPoint()
{
    if (!accessPointActive)
    {
        return true;
    }


    Serial.println(
        "[WIFI] Stoppe Konfigurations-Access-Point"
    );


    WiFi.softAPdisconnect(
        true
    );


    accessPointActive = false;


    return true;
}


// ============================================================================
// AP AKTIV?
// ============================================================================

bool WifiHost::isAccessPointActive() const
{
    return accessPointActive;
}


// ============================================================================
// SSID
// ============================================================================

String WifiHost::getSSID() const
{
    if (WiFi.status() == WL_CONNECTED)
    {
        return WiFi.SSID();
    }


    return "";
}


// ============================================================================
// LOCAL IP
// ============================================================================

IPAddress WifiHost::getLocalIP() const
{
    return WiFi.localIP();
}


// ============================================================================
// ACCESS-POINT IP
// ============================================================================

IPAddress WifiHost::getAccessPointIP() const
{
    return WiFi.softAPIP();
}