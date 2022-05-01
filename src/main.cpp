#define _DISABLE_TLS_
#include <Arduino.h>
#include "setupWifi.h"
#include "blink.h"
#include "fetch.h"
#include "display.h"
#include "temperature.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  createWifiTask();
  Temperature::init();
  Blink::setupTimer();
  Display::init();
  Blink::enable();
  waitForWifi();
  Serial.println("Connected!");
  Blink::disable();
  Display::done();
}

void loop()
{
  Blink::enable();
  Display::fetching();
  Fetch::fetch();
  Display::done();
  Display::render();
  Blink::disable();
  delay(60 * 1000);
}
