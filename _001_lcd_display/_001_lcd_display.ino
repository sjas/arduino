#include <LiquidCrystal.h>




LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

//// comments are debugging stuff
//int L = 13;




void setup() {

  lcd.begin(16, 2);

  //  pinMode(L, OUTPUT);
}




void loop() {

  //  digitalWrite(L, LOW);
  //  delay(1000);

  lcd.setCursor(0, 0);
 
  lcd.print("test");

  lcd.setCursor(4, 1);

  lcd.print("tab+newline");

  //  digitalWrite(L, HIGH);
  //  delay(2500);
}
