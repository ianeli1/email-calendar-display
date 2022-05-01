
#include "temperature.h"

OneWire Temperature::oneWire(4);
DallasTemperature Temperature::sensor(&oneWire);

void Temperature::init()
{
    sensor.begin();
}

float Temperature::getC()
{
    sensor.requestTemperatures();
    return sensor.getTempCByIndex(0);
}