#include "ConfigWebServer.hpp"


// ============================================================================
// KONSTRUKTOR
// ============================================================================

ConfigWebServer::ConfigWebServer()
    : server(80),
      startTime(0),
      active(false),
      configChanged(false),
      wifiNetworkCount(0)
{
}


// ============================================================================
// BEGIN
//
// Lädt nur die Konfiguration.
// Der AP/Webserver wird hier noch nicht gestartet.
// ============================================================================

void ConfigWebServer::begin()
{
    load();

    configChanged = false;
}


// ============================================================================
// START
//
// Startet:
// 1. eigenen Access Point
// 2. WLAN-Scan
// 3. Webserver
//
// Der eigene AP ist unabhängig vom Haus-WLAN.
// ============================================================================

void ConfigWebServer::start()
{
    if (active)
        return;


    Serial.println();
    Serial.println(
        "========================================"
    );

    Serial.println(
        " CONFIG WEBSERVER START"
    );

    Serial.println(
        "========================================"
    );


    // -------------------------------------------------------------------------
    // AP + WLAN gleichzeitig
    // -------------------------------------------------------------------------

    WiFi.mode(
        WIFI_AP_STA
    );


    // -------------------------------------------------------------------------
    // Eigenes WLAN starten
    // -------------------------------------------------------------------------

    bool apStarted =
        WiFi.softAP(
            "WordClock"
        );


    if (apStarted)
    {
        Serial.println(
            "[CONFIG] Eigener Access Point gestartet"
        );
    }
    else
    {
        Serial.println(
            "[CONFIG] Fehler beim Start des Access Points"
        );
    }


    Serial.print(
        "[CONFIG] Eigene AP-IP: "
    );

    Serial.println(
        WiFi.softAPIP()
    );


    // -------------------------------------------------------------------------
    // WLAN SCAN
    //
    // Der Scan wird bereits beim Start durchgeführt.
    // Dadurch sind die WLANs vorhanden, sobald die Webseite geöffnet wird.
    // -------------------------------------------------------------------------

    scanWifi();


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
    // Uhr speichern
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
    // WLAN verbinden
    // -------------------------------------------------------------------------

    server.on(
        "/wifi",
        HTTP_GET,
        [this]()
        {
            handleWifi();
        }
    );


    // -------------------------------------------------------------------------
    // WLAN erneut scannen
    //
    // Optionaler Button auf der Webseite.
    // Der erste Scan passiert aber automatisch.
    // -------------------------------------------------------------------------

    server.on(
        "/scan",
        HTTP_GET,
        [this]()
        {
            scanWifi();

            server.send(
                200,
                "text/html; charset=utf-8",
                htmlPage(false)
            );
        }
    );


    // -------------------------------------------------------------------------
    // Schließen
    // -------------------------------------------------------------------------

    server.on(
        "/close",
        HTTP_GET,
        [this]()
        {
            handleClose();
        }
    );


    // -------------------------------------------------------------------------
    // Webserver starten
    // -------------------------------------------------------------------------

    server.begin();


    startTime =
        millis();


    active =
        true;


    Serial.println(
        "[CONFIG] Webserver gestartet"
    );


    Serial.print(
        "[CONFIG] Webserver: http://"
    );

    Serial.print(
        WiFi.softAPIP()
    );

    Serial.println(
        "/"
    );
}


// ============================================================================
// HANDLE
// ============================================================================

