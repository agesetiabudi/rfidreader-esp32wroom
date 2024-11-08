#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define SS_PIN 10   
#define RST_PIN 9   
#define BUZZER_PIN 19  
#define SDA_PIN 5    
#define SCL_PIN 8   

MFRC522 mfrc522(SS_PIN, RST_PIN); 
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);      
  SPI.begin(6, 2, 7);        
  mfrc522.PCD_Init();        
  pinMode(BUZZER_PIN, OUTPUT); 

  Wire.begin(SDA_PIN, SCL_PIN);
  lcd.init();
  lcd.backlight();
  
  tampilkanPesanAwal();
  Serial.println("Tempatkan kartu RFID dekat dengan modul...");
}

void loop() {
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

  // Untuk Serial Monitor juga, tanpa spasi antar byte
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