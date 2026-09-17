#include "ConfigWebServer.hpp"


// ============================================================================
// KONSTRUKTOR
// ============================================================================

ConfigWebServer::ConfigWebServer()
    : server(80),
      startTime(0),
      active(false),
      configChanged(false)
{
}


// ============================================================================
// BEGIN
//
// Lädt nur die Konfiguration.
// Der Webserver wird hier noch NICHT gestartet.
// ============================================================================

void ConfigWebServer::begin()
{
    load();

    configChanged = false;
}


// ============================================================================
// START
// ============================================================================

void ConfigWebServer::start()
{
    if (active)
        return;


    Serial.println();
    Serial.println(
        "=== CONFIG WEBSERVER START ==="
    );


    Serial.printf(
        "Sleep:  %02d:%02d\n",
        config.sleepHour,
        config.sleepMinute
    );


    Serial.printf(
        "Wake:   %02d:%02d\n",
        config.wakeupHour,
        config.wakeupMinute
    );


    Serial.printf(
        "Header1: %s\n",
        config.header1 ? "ON" : "OFF"
    );


    Serial.printf(
        "Header2: %s\n",
        config.header2 ? "ON" : "OFF"
    );


    // -------------------------------------------------------------------------
    // Hauptseite
    // -------------------------------------------------------------------------

    server.on(
        "/",
        HTTP_GET,
        [this]()
        {
            handleRoot();
        }
    );


    // -------------------------------------------------------------------------
    // Speichern
    // -------------------------------------------------------------------------

    server.on(
        "/save",
        HTTP_GET,
        [this]()
        {
            handleSave();
        }
    );


    // -------------------------------------------------------------------------
    // Webserver manuell beenden
    // -------------------------------------------------------------------------

    server.on(
        "/close",
        HTTP_GET,
        [this]()
        {
            server.send(
                200,
                "text/html; charset=utf-8",

                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='UTF-8'>"
                "<meta name='viewport' "
                "content='width=device-width,initial-scale=1'>"
                "<title>Woerteruhr</title>"
                "</head>"

                "<body>"

                "<h1>Auswahl beendet</h1>"

                "<p>Der Webserver wurde geschlossen.</p>"

                "</body>"
                "</html>"
            );


            // Dem Browser Zeit geben,
            // die Antwort vollständig zu empfangen.
            delay(100);


            stop();
        }
    );


    // -------------------------------------------------------------------------
    // Server starten
    // -------------------------------------------------------------------------

    server.begin();


    startTime = millis();

    active = true;


    Serial.println(
        "[CONFIG] Webserver gestartet"
    );


    Serial.print(
        "[CONFIG] http://"
    );

    Serial.print(
        WiFi.localIP()
    );

    Serial.println(
        "/"
    );


    Serial.println(
        "[CONFIG] aktiv fuer maximal 120 Sekunden"
    );
}


// ============================================================================
// HANDLE
// ============================================================================

void ConfigWebServer::handle()
{
    if (!active)
        return;


    server.handleClient();


    // -------------------------------------------------------------------------
    // Automatischer Timeout
    // -------------------------------------------------------------------------

    if (
        millis() - startTime >=
        WEB_TIMEOUT_MS
    )
    {
        Serial.println(
            "[CONFIG] Timeout erreicht"
        );

        stop();
    }
}


// ============================================================================
// STOP
// ============================================================================

void ConfigWebServer::stop()
{
    if (!active)
        return;


    server.stop();

    active = false;


    Serial.println(
        "[CONFIG] Webserver beendet"
    );
}


// ============================================================================
// IS ACTIVE
// ============================================================================

bool ConfigWebServer::isActive()
{
    return active;
}


// ============================================================================
// CONFIG CHANGED
// ============================================================================

bool ConfigWebServer::hasConfigChanged() const
{
    return configChanged;
}


// ============================================================================
// CLEAR CONFIG CHANGED
// ============================================================================

void ConfigWebServer::clearConfigChanged()
{
    configChanged = false;
}


// ============================================================================
// LOAD
// ============================================================================

void ConfigWebServer::load()
{
    EEPROM.begin(
        EEPROM_SIZE
    );


    EEPROM.get(
        0,
        config
    );


    // -------------------------------------------------------------------------
    // Keine gültige Konfiguration
    // -------------------------------------------------------------------------

    if (config.magic != CONFIG_MAGIC)
    {
        Serial.println(
            "[CONFIG] Keine gueltige Konfiguration gefunden"
        );


        config.magic = CONFIG_MAGIC;


        config.sleepHour = 23;
        config.sleepMinute = 0;


        config.wakeupHour = 6;
        config.wakeupMinute = 0;


        config.header1 = true;
        config.header2 = true;


        save();
    }
    else
    {
        Serial.println(
            "[CONFIG] Konfiguration geladen"
        );
    }
}


