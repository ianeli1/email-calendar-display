#pragma once
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Display
{
public:
    static const byte columns = 20;
    static const byte rows = 4;
    static LiquidCrystal_I2C lcd;

    void init();
    void loop();
};