#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);
Servo palang;

//================ PIN ==================
const int irMasuk  = 13;
const int irKeluar = 12;

const int servoPin = 9;
const int buzzer   = 10;

// Sensor IR umumnya aktif LOW
const int STATUS_TERDETEKSI = LOW;

//================ KAPASITAS PARKIR ==================
const int MAX_KAPASITAS = 10;   // ubah sesuai kapasitas lahan parkir

//================ VARIABEL ==================
int masuk  = 0;
int keluar = 0;
int total  = 0;

bool statusMasuk  = false;
bool statusKeluar = false;

//================ FUNGSI BANTUAN ==================
bool parkirPenuh() {
  return total >= MAX_KAPASITAS;
}

//================ LCD ==================
void tampilkanStatus() {

  lcd.setCursor(0, 0);
  lcd.print("M:");
  lcd.print(masuk);
  lcd.print("   ");

  lcd.setCursor(8, 0);
  lcd.print("K:");
  lcd.print(keluar);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  if (parkirPenuh()) {
    lcd.print("PENUH (");
    lcd.print(total);
    lcd.print("/");
    lcd.print(MAX_KAPASITAS);
    lcd.print(") ");
  } else {
    lcd.print("Total:");
    lcd.print(total);
    lcd.print("/");
    lcd.print(MAX_KAPASITAS);
    lcd.print("  ");
  }
}

//================ PALANG MASUK ==================
void bukaPalangMasuk() {

  tone(buzzer, 1000, 200);

  palang.write(90);
  delay(1500);

  noTone(buzzer);

  palang.write(0);
  delay(500);
}

//================ PALANG KELUAR ==================
void bukaPalangKeluar() {

  palang.write(90);
  delay(1500);

  palang.write(0);
  delay(500);
}

//================ NOTIFIKASI PARKIR PENUH ==================
void tolakMasukParkirPenuh() {

  // Servo TIDAK bergerak sama sekali, hanya buzzer peringatan
  for (int i = 0; i < 3; i++) {
    tone(buzzer, 2000, 150);
    delay(200);
  }
  noTone(buzzer);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("MAAF, PARKIR");
  lcd.setCursor(0, 1);
  lcd.print("PENUH!");
  delay(1500);
  lcd.clear();
}

//================ SETUP ==================
void setup() {

  pinMode(irMasuk, INPUT);
  pinMode(irKeluar, INPUT);

  pinMode(buzzer, OUTPUT);

  palang.attach(servoPin);
  palang.write(0);

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(" SISTEM PARKIR ");
  lcd.setCursor(0, 1);
  lcd.print("   SIAP...");

  delay(2000);

  lcd.clear();
}

//================ LOOP ==================
void loop() {

  //========== MOBIL MASUK ==========
  if (digitalRead(irMasuk) == STATUS_TERDETEKSI && !statusMasuk) {

    statusMasuk = true;

    if (parkirPenuh()) {
      // Kapasitas penuh -> palang TIDAK dibuka
      tolakMasukParkirPenuh();

    } else {
      masuk++;
      total++;

      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("MOBIL MASUK");
      lcd.setCursor(0, 1);
      lcd.print("PALANG BUKA");

      bukaPalangMasuk();

      lcd.clear();
    }
  }

  if (digitalRead(irMasuk) != STATUS_TERDETEKSI) {
    statusMasuk = false;
  }

  //========== MOBIL KELUAR ==========
  if (digitalRead(irKeluar) == STATUS_TERDETEKSI && !statusKeluar) {

    statusKeluar = true;

    keluar++;

    if (total > 0) {
      total--;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("MOBIL KELUAR");
    lcd.setCursor(0, 1);
    lcd.print("PALANG BUKA");

    bukaPalangKeluar();

    lcd.clear();
  }

  if (digitalRead(irKeluar) != STATUS_TERDETEKSI) {
    statusKeluar = false;
  }

  tampilkanStatus();

  delay(100);
}
