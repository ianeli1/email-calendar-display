#pragma once
#include <OneWire.h>
#include <DallasTemperature.h>

class Temperature
{
public:
    static DallasTemperature sensor;
    static OneWire oneWire;

    static void init();

    static float getC();
};