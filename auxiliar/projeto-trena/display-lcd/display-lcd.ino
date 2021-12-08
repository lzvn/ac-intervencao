#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);

void setup() {
  lcd.init();
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Hello World!");
  lcd.setCursor(0, 1);
  lcd.print("Hello World!");

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("Hello");
  lcd.setCursor(5, 1);
  lcd.print("World!");
}

void loop() {
}
