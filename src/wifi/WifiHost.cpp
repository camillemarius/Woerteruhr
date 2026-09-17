#include "WifiHost.hpp"


WifiHost::WifiHost(
    int configPortalTimeout,
    int connectTimeout
)
    : this_configPortalTimeout(configPortalTimeout),
      this_connectTimeout(connectTimeout)
{
}


bool WifiHost::connectToWifi(void)
{
    manager.setConfigPortalTimeout(this_configPortalTimeout);
    manager.setConnectTimeout(this_connectTimeout);


    if (manager.autoConnect("Clocktwo")) {
        return false;
    }
    Serial.println("[WIFI] There is no Wifi Connection, not even after timeout");

    return true;
}


bool WifiHost::disconnectFromWifi(void)
{
    return manager.disconnect();
}