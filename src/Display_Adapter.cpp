#include "Display_Adapter.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);

DisplayAdapter::DisplayAdapter() {
  _lcdRef = &lcd;
}

void DisplayAdapter::begin() {
  lcd.init();
  lcd.clear();
  lcd.backlight();
}

void DisplayAdapter::render(ProgramCollection* programCollection) {
  
}

void DisplayAdapter::render(int16_t col, int16_t row, char* text) {
  if (text != NULL && strlen(text) > 0) {
    lcd.setCursor(col, row);
    lcd.print(text);
  }
}

void DisplayAdapter::clear() {
  lcd.clear();
}
