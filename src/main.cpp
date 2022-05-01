
#include <Arduino.h>
#include "setupWifi.h"
#include "blink.h"
#include "fetch.h"
#include "display.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  Blink::setupTimer();
  Blink::enable();
  setupWifi();
  Serial.println("Connected!");
  Blink::disable();
  FetchResponse fetchRes = Fetch::fetch();
  if (fetchRes.error)
  {
    Serial.println("An error ocurred!");
  }
  else
  {
    Serial.println("Req:");
    Serial.println(Fetch::req);
    Serial.printf("Body (%i chars):\n", fetchRes.length);
    Serial.println(fetchRes.res);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}
