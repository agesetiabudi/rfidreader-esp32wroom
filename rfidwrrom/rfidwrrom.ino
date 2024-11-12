#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <SD_MMC.h>
#include <ArduinoJson.h>

#define SS_PIN 10
#define RST_PIN 9       
#define BUZZER_PIN 19   
#define SDA_PIN 5    
#define SCL_PIN 8     
#define SD_MMC_CMD 38 // Please do not modify it.
#define SD_MMC_CLK 39 // Please do not modify it.
#define SD_MMC_D0 40  // Please do not modify it.

MFRC522 mfrc522(SS_PIN, RST_PIN);  
LiquidCrystal_I2C lcd(0x27, 16, 2); 
RTC_DS3231 rtc;                 
DateTime sekarang;              

void setup() {
  Serial.begin(115200);
  
  // Inisialisasi RFID
  SPI.begin(6, 2, 7);    
  mfrc522.PCD_Init();  
  pinMode(BUZZER_PIN, OUTPUT); 

  // Inisialisasi LCD
  Wire.begin(SDA_PIN, SCL_PIN); 
  lcd.init();             
  lcd.backlight();        

  // Inisialisasi RTC
  if (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC kehilangan daya, set ulang waktu...");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); 
  }

  // Inisialisasi SD card
  SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
  if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.println("No SD_MMC card attached");
    return;
  }
  Serial.print("SD_MMC Card Type: ");
  if(cardType == CARD_MMC){
    Serial.println("MMC");
  } else if(cardType == CARD_SD){
    Serial.println("SDSC");
  } else if(cardType == CARD_SDHC){
    Serial.println("SDHC");
  } else {
    Serial.println("UNKNOWN");
  }

  uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
  Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

  // Tampilkan pesan awal di LCD
  tampilkanPesanAwal();

  Serial.println("Tempatkan kartu RFID dekat dengan modul...");
}

void loop() {
  sekarang = rtc.now();

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  tampilkanIDKartu();
  delay(1000);
}

void tampilkanPesanAwal() {
  lcd.clear();
  lcd.setCursor(3, 4);
  lcd.print("Tempatkan kartu");
}

void tampilkanIDKartu() {
  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);
  digitalWrite(BUZZER_PIN, LOW); 

  lcd.clear();
  lcd.setCursor(3, 4);

  String uidString = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uidString += String(mfrc522.uid.uidByte[i], HEX);
  }

  lcd.print(uidString); // Tampilkan UID di LCD

  Serial.print("UID Kartu: ");
  Serial.println(uidString);
  
  simpanDataKeSD(uidString);

  mfrc522.PICC_HaltA();   // Menghentikan pembacaan kartu
}

void simpanDataKeSD(String uid) {
  File file = SD_MMC.open("/data.json", FILE_READ);

  if (!file) {
    // Jika file tidak ada, buat file baru
    Serial.println("File tidak ditemukan. Membuat file baru...");

    file = SD_MMC.open("/data.json", FILE_WRITE);
    if (!file) {
      Serial.println("Gagal membuka file untuk menulis.");
      return;
    }

    // Menulis data awal (JSON kosong)
    DynamicJsonDocument doc(1024);
    JsonArray array = doc.createNestedArray("data");

    // Menambahkan data pertama kali
    JsonObject data = array.createNestedObject();
    data["uid"] = uid;
    data["timestamp"] = sekarang.timestamp();

    // Menulis data JSON ke file
    serializeJson(doc, file);
    file.close();
    Serial.println("File baru dibuat dan data disimpan.");
    return;
  }

  // Jika file ada, baca dan deserialisasi JSON
  DynamicJsonDocument doc(1024);
  DeserializationError error = deserializeJson(doc, file);
  if (error) {
    Serial.println("Gagal mendeserialisasi JSON.");
    file.close();
    return;
  }
  file.close();

  // Menambahkan data baru ke dalam JSON
  JsonArray array = doc["data"];
  JsonObject data = array.createNestedObject();
  data["uid"] = uid;
  data["timestamp"] = sekarang.timestamp();

  // Menulis ulang data JSON ke file
  file = SD_MMC.open("/data.json", FILE_WRITE);
  if (!file) {
    Serial.println("Gagal membuka file untuk menulis.");
    return;
  }
  serializeJson(doc, file);
  file.close();

  Serial.println("UID disimpan ke SD card.");
}