// ============================================================================
// SAVE
// ============================================================================

void ConfigWebServer::save()
{
    EEPROM.put(
        0,
        config
    );


    EEPROM.commit();


    Serial.println(
        "[CONFIG] Konfiguration gespeichert"
    );
}


// ============================================================================
// ROOT
// ============================================================================

void ConfigWebServer::handleRoot()
{
    server.send(
        200,
        "text/html; charset=utf-8",
        htmlPage(false)
    );
}


// ============================================================================
// SAVE HANDLER
// ============================================================================

void ConfigWebServer::handleSave()
{
    // =========================================================================
    // SLEEP
    // =========================================================================

    if (server.hasArg("sleep"))
    {
        String value = server.arg("sleep");

        int separator = value.indexOf(':');


        if (separator > 0)
        {
            config.sleepHour =
                value.substring(
                    0,
                    separator
                ).toInt();


            config.sleepMinute =
                value.substring(
                    separator + 1
                ).toInt();
        }
    }


    // =========================================================================
    // WAKE
    // =========================================================================

    if (server.hasArg("wake"))
    {
        String value = server.arg("wake");

        int separator = value.indexOf(':');


        if (separator > 0)
        {
            config.wakeupHour =
                value.substring(
                    0,
                    separator
                ).toInt();


            config.wakeupMinute =
                value.substring(
                    separator + 1
                ).toInt();
        }
    }


    // =========================================================================
    // HEADER
    // =========================================================================

    config.header1 =
        server.hasArg("header1");


    config.header2 =
        server.hasArg("header2");


    // =========================================================================
    // WERTE BEGRENZEN
    // =========================================================================

    if (config.sleepHour > 23)
        config.sleepHour = 23;


    if (config.sleepMinute > 59)
        config.sleepMinute = 59;


    if (config.wakeupHour > 23)
        config.wakeupHour = 23;


    if (config.wakeupMinute > 59)
        config.wakeupMinute = 59;


    // =========================================================================
    // SPEICHERN
    // =========================================================================

    save();


    configChanged = true;


    // =========================================================================
    // SERIELLE AUSGABE
    // =========================================================================

    Serial.println();

    Serial.println(
        "[CONFIG] Neue Einstellungen:"
    );


    Serial.printf(
        "  Sleep:   %02d:%02d\n",
        config.sleepHour,
        config.sleepMinute
    );


    Serial.printf(
        "  Wake:    %02d:%02d\n",
        config.wakeupHour,
        config.wakeupMinute
    );


    Serial.printf(
        "  Header1: %s\n",
        config.header1 ? "ON" : "OFF"
    );


    Serial.printf(
        "  Header2: %s\n",
        config.header2 ? "ON" : "OFF"
    );


    // =========================================================================
    // ANTWORT
    // =========================================================================

    server.send(
        200,
        "text/html; charset=utf-8",
        htmlPage(true)
    );
}


// ============================================================================
// HTML
// ============================================================================

