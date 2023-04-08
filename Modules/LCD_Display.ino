#include <LiquidCrystal_I2C.h>

// Replace the I2C address with that of your device
// Here used a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  lcd.init();
  lcd.clear();         
  lcd.backlight();   // Making sure that backlight is on
  
  // Print a message on both lines of the LCD.
  lcd.setCursor(2,0);   //Set cursor to character 2 on line 0
  lcd.print("Hello World!");
  
  lcd.setCursor(2,1);   //Move cursor to character 2 on line 1
  lcd.print("LCD Tutorial");
}

void loop() {

}