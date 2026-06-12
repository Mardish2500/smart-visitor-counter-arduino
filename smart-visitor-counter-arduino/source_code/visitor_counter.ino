#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

const int pirMasuk = 8;
const int pirKeluar = 9;

int masuk = 0;
int keluar = 0;
int total = 0;

void setup()
{
  pinMode(pirMasuk, INPUT);
  pinMode(pirKeluar, INPUT);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0,0);
  lcd.print("SELAMAT DATANG");
  lcd.setCursor(0,1);
  lcd.print("Visitor Counter");
  delay(2000);

  lcd.clear();
}

void loop()
{
  if(digitalRead(pirMasuk) == HIGH)
  {
    masuk++;
    total++;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("MASUK");
    delay(1000);

    while(digitalRead(pirMasuk)==HIGH);
  }

  if(digitalRead(pirKeluar) == HIGH)
  {
    keluar++;

    if(total > 0)
      total--;

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("KELUAR");
    delay(1000);

    while(digitalRead(pirKeluar)==HIGH);
  }

  lcd.setCursor(0,0);
  lcd.print("M:");
  lcd.print(masuk);

  lcd.setCursor(8,0);
  lcd.print("K:");
  lcd.print(keluar);

  lcd.setCursor(0,1);
  lcd.print("Total:");
  lcd.print(total);
  lcd.print("   ");

  delay(200);
}