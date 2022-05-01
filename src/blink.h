#pragma once
#include "Arduino.h"

class Blink
{
public:
    static volatile int interruptCounter;
    static bool enabled;

    static hw_timer_t *timer;
    static portMUX_TYPE timerMux;

    static void IRAM_ATTR onTimer();

    static void setupTimer();

    static void enable();
    static void disable();
};