void ConfigWebServer::handle()
{
    if (!active)
        return;


    // ------------------------------------------------------------------------
    // HTTP-Anfragen bearbeiten
    // ------------------------------------------------------------------------

    server.handleClient();


    // ------------------------------------------------------------------------
    // Timeout prüfen
    // ------------------------------------------------------------------------

    unsigned long elapsed =
        millis() - startTime;


    if (elapsed >= WEB_TIMEOUT_MS)
    {
        Serial.println();
        Serial.println(
            "[CONFIG] ================================"
        );

        Serial.println(
            "[CONFIG] Timeout erreicht"
        );

        Serial.println(
            "[CONFIG] Webserver wird beendet"
        );

        Serial.println(
            "[CONFIG] ================================"
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


    // -------------------------------------------------------------------------
    // AP abschalten
    // -------------------------------------------------------------------------

    WiFi.softAPdisconnect(
        true
    );


    active =
        false;


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
    configChanged =
        false;
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
    // Gültigkeit prüfen
    // -------------------------------------------------------------------------

    if (
        config.magic != CONFIG_MAGIC ||
        config.version != CONFIG_VERSION
    )
    {
        Serial.println(
            "[CONFIG] Keine gueltige Konfiguration gefunden"
        );


        memset(
            &config,
            0,
            sizeof(config)
        );


        config.magic =
            CONFIG_MAGIC;


        config.version =
            CONFIG_VERSION;


        // ---------------------------------------------------------------------
        // Standardwerte Uhr
        // ---------------------------------------------------------------------

        config.sleepHour =
            23;

        config.sleepMinute =
            0;


        config.wakeupHour =
            6;

        config.wakeupMinute =
            0;


        // ---------------------------------------------------------------------
        // Standard Header
        // ---------------------------------------------------------------------

        config.header1 =
            true;

        config.header2 =
            true;


        // ---------------------------------------------------------------------
        // Kein WLAN
        // ---------------------------------------------------------------------

        config.wifiSSID[0] =
            '\0';


        config.wifiPassword[0] =
            '\0';


        save();
    }
    else
    {
        Serial.println(
            "[CONFIG] Konfiguration geladen"
        );
    }


    // -------------------------------------------------------------------------
    // Ausgabe
    // -------------------------------------------------------------------------

    Serial.printf(
        "[CONFIG] Sleep: %02d:%02d\n",
        config.sleepHour,
        config.sleepMinute
    );


    Serial.printf(
        "[CONFIG] Wake:  %02d:%02d\n",
        config.wakeupHour,
        config.wakeupMinute
    );


    Serial.printf(
        "[CONFIG] Header1: %s\n",
        config.header1
            ? "ON"
            : "OFF"
    );


    Serial.printf(
        "[CONFIG] Header2: %s\n",
        config.header2
            ? "ON"
            : "OFF"
    );


    if (
        strlen(config.wifiSSID) > 0
    )
    {
        Serial.printf(
            "[CONFIG] WLAN: %s\n",
            config.wifiSSID
        );
    }
    else
    {
        Serial.println(
            "[CONFIG] Kein WLAN gespeichert"
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

    if (
        server.hasArg("sleep")
    )
    {
        String value =
            server.arg("sleep");


        int separator =
            value.indexOf(':');


        if (
            separator > 0
        )
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

    if (
        server.hasArg("wake")
    )
    {
        String value =
            server.arg("wake");


        int separator =
            value.indexOf(':');


        if (
            separator > 0
        )
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

    if (
        config.sleepHour > 23
    )
        config.sleepHour = 23;


    if (
        config.sleepMinute > 59
    )
        config.sleepMinute = 59;


    if (
        config.wakeupHour > 23
    )
        config.wakeupHour = 23;


    if (
        config.wakeupMinute > 59
    )
        config.wakeupMinute = 59;


    // =========================================================================
    // SPEICHERN
    // =========================================================================

    save();


    configChanged =
        true;


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
        config.header1
            ? "ON"
            : "OFF"
    );


    Serial.printf(
        "  Header2: %s\n",
        config.header2
            ? "ON"
            : "OFF"
    );


    server.send(
        200,
        "text/html; charset=utf-8",
        htmlPage(true)
    );
}


// ============================================================================
// WLAN VERBINDEN
// ============================================================================

void ConfigWebServer::handleWifi()
{
    if (
        !server.hasArg("ssid")
    )
    {
        server.send(
            400,
            "text/plain",
            "SSID fehlt"
        );

        return;
    }


    String newSSID =
        server.arg("ssid");


    newSSID.trim();


    if (
        newSSID.length() == 0
    )
    {
        server.send(
            400,
            "text/plain",
            "SSID ist leer"
        );

        return;
    }


    // =========================================================================
    // PASSWORT
    //
    // LEER = altes Passwort behalten
    // =========================================================================

    String newPassword =
        String(
            config.wifiPassword
        );


    if (
        server.hasArg("password")
    )
    {
        String enteredPassword =
            server.arg("password");


        if (
            enteredPassword.length() > 0
        )
        {
            newPassword =
                enteredPassword;
        }
    }


    Serial.println();
    Serial.println(
        "[CONFIG] Versuche WLAN-Verbindung"
    );


    Serial.print(
        "[CONFIG] SSID: "
    );

    Serial.println(
        newSSID
    );


    // =========================================================================
    // TEMPORÄR VERBINDEN
    //
    // Die alte Konfiguration wird erst gespeichert,
    // wenn die Verbindung erfolgreich ist.
    // =========================================================================

    WiFi.mode(
        WIFI_AP_STA
    );


    WiFi.begin(
        newSSID.c_str(),
        newPassword.c_str()
    );


    unsigned long connectStart =
        millis();


    const unsigned long timeout =
        15000UL;


    while (
        WiFi.status() != WL_CONNECTED &&
        millis() - connectStart < timeout
    )
    {
        server.handleClient();

        delay(250);

        yield();
    }


    // =========================================================================
    // ERFOLGREICH
    // =========================================================================

    if (
        WiFi.status() == WL_CONNECTED
    )
    {
        Serial.println(
            "[CONFIG] WLAN erfolgreich verbunden"
        );


        Serial.print(
            "[CONFIG] IP: "
        );

        Serial.println(
            WiFi.localIP()
        );


        // ---------------------------------------------------------------------
        // Neue Daten erst jetzt speichern
        // ---------------------------------------------------------------------

        setWifiCredentials(
            newSSID,
            newPassword
        );


        save();


        configChanged =
            true;


        server.send(
            200,
            "text/html; charset=utf-8",
            htmlPage(true)
        );


        return;
    }


    // =========================================================================
    // FEHLGESCHLAGEN
    // =========================================================================

    Serial.println(
        "[CONFIG] WLAN-Verbindung fehlgeschlagen"
    );


    // -------------------------------------------------------------------------
    // Alte Konfiguration bleibt erhalten.
    // -------------------------------------------------------------------------

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

        "<h1>WLAN nicht verbunden</h1>"

        "<p>"
        "Die Verbindung konnte nicht hergestellt werden."
        "</p>"

        "<p>"
        "Die bisher gespeicherten WLAN-Daten wurden "
        "nicht überschrieben."
        "</p>"

        "<p>"
        "<a href='/'>Zurück</a>"
        "</p>"

        "</body>"

        "</html>"
    );
}


// ============================================================================
// WLAN SCAN
//
// Wird automatisch beim Start ausgeführt.
// ============================================================================

void ConfigWebServer::scanWifi()
{
    Serial.println();
    Serial.println(
        "[CONFIG] WLAN-Scan gestartet"
    );


    // -------------------------------------------------------------------------
    // AP + Station
    // -------------------------------------------------------------------------

    WiFi.mode(
        WIFI_AP_STA
    );


    // -------------------------------------------------------------------------
    // Alte Ergebnisse löschen
    // -------------------------------------------------------------------------

    WiFi.scanDelete();


    wifiNetworkCount =
        0;


    // -------------------------------------------------------------------------
    // Scan
    // -------------------------------------------------------------------------

    int count =
        WiFi.scanNetworks(
            false,
            true
        );


    Serial.printf(
        "[CONFIG] %d WLAN(s) gefunden\n",
        count
    );


    if (
        count <= 0
    )
    {
        Serial.println(
            "[CONFIG] Keine WLANs gefunden"
        );

        return;
    }


    // =========================================================================
    // Ergebnisse übernehmen
    // =========================================================================

    for (
        int i = 0;
        i < count &&
        wifiNetworkCount < MAX_WIFI_NETWORKS;
        i++
    )
    {
        String ssid =
            WiFi.SSID(i);


        // ---------------------------------------------------------------------
        // Leere SSIDs ignorieren
        // ---------------------------------------------------------------------

        if (
            ssid.length() == 0
        )
        {
            continue;
        }


        wifiNetworks[
            wifiNetworkCount
        ].ssid =
            ssid;


        wifiNetworks[
            wifiNetworkCount
        ].rssi =
            WiFi.RSSI(i);


        wifiNetworks[
            wifiNetworkCount
        ].secure =
            (
                WiFi.encryptionType(i)
                != ENC_TYPE_NONE
            );


        wifiNetworkCount++;
    }


    // -------------------------------------------------------------------------
    // Scan-Ergebnisse löschen
    //
    // Unsere kopierten Ergebnisse bleiben erhalten.
    // -------------------------------------------------------------------------

    WiFi.scanDelete();


    Serial.printf(
        "[CONFIG] %d WLAN(s) fuer Webseite gespeichert\n",
        wifiNetworkCount
    );
}


// ============================================================================
// CLOSE
// ============================================================================

void ConfigWebServer::handleClose()
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


    delay(100);


    stop();
}


// ============================================================================
// WLAN CREDENTIALS
// ============================================================================

void ConfigWebServer::setWifiCredentials(
    const String& ssid,
    const String& password
)
{
    memset(
        config.wifiSSID,
        0,
        sizeof(config.wifiSSID)
    );


    memset(
        config.wifiPassword,
        0,
        sizeof(config.wifiPassword)
    );


    ssid.toCharArray(
        config.wifiSSID,
        sizeof(config.wifiSSID)
    );


    password.toCharArray(
        config.wifiPassword,
        sizeof(config.wifiPassword)
    );
}


// ============================================================================
// GET WLAN SSID
// ============================================================================

String ConfigWebServer::getWifiSSID() const
{
    return String(
        config.wifiSSID
    );
}


// ============================================================================
// GET WLAN PASSWORT
// ============================================================================

String ConfigWebServer::getWifiPassword() const
{
    return String(
        config.wifiPassword
    );
}


// ============================================================================
// WIFI STATUS TEXT
// ============================================================================

String ConfigWebServer::wifiStatusText()
{
    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        return
            "Verbunden";
    }


    return
        "Nicht verbunden";
}


// ============================================================================
// WIFI STATUS CLASS
// ============================================================================

String ConfigWebServer::wifiStatusClass()
{
    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        return
            "connected";
    }


    return
        "notconnected";
}


// ============================================================================
// WIFI NETWORK OPTIONS
// ============================================================================

String ConfigWebServer::wifiNetworkOptions()
{
    String html;


    // =========================================================================
    // Bereits gespeichertes WLAN
    // =========================================================================

    String currentSSID =
        String(
            config.wifiSSID
        );


    // =========================================================================
    // Keine Netzwerke
    // =========================================================================

    if (
        wifiNetworkCount == 0
    )
    {
        html +=
            "<option value=''>"
            "Keine WLANs gefunden"
            "</option>";


        return html;
    }


    // =========================================================================
    // Netzwerke
    // =========================================================================

    for (
        int i = 0;
        i < wifiNetworkCount;
        i++
    )
    {
        String ssid =
            wifiNetworks[i].ssid;


        html +=
            "<option value='";


        // ---------------------------------------------------------------------
        // HTML absichern
        // ---------------------------------------------------------------------

        String escapedSSID =
            ssid;


        escapedSSID.replace(
            "&",
            "&amp;"
        );


        escapedSSID.replace(
            "\"",
            "&quot;"
        );


        escapedSSID.replace(
            "<",
            "&lt;"
        );


        escapedSSID.replace(
            ">",
            "&gt;"
        );


        html +=
            escapedSSID;


        html +=
            "'";


        // ---------------------------------------------------------------------
        // Aktuelles WLAN automatisch auswählen
        // ---------------------------------------------------------------------

        if (
            ssid == currentSSID
        )
        {
            html +=
                " selected";
        }


        html +=
            ">";


        html +=
            escapedSSID;


        html +=
            " (";


        html +=
            String(
                wifiNetworks[i].rssi
            );


        html +=
            " dBm)";


        if (
            wifiNetworks[i].secure
        )
        {
            html +=
                " 🔒";
        }
        else
        {
            html +=
                " 🔓";
        }


        html +=
            "</option>";
    }


    return html;
}


// ============================================================================
// HTML
// ============================================================================

String ConfigWebServer::htmlPage(
    bool saved
)
{
    String html;


    // =========================================================================
    // HEADER
    // =========================================================================

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
            "margin:20px;"
            "max-width:600px;"
            "color:#222;"
        "}"


        "h1{"
            "font-size:28px;"
            "margin-bottom:20px;"
        "}"


        "h2{"
            "font-size:22px;"
            "margin-top:30px;"
        "}"


        ".status{"
            "background:#f3f3f3;"
            "border:1px solid #ccc;"
            "border-radius:8px;"
            "padding:16px;"
            "margin-bottom:25px;"
        "}"


        ".statusTitle{"
            "font-size:20px;"
            "font-weight:bold;"
            "margin-bottom:12px;"
        "}"


        ".connected{"
            "color:#008000;"
            "font-weight:bold;"
        "}"


        ".notconnected{"
            "color:#c00000;"
            "font-weight:bold;"
        "}"


        ".info{"
            "margin-top:8px;"
        "}"


        ".wifiBox{"
            "border:1px solid #ccc;"
            "border-radius:8px;"
            "padding:16px;"
        "}"


        "label{"
            "display:block;"
            "margin-top:16px;"
            "font-size:18px;"
        "}"


        "select,"
        "input[type=text],"
        "input[type=password],"
        "input[type=time]{"
            "font-size:18px;"
            "padding:10px;"
            "margin-top:6px;"
            "width:100%;"
            "box-sizing:border-box;"
        "}"


        "input[readonly]{"
            "background:#f2f2f2;"
        "}"


        "button{"
            "padding:12px 20px;"
            "font-size:17px;"
            "border:0;"
            "border-radius:6px;"
            "cursor:pointer;"
            "margin-top:15px;"
        "}"


        ".hint{"
            "font-size:14px;"
            "color:#666;"
            "margin-top:6px;"
            "line-height:1.4;"
        "}"


        ".message{"
            "margin-top:20px;"
            "padding:14px;"
            "border:1px solid #999;"
            "border-radius:8px;"
            "background:#f5f5f5;"
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


        "</style>"


        // =====================================================================
        // JAVASCRIPT
        // =====================================================================

        "<script>"

        "function selectWifi(){"

            "var list="
            "document.getElementById('wifiList');"

            "var ssid="
            "document.getElementById('ssid');"

            "var hidden="
            "document.getElementById('wifiSSIDHidden');"


            "if(list.value){"

                "ssid.value=list.value;"

                "hidden.value=list.value;"

            "}"

        "}"


        "</script>"


        "</head>"


        "<body>"


        "<h1>Woerteruhr</h1>"
    );


    // =========================================================================
    // STATUS GANZ OBEN
    // =========================================================================

    html += F(
        "<div class='status'>"

        "<div class='statusTitle'>"
        "Status"
        "</div>"


        "<div>"
        "<b>Haus-WLAN:</b> "
    );


    html +=
        "<span class='";


    html +=
        wifiStatusClass();


    html +=
        "'>";


    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        html +=
            "✓ Verbunden";
    }
    else
    {
        html +=
            "✗ Nicht verbunden";
    }


    html +=
        "</span>";


    html +=
        "</div>";


    // =========================================================================
    // SSID
    // =========================================================================

    html +=
        "<div class='info'>"
        "<b>SSID:</b> ";


    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        html +=
            WiFi.SSID();
    }
    else if (
        strlen(config.wifiSSID) > 0
    )
    {
        html +=
            config.wifiSSID;
    }
    else
    {
        html +=
            "keine";
    }


    html +=
        "</div>";


    // =========================================================================
    // IP
    // =========================================================================

    html +=
        "<div class='info'>"
        "<b>IP-Adresse:</b> ";


    if (
        WiFi.status() ==
        WL_CONNECTED
    )
    {
        html +=
            WiFi.localIP().toString();
    }
    else
    {
        html +=
            "-";
    }


    html +=
        "</div>";


    // =========================================================================
    // EIGENER AP
    // =========================================================================

    html +=
        "<div class='info'>"
        "<b>Konfigurations-IP:</b> ";


    html +=
        WiFi.softAPIP().toString();


    html +=
        "</div>";


    html +=
        "</div>";


    // =========================================================================
    // WLAN
    // =========================================================================

    html += F(
        "<h2>WLAN</h2>"


        "<div class='wifiBox'>"


        "<label>"
        "Gefundene WLANs"
        "</label>"


        "<select "
        "id='wifiList' "
        "onchange='selectWifi()'>"
    );


    html +=
        wifiNetworkOptions();


    html += F(
        "</select>"


        "<div class='hint'>"

        "Die WLANs wurden beim Start der Uhr automatisch "
        "gescannt."

        "</div>"


        "<form action='/wifi' method='GET'>"


        "<label>"
        "Ausgewähltes WLAN"
        "</label>"


        "<input "
        "type='text' "
        "id='ssid' "
        "readonly "
        "value='"
    );


    // =========================================================================
    // AKTUELLE SSID
    // =========================================================================

    if (
        strlen(config.wifiSSID) > 0
    )
    {
        html +=
            config.wifiSSID;
    }


    html += F(
        "'>"


        // ---------------------------------------------------------------------
        // Hidden SSID für Formular
        // ---------------------------------------------------------------------

        "<input "
        "type='hidden' "
        "id='wifiSSIDHidden' "
        "name='ssid' "
        "value='"
    );


    if (
        strlen(config.wifiSSID) > 0
    )
    {
        html +=
            config.wifiSSID;
    }


    html += F(
        "'>"


        "<label>"
        "Passwort"
        "</label>"


        "<input "
        "type='password' "
        "name='password' "
        "autocomplete='new-password' "
        "placeholder='Bereits gespeichert – leer lassen'>"


        "<div class='hint'>"

        "Wenn bereits ein Passwort gespeichert ist, "
        "musst du es nicht erneut eingeben. "
        "Leer lassen = bisheriges Passwort behalten."

        "</div>"


        "<button type='submit'>"

        "Mit WLAN verbinden"

        "</button>"


        "</form>"


        "<form action='/scan' method='GET'>"

        "<button type='submit'>"

        "WLANs erneut scannen"

        "</button>"

        "</form>"


        "</div>"
    );


    // =========================================================================
    // SPEICHERBESTÄTIGUNG
    // =========================================================================

    if (
        saved
    )
    {
        html += F(
            "<div class='message'>"

            "<b>Einstellungen gespeichert ✓</b>"

            "</div>"
        );
    }


    // =========================================================================
    // UHR
    // =========================================================================

    html += F(
        "<h2>Uhr</h2>"


        "<form action='/save' method='GET'>"


        "<label>"
        "Einschlafen"
        "</label>"
    );


    // =========================================================================
    // SLEEP TIME
    // =========================================================================

    html +=
        "<input "
        "type='time' "
        "name='sleep' "
        "value='";


    if (
        config.sleepHour < 10
    )
        html +=
            "0";


    html +=
        String(
            config.sleepHour
        );


    html +=
        ":";


    if (
        config.sleepMinute < 10
    )
        html +=
            "0";


    html +=
        String(
            config.sleepMinute
        );


    html +=
        "'>";


    // =========================================================================
    // WAKE TIME
    // =========================================================================

    html += F(
        "<label>"
        "Aufwachen"
        "</label>"
    );


    html +=
        "<input "
        "type='time' "
        "name='wake' "
        "value='";


    if (
        config.wakeupHour < 10
    )
        html +=
            "0";


    html +=
        String(
            config.wakeupHour
        );


    html +=
        ":";


    if (
        config.wakeupMinute < 10
    )
        html +=
            "0";


    html +=
        String(
            config.wakeupMinute
        );


    html +=
        "'>";


    // =========================================================================
    // HEADER
    // =========================================================================

    html += F(
        "<div class='check'>"


        "<label>"

        "<input "
        "type='checkbox' "
        "name='header1' "
    );


    if (
        config.header1
    )
    {
        html +=
            "checked";
    }


    html += F(
        "> Header 1 anzeigen"

        "</label>"


        "<label>"

        "<input "
        "type='checkbox' "
        "name='header2' "
    );


    if (
        config.header2
    )
    {
        html +=
            "checked";
    }


    html += F(
        "> Header 2 anzeigen"

        "</label>"


        "</div>"


        "<button type='submit'>"

        "Einstellungen speichern"

        "</button>"


        "</form>"


        // =========================================================================
        // CLOSE
        // =========================================================================

        "<form action='/close' method='GET'>"

        "<button type='submit'>"

        "Auswahl beenden"

        "</button>"

        "</form>"


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