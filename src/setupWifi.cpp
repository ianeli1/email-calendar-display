#include "WiFi.h"

const char *WIFI_SSID = "<ssid>";
const char *WIFI_PASS = "<password>";

bool setupWifi()
{
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  wl_status_t wifi_status;
  while ((wifi_status = WiFi.status()) != WL_CONNECTED)
  {
    if (wifi_status == WL_CONNECT_FAILED)
    {
      return false;
    }
    delay(1000);
  }
  return true;
}