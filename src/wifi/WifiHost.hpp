#ifndef WifiHost_HPP
#define WifiHost_HPP

#include <Arduino.h>

#include <WifiClientSecure.h>
#include <WiFiManager.h>


class WifiHost
{
public:

    WifiHost(
        int configPortalTimeout,
        int connectTimeout
    );

    bool connectToWifi(void);

    bool disconnectFromWifi(void);

private:

    WiFiManager manager;

    int this_configPortalTimeout;
    int this_connectTimeout;
};

#endif