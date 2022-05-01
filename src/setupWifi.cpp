#include "WiFi.h"
#include "setupWifi.h"

const char *WIFI_SSID = "<ssid>";
const char *WIFI_PASS = "<password>";

#define WIFI_TIMEOUT_MS 20000
#define WIFI_RECOVER_TIME_MS 30000
bool wifi_connected = false;

void waitForWifi()
{
  while (!wifi_connected)
  {
    delay(5000);
  }
}

void keepWiFiAlive(void *parameter)
{
  for (;;)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      wifi_connected = true;
      vTaskDelay(10000 / portTICK_PERIOD_MS);
      continue;
    }
    wifi_connected = false;

    Serial.println("[WIFI] Connecting");
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);

    unsigned long startAttemptTime = millis();

    // Keep looping while we're not connected and haven't reached the timeout
    while (WiFi.status() != WL_CONNECTED &&
           millis() - startAttemptTime < WIFI_TIMEOUT_MS)
    {
    }

    // When we couldn't make a WiFi connection (or the timeout expired)
    // sleep for a while and then retry.
    if (WiFi.status() != WL_CONNECTED)
    {
      Serial.println("[WIFI] FAILED");
      vTaskDelay(WIFI_RECOVER_TIME_MS / portTICK_PERIOD_MS);
      continue;
    }

    Serial.println("[WIFI] Connected: " + WiFi.localIP());
  }
}

void createWifiTask()
{
  xTaskCreatePinnedToCore(
      keepWiFiAlive,
      "keepWiFiAlive", // Task name
      5000,            // Stack size (bytes)
      NULL,            // Parameter
      1,               // Task priority
      NULL,            // Task handle
      ARDUINO_RUNNING_CORE);
}