#include "display.h"
#include "temperature.h"

LiquidCrystal_I2C Display::lcd((PCF8574_address)0x27);

byte gmail[] = {B00000, B00000, B10001, B11011, B10101, B10001, B00000, B00000};

void Display::init()
{
    lcd.begin(columns, rows);
    lcd.createChar(0, gmail);
    lcd.noBacklight();
    lcd.setCursor(0, 0);
}

void Display::loop()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("%fC", Temperature::getC());
}