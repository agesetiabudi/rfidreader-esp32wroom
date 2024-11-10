#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>

#define SS_PIN 10   
#define RST_PIN 9   
#define BUZZER_PIN 19  
#define SDA_PIN 5    
#define SCL_PIN 8   

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS3231 rtc;

// Variabel global untuk menyimpan waktu
DateTime sekarang;

void setup() {
  Serial.begin(115200);      
  SPI.begin(6, 2, 7);        
  mfrc522.PCD_Init();        
  pinMode(BUZZER_PIN, OUTPUT); 

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();

  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, set ulang waktu...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  tampilkanPesanAwal();
  Serial.println("Tempatkan kartu RFID dekat dengan modul...");
}

void loop() {
  sekarang = rtc.now();  // Ambil waktu sekarang setiap loop
  Serial.print("Waktu sekarang: ");
  Serial.print(sekarang.timestamp(DateTime::TIMESTAMP_TIME));
  // Serial.println(" (Asia/Jakarta)");

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);                    
  digitalWrite(BUZZER_PIN, LOW); 

  Serial.print("UID Kartu: ");
  lcd.clear();
  lcd.setCursor(3, 4);

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    lcd.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    lcd.print(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("UID Kartu: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  delay(1000);
  tampilkanPesanAwal();

  mfrc522.PICC_HaltA();
}

void tampilkanPesanAwal() {
  lcd.clear();
  lcd.setCursor(3, 4);
  lcd.print("Tempatkan kartu");
}