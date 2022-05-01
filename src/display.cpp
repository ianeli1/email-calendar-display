#include "display.h"
#include "temperature.h"
#include "fetch.h"

LiquidCrystal_I2C Display::lcd((PCF8574_address)0x27);

byte gmail[] = {B00000, B00000, B10001, B11011, B10101, B10001, B00000, B00000};

void Display::init()
{
    lcd.begin(columns, rows);
    lcd.createChar(0, gmail);
    lcd.noBacklight();
    lcd.setCursor(0, 0);
    Display::lcd.backlight();
    lcd.println("Starting...");
}

void Display::done()
{
    lcd.clear();
    lcd.home();
    lcd.println("Done!");
}

void Display::fetching()
{
    lcd.clear();
    lcd.home();
    lcd.println("Fetching...");
}

void Display::loop()
{
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.printf("%fC", Temperature::getC());
}

void Display::render()
{
    lcd.clear();
    ApiResponse res = Fetch::getJson();
    lcd.setCursor(0, 0);
    lcd.printf("%s %s", res.time, res.date);
    lcd.setCursor(17, 0);
    lcd.printf("%iC", (int)Temperature::getC());
    lcd.setCursor(0, 1);
    lcd.write(0);
    lcd.print(res.unread);
    lcd.setCursor(0, 2);
    lcd.print(res.subject);
    lcd.setCursor(0, 3);
    lcd.printf(">%s", res.from);
}