String ConfigWebServer::htmlPage(bool saved)
{
    String html;


    html += F(
        "<!DOCTYPE html>"
        "<html>"

        "<head>"

        "<meta charset='UTF-8'>"

        "<meta name='viewport' "
        "content='width=device-width,initial-scale=1'>"

        "<title>Woerteruhr</title>"

        "<style>"

        "body{"
            "font-family:Arial,sans-serif;"
            "margin:30px;"
            "max-width:500px;"
        "}"

        "h1{"
            "font-size:26px;"
        "}"

        "label{"
            "display:block;"
            "margin-top:20px;"
            "font-size:18px;"
        "}"

        "input[type=time]{"
            "font-size:20px;"
            "padding:8px;"
            "margin-top:5px;"
            "width:140px;"
        "}"

        ".check{"
            "margin-top:20px;"
        "}"

        ".check label{"
            "margin-top:12px;"
        "}"

        "input[type=checkbox]{"
            "width:20px;"
            "height:20px;"
            "vertical-align:middle;"
            "margin-right:8px;"
        "}"

        "button{"
            "padding:12px 25px;"
            "font-size:18px;"
            "border:0;"
            "border-radius:6px;"
            "cursor:pointer;"
        "}"

        ".save{"
            "margin-top:30px;"
        "}"

        ".close{"
            "margin-top:15px;"
        "}"

        ".message{"
            "margin-top:20px;"
            "padding:12px;"
            "border:1px solid #999;"
            "border-radius:6px;"
            "font-size:17px;"
        "}"

        "</style>"

        "</head>"

        "<body>"

        "<h1>Woerteruhr</h1>"
    );


    // =========================================================================
    // SPEICHERBESTÄTIGUNG
    // =========================================================================

    if (saved)
    {
        html += F(
            "<div class='message'>"
            "<b>Einstellungen gespeichert</b><br><br>"
        );


        // ---------------------------------------------------------------------
        // Sleep
        // ---------------------------------------------------------------------

        html += "Sleep: ";

        html += String(
            config.sleepHour < 10 ? "0" : ""
        );

        html += String(
            config.sleepHour
        );

        html += ":";

        html += String(
            config.sleepMinute < 10 ? "0" : ""
        );

        html += String(
            config.sleepMinute
        );


        html += "<br>";


        // ---------------------------------------------------------------------
        // Wake
        // ---------------------------------------------------------------------

        html += "Wake: ";

        html += String(
            config.wakeupHour < 10 ? "0" : ""
        );

        html += String(
            config.wakeupHour
        );

        html += ":";

        html += String(
            config.wakeupMinute < 10 ? "0" : ""
        );

        html += String(
            config.wakeupMinute
        );


        html += "<br><br>";


        // ---------------------------------------------------------------------
        // Header 1
        // ---------------------------------------------------------------------

        html += "Header 1: ";

        html += config.header1
            ? "EIN"
            : "AUS";


        html += "<br>";


        // ---------------------------------------------------------------------
        // Header 2
        // ---------------------------------------------------------------------

        html += "Header 2: ";

        html += config.header2
            ? "EIN"
            : "AUS";


        html += F(
            "</div>"
        );
    }


    // =========================================================================
    // FORM
    // =========================================================================

    html += F(
        "<form action='/save' method='GET'>"
    );


    // =========================================================================
    // SLEEP
    // =========================================================================

    html += F(
        "<label>Einschlafen</label>"
    );


    html +=
        "<input "
        "type='time' "
        "name='sleep' "
        "value='";


    html += String(
        config.sleepHour < 10 ? "0" : ""
    );


    html += String(
        config.sleepHour
    );


    html += ":";


    html += String(
        config.sleepMinute < 10 ? "0" : ""
    );


    html += String(
        config.sleepMinute
    );


    html += "'>";


    // =========================================================================
    // WAKE
    // =========================================================================

    html += F(
        "<label>Aufwachen</label>"
    );


    html +=
        "<input "
        "type='time' "
        "name='wake' "
        "value='";


    html += String(
        config.wakeupHour < 10 ? "0" : ""
    );


    html += String(
        config.wakeupHour
    );


    html += ":";


    html += String(
        config.wakeupMinute < 10 ? "0" : ""
    );


    html += String(
        config.wakeupMinute
    );


    html += "'>";


    // =========================================================================
    // HEADER
    // =========================================================================

    html += F(
        "<div class='check'>"
    );


    // -------------------------------------------------------------------------
    // Header 1
    // -------------------------------------------------------------------------

    html += "<label>";


    html +=
        "<input "
        "type='checkbox' "
        "name='header1' ";


    if (config.header1)
        html += "checked";


    html +=
        "> Header 1 anzeigen";


    html += "</label>";


    // -------------------------------------------------------------------------
    // Header 2
    // -------------------------------------------------------------------------

    html += "<label>";


    html +=
        "<input "
        "type='checkbox' "
        "name='header2' ";


    if (config.header2)
        html += "checked";


    html +=
        "> Header 2 anzeigen";


    html += "</label>";


    html += F(
        "</div>"
    );


    // =========================================================================
    // SAVE BUTTON
    // =========================================================================

    html += F(
        "<button "
        "class='save' "
        "type='submit'>"
        "Speichern"
        "</button>"
    );


    html += F(
        "</form>"
    );


    // =========================================================================
    // CLOSE BUTTON
    // =========================================================================

    html += F(
        "<form action='/close' method='GET'>"

        "<button "
        "class='close' "
        "type='submit'>"
        "Auswahl beenden"
        "</button>"

        "</form>"
    );


    // =========================================================================
    // ENDE
    // =========================================================================

    html += F(
        "</body>"
        "</html>"
    );


    return html;
}


// ============================================================================
// GET SLEEP
// ============================================================================

uint8_t ConfigWebServer::getSleepHour() const
{
    return config.sleepHour;
}


uint8_t ConfigWebServer::getSleepMinute() const
{
    return config.sleepMinute;
}


// ============================================================================
// GET WAKEUP
// ============================================================================

uint8_t ConfigWebServer::getWakeupHour() const
{
    return config.wakeupHour;
}


uint8_t ConfigWebServer::getWakeupMinute() const
{
    return config.wakeupMinute;
}


// ============================================================================
// GET HEADER
// ============================================================================

bool ConfigWebServer::getHeader1() const
{
    return config.header1;
}


bool ConfigWebServer::getHeader2() const
{
    return config.header2;
}
