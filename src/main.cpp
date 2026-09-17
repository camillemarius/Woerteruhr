#include <Arduino.h>

#include <time/SwissTime.hpp>
#include <WordClock/WordClock.hpp>
#include <config/ConfigWebServer.hpp>
#include <wifi/WifiHost.hpp>


// ============================================================================
// OBJEKTE
// ============================================================================

SwissTime swissTime;
WordClock wordClock;
ConfigWebServer configWebServer;

WifiHost wifiHost(
    180,    // Config-Portal Timeout: 180 Sekunden
    30      // WLAN-Verbindungs-Timeout: 30 Sekunden
);


// ============================================================================
// TIMING
// ============================================================================

unsigned long lastTimeUpdate = 0;

const unsigned long TIME_UPDATE_INTERVAL = 60000UL;   // 1 Minute

int lastDisplayedMinute = -1;
int lastDisplayedHour = -1;


// ============================================================================
// DISPLAY AKTUALISIEREN
// ============================================================================

void updateDisplay()
{
    wordClock.showTime(
        swissTime.s,
        swissTime.m,
        swissTime.h,
        configWebServer.getHeader1(),
        configWebServer.getHeader2()
    );

    lastDisplayedMinute = swissTime.m;
    lastDisplayedHour = swissTime.h;

    Serial.printf(
        "[MAIN] Anzeige: %02d:%02d:%02d\n",
        swissTime.h,
        swissTime.m,
        swissTime.s
    );
}


// ============================================================================
// AUF NTP-ZEIT WARTEN
// ============================================================================

bool waitForValidTime()
{
    Serial.println("[MAIN] Warte auf NTP-Zeit...");

    const int maxAttempts = 20;

    for (int i = 0; i < maxAttempts; i++)
    {
        bool timeError = swissTime.getTime();

        if (!timeError)
        {
            Serial.printf(
                "[MAIN] Gueltige Zeit erhalten: %02d:%02d:%02d\n",
                swissTime.h,
                swissTime.m,
                swissTime.s
            );

            return true;
        }

        Serial.printf(
            "[MAIN] NTP noch nicht verfuegbar (%d/%d)\n",
            i + 1,
            maxAttempts
        );

        delay(500);
    }

    Serial.println(
        "[MAIN] NTP-Zeit konnte nicht ermittelt werden"
    );

    return false;
}


// ============================================================================
// SETUP
// ============================================================================

void setup()
{
    Serial.begin(74880);
    delay(500);
    // -------------------------------------------------------------------------
    // Led auschalten
    // -------------------------------------------------------------------------
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // -------------------------------------------------------------------------
    // Konfiguration aus EEPROM laden
    // -------------------------------------------------------------------------
    configWebServer.begin();


    // -------------------------------------------------------------------------
    // WLAN verbinden
    // -------------------------------------------------------------------------
    bool wifiError = wifiHost.connectToWifi();

    if (wifiError) {
        Serial.println("[MAIN] WLAN verbunden");
        Serial.print("[MAIN] IP: ");
        Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("[MAIN] Keine WLAN-Verbindung");
    }


    // -------------------------------------------------------------------------
    // NTP-Zeit holen
    // -------------------------------------------------------------------------
    bool timeError = true;

    if (!wifiError) {
        timeError = !waitForValidTime();
    }
    else {
        Serial.println("[MAIN] Keine WLAN-Verbindung -> keine NTP-Zeit");
    }


    // -------------------------------------------------------------------------
    // Zeit anzeigen
    // -------------------------------------------------------------------------
    if (!timeError) {
        updateDisplay();
    }
    else {
        Serial.println("[MAIN] Keine gueltige Zeit -> Anzeige wird nicht aktualisiert");
    }


    // -------------------------------------------------------------------------
    // Webserver fuer Auswahl starten
    // -------------------------------------------------------------------------
    if (!wifiError) {
        configWebServer.start();
        Serial.println("[MAIN] Config Webserver läuft");
    }
    else {
        Serial.println("[MAIN] Auswahl nicht gestartet, da WLAN nicht verbunden ist");
    }

    // -------------------------------------------------------------------------
    // Timer starten
    // -------------------------------------------------------------------------
    lastTimeUpdate = millis();
}


// ============================================================================
// LOOP
// ============================================================================

void loop()
{
    // -------------------------------------------------------------------------
    // Webserver während der Auswahl bedienen
    // -------------------------------------------------------------------------
    if (configWebServer.isActive()) {
        configWebServer.handle();
    }

    // -------------------------------------------------------------------------
    // Konfiguration geändert?
    // -------------------------------------------------------------------------
    if (configWebServer.hasConfigChanged()) {
        Serial.println("[MAIN] Konfiguration wurde geaendert");
        updateDisplay();
        configWebServer.clearConfigChanged();
    }

    // -------------------------------------------------------------------------
    // Wenn Auswahl aktiv ist, nicht auf die nächste Minute blockieren
    // -------------------------------------------------------------------------
    if (configWebServer.isActive()){
        yield();
        return;
    }

    // -------------------------------------------------------------------------
    // Normalbetrieb:
    // Zeit holen
    // -------------------------------------------------------------------------
    bool timeError = swissTime.getTime();
    if (timeError) {
        Serial.println("[MAIN] Keine gueltige Zeit");
        wordClock.showNoInternet();
        delay(5000);
        return;
    }

    // -------------------------------------------------------------------------
    // Nachts ausschalten
    // -------------------------------------------------------------------------
    if ((swissTime.h >= configWebServer.getSleepHour()) || (swissTime.h < configWebServer.getWakeupHour())) {
        wordClock.allOff();
        // Im Schlafbetrieb nur einmal pro Minute prüfen.
        delay(60000);
        return;
    }

    // -------------------------------------------------------------------------
    // Uhrzeit anzeigen
    // -------------------------------------------------------------------------
    updateDisplay();


    // -------------------------------------------------------------------------
    // Auf nächste Minuten-Grenze warten
    // -------------------------------------------------------------------------
    swissTime.awaitNextMinuteBoundary();
    yield();
}