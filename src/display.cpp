#include "display.h"

LiquidCrystal_I2C Display::lcd((PCF8574_address)0x27);

void Display::init()
{
    lcd.begin(columns, rows);
    lcd.noBacklight();
    lcd.setCursor(0, 0);
}

void Display::loop()
{
}