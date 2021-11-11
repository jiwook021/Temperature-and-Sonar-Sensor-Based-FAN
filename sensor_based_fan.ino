#include "SR04.h"
#include <dht_nonblocking.h>
#include <LiquidCrystal.h>


#define tempPin 0 
#define Push_Button 2   
#define potPin 3
#define FANPIN 4    
#define ECHO_PIN 5 //sonar sensor
#define TRIG_PIN 6 //sonar sensor


int potpinval;           // variable to store the value coming from the sensor
int Pottempread;
int buttonval;      
boolean mode = true;
float tempC;     
long sonardistance;



LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);


void setup() {
  pinMode(FANPIN, OUTPUT);
  lcd.begin(16, 2);
  delay(500);
  lcd.setCursor(0, 0);
  lcd.print("Track Distance ");
  lcd.setCursor(0, 1);
  lcd.print("On ");
  mode = true;
  delay(3000);
  setupmintemp();
  clearscreen();
}



void clearscreen()
{
  lcd.setCursor(0, 0);
  lcd.print("                 ");
  lcd.setCursor(0, 1);
  lcd.print("                 ");
}

void MinTempcheck()
{
  if (Pottempread < tempC)
  {
    analogWrite(FANPIN, 255);
  }
  else
  {
    analogWrite(FANPIN, 0);
  }
}



void buttoncheck()
{
  
  buttonval = analogRead(Push_Button);    // read the value from the sensor
  if (buttonval > 700)
  {
    ChangeMode();
  }

}

void ChangeMode()
{
  clearscreen();
  if (mode == true)
  {
    lcd.setCursor(0, 0);
    lcd.print("Track Distance ");
    lcd.setCursor(0, 1);
    lcd.print("Off ");
    mode = false;
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.print("Track Distance ");
    lcd.setCursor(0, 1);
    lcd.print("On ");
    mode = true;

  }

  delay(2000);


}

void setupmintemp()
{
  potpinval = analogRead(potPin);    
  Pottempread = potpinval / 20;

  lcd.setCursor(0, 0);
  lcd.print("Minimum Temp Value:");
  lcd.setCursor(0, 1);
  lcd.print(Pottempread);
  lcd.setCursor(3, 1);
  lcd.print("c");

  delay(3000);
}
void potchange() {

  clearscreen();

  delay(100);
  while (potpinval > (analogRead(potPin) + 20) || potpinval < (analogRead(potPin) - 20))

  {

    potpinval = analogRead(potPin);    // read the value from the sensor
    Pottempread = potpinval / 20;

    lcd.setCursor(0, 0);
    lcd.print("Set Min Temp:");
    lcd.setCursor(0, 1);
    lcd.print(Pottempread);
    lcd.setCursor(3, 1);
    lcd.print("c");
    delay(500);
  }

  for (int i = 0; i < 10; i++)
  {
    potpinval = analogRead(potPin);    // read the value from the sensor
    delay(100);
    if (potpinval > (analogRead(potPin) + 20) || potpinval < (analogRead(potPin) - 20))
    {
      potchange();
    }
  }
}

void loop() {

  float temperature;
  float humidity;
  int tempReading = analogRead(tempPin);
  double tempK = log(10000.0 * ((1024.0 / tempReading - 1)));
  tempK = 1 / (0.001129148 + (0.000234125 + (0.0000000876741 * tempK * tempK )) * tempK );       //  Temp Kelvin
  tempC = tempK - 273.15;            // Convert Kelvin to Celcius

 sonardistance = sr04.Distance();



  lcd.setCursor(0, 0);
  lcd.print("Temp        C");
  lcd.setCursor(6, 0);
  lcd.print(tempC);


  if (mode)
  {
    if (sonardistance < 30)
    {
      lcd.setCursor(0, 1);
      lcd.print("Distance    cm");
      lcd.setCursor(9, 1);
      lcd.print(sonardistance);
      MinTempcheck();
    }
    else
    {
      lcd.setCursor(0, 1);
      lcd.print("              ");
      analogWrite(FANPIN, 0);

    }
  }
  if (!(mode))
  {
    MinTempcheck();
  }


  for (int i = 0; i < 5; i++)
  {
    buttoncheck();
    potpinval = analogRead(potPin);    
    delay(100);
    if (potpinval > (analogRead(potPin) + 20) || potpinval < (analogRead(potPin) - 20))
    {
      potchange();
    }
  }
  clearscreen();
}
