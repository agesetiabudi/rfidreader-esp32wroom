#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>  // Gunakan library LCD I2C

// Definisikan pin untuk SPI, RC522, Buzzer, dan LCD I2C
#define SS_PIN 10          // Pin SDA (SS) dihubungkan ke GPIO10 di ESP32-S3
#define RST_PIN 9          // Pin RST dihubungkan ke GPIO9 di ESP32-S3
#define BUZZER_PIN 4       // Pin Buzzer dihubungkan ke GPIO4 di ESP32-S3
#define SDA_PIN 5          // Pin SDA untuk I2C
#define SCL_PIN 3          // Pin SCL untuk I2C

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Membuat instance MFRC522
LiquidCrystal_I2C lcd(0x27, 16, 2); // Membuat instance LCD I2C pada alamat 0x27 (ubah jika perlu)

void setup() {
  Serial.begin(115200);           // Memulai komunikasi serial
  SPI.begin(6, 2, 7);             // SPI.begin(SCK, MISO, MOSI) sesuai pin yang dihubungkan ke ESP32-S3
  mfrc522.PCD_Init();             // Inisialisasi RC522
  pinMode(BUZZER_PIN, OUTPUT);    // Set pin buzzer sebagai output

  // Inisialisasi I2C untuk LCD
  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.begin();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Tempatkan kartu");

  Serial.println("Tempatkan kartu RFID dekat dengan modul...");
}

void loop() {
  // Periksa apakah ada kartu RFID yang terdeteksi
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Menyalakan buzzer saat kartu terdeteksi
  digitalWrite(BUZZER_PIN, HIGH);  // Nyalakan buzzer
  delay(200);                      // Durasi buzzer berbunyi
  digitalWrite(BUZZER_PIN, LOW);   // Matikan buzzer

  // Menampilkan UID kartu di Serial dan LCD
  Serial.print("UID Kartu: ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("UID Kartu:");
  lcd.setCursor(0, 1);

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
    lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Memberhentikan komunikasi dengan kartu yang sudah dibaca
  mfrc522.PICC_HaltA();
}
