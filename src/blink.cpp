#include "blink.h"
#define LED 2

void IRAM_ATTR Blink::onTimer()
{
    portENTER_CRITICAL_ISR(&timerMux);
    if (enabled)
    {
        digitalWrite(LED, !digitalRead(LED));
    }
    else
    {
        digitalWrite(LED, false);
    }
    portEXIT_CRITICAL_ISR(&timerMux);
}

void Blink::setupTimer()
{
    pinMode(LED, OUTPUT);

    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, 1000000, true);
    timerAlarmEnable(timer);
}

void Blink::enable()
{
    enabled = true;
}

void Blink::disable()
{
    enabled = false;
}

bool Blink::enabled = false;
hw_timer_t *Blink::timer = NULL;
portMUX_TYPE Blink::timerMux = portMUX_INITIALIZER_UNLOCKED;
