#include <LiquidCrystal.h>





LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// thermal sensor
int TMP36 = A0;

int temperature = 0;
int temperatureSum = 0;

//always change these together
int temperatures[10];
int temperatureCount = 10;
//variables using other variables does not work, so next line is just broken
//int temperatures[temperatureCount];

int waitTime = 20;
int refreshTime = 1000;




void setup() {

  lcd.begin(16, 2);
  // serial stuff was just used for debugging
  Serial.begin(9600);
}





void loop() {

  temperature = 0;
  temperatureSum = 0;

  for (int i = 0; i < temperatureCount; i++) {
    // grad celsius
    temperatures[i] = map(analogRead(TMP36), 0, 410, -50, 150);
    // alternativ: grad fahrenheit
    //    temperatures[i] = map(analogRead(TMP36), 0, 410, -58, 302);
    delay(waitTime);
  }

  for (int i = 0; i < temperatureCount; i++) {
    Serial.print(temperatures[i]);
    Serial.print(" ");
    temperatureSum += temperatures[i];
  }

  temperature = temperatureSum / temperatureCount;

  lcd.setCursor(0, 0);
  lcd.print("Temperatur");
  lcd.setCursor(0, 1);
  lcd.print(temperature);
  lcd.setCursor(2, 1);
  lcd.print(" Grad Celsius");

  Serial.print(temperature);
  Serial.println(" Grad Celsius");


  delay(refreshTime);
}
