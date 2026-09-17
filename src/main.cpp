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
WifiHost wifiHost(180, 30);

// ============================================================================
// TIMING
// ============================================================================

unsigned long lastTimeUpdate = 0;
const unsigned long TIME_UPDATE_INTERVAL = 60000UL;

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

    Serial.println("[MAIN] NTP-Zeit konnte nicht ermittelt werden");
    return false;
}

// ============================================================================
// CONFIG WEBSERVER BEARBEITEN
// ============================================================================

void handleConfig()
{
    if (!configWebServer.isActive()) {
        return;
    }

    bool wasActive = configWebServer.isActive();

    configWebServer.handle();

    // ---------------------------------------------------------
    // Konfiguration geändert
    // ---------------------------------------------------------
    if (configWebServer.hasConfigChanged()) {
        Serial.println("[MAIN] Konfiguration wurde geaendert");

        if (swissTime.h >= 0 && swissTime.m >= 0) {
            updateDisplay();
        }

        configWebServer.clearConfigChanged();
    }

    // ---------------------------------------------------------
    // Webserver wurde durch Timeout oder /close beendet
    // ---------------------------------------------------------
    if (wasActive && !configWebServer.isActive()) {
        Serial.println("[MAIN] Config-Webserver beendet");

        // Config-AP ebenfalls beenden
        wifiHost.stopAccessPoint();

        Serial.println("[MAIN] Config-AP beendet");
    }
}

// ============================================================================
// SETUP
// ============================================================================

void setup()
{
    Serial.begin(74880);
    delay(500);

    // =========================================================================
    // LED AUS
    // =========================================================================
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);

    // =========================================================================
    // KONFIGURATION LADEN
    // =========================================================================
    configWebServer.begin();

    // =========================================================================
    // HAUS-WLAN VERBINDEN
    // =========================================================================
    Serial.println();
    Serial.println("[MAIN] Versuche Haus-WLAN...");

    bool wifiError = wifiHost.connectToWifi();
    if (!wifiError) {
        Serial.println("[MAIN] WLAN verbunden");
        Serial.print("[MAIN] IP: ");
        Serial.println(WiFi.localIP());
    }
    else {
        Serial.println("[MAIN] Keine WLAN-Verbindung");
    }

    // =========================================================================
    // NTP-ZEIT
    // =========================================================================

    bool timeError = true;

    if (!wifiError) {
        timeError = !waitForValidTime();
    }
    else {
        Serial.println(
            "[MAIN] Keine WLAN-Verbindung -> keine NTP-Zeit"
        );
    }

    // =========================================================================
    // ZEIT ANZEIGEN
    // =========================================================================

    if (!timeError) {
        updateDisplay();
    }
    else {
        Serial.println(
            "[MAIN] Keine gueltige Zeit -> Anzeige wird nicht aktualisiert"
        );
    }

    // =========================================================================
    // CONFIG AP + WEBSERVER
    // =========================================================================
    Serial.println();
    Serial.println("[MAIN] Starte Konfigurations-AP...");

    configWebServer.start();

    Serial.println("[MAIN] Konfigurationsseite bereit");
    Serial.println("[MAIN] Verbinde dich mit WLAN: WordClock");

    Serial.print("[MAIN] Oeffne: http://");
    Serial.print(WiFi.softAPIP());
    Serial.println("/");

    // =========================================================================
    // TIMER
    // =========================================================================
    lastTimeUpdate = millis();
}

// ============================================================================
// LOOP
// ============================================================================

void loop()
{
    // =========================================================================
    // CONFIG WEBSERVER
    // =========================================================================
    handleConfig();

    // =========================================================================
    // SOLANGE CONFIG AKTIV IST NICHT BLOCKIEREN
    // =========================================================================
    if (configWebServer.isActive()) {
        yield();
        delay(10);
        return;
    }

    // =========================================================================
    // NORMALBETRIEB
    // =========================================================================
    bool timeError = swissTime.getTime();

    if (timeError) {
        Serial.println("[MAIN] Keine gueltige Zeit");
        wordClock.showNoInternet();
        delay(5000);
        return;
    }

    // =========================================================================
    // NACHTBETRIEB
    // =========================================================================
    int currentMinutes = swissTime.h * 60 + swissTime.m;
    int sleepMinutes = configWebServer.getSleepHour() * 60 + configWebServer.getSleepMinute();
    int wakeupMinutes = configWebServer.getWakeupHour() * 60 + configWebServer.getWakeupMinute();
    bool sleeping;
    if (sleepMinutes > wakeupMinutes) {
        // Schlafzeit über Mitternacht, z.B. 23:30 -> 06:30
        sleeping = currentMinutes >= sleepMinutes || currentMinutes < wakeupMinutes;
    }
    else {
        // Schlafzeit innerhalb eines Tages, z.B. 01:00 -> 06:30
        sleeping = currentMinutes >= sleepMinutes && currentMinutes < wakeupMinutes;
    }

    if (sleeping) {
        wordClock.allOff();
        delay(1000);
        return;
    }

    // =========================================================================
    // UHRZEIT ANZEIGEN
    // =========================================================================
    updateDisplay();

    // =========================================================================
    // AUF NÄCHSTE MINUTE WARTEN
    // =========================================================================
    swissTime.awaitNextMinuteBoundary();
    yield();
}