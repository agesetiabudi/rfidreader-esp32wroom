#include <SPI.h>
#include <MFRC522.h>

// Definisikan pin untuk SPI, RC522, dan Buzzer
#define SS_PIN 10        // Pin SDA (SS) dihubungkan ke GPIO10 di ESP32-S3
#define RST_PIN 9        // Pin RST dihubungkan ke GPIO9 di ESP32-S3
#define BUZZER_PIN 4     // Pin Buzzer dihubungkan ke GPIO3 di ESP32-S3

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Membuat instance MFRC522

void setup() {
  Serial.begin(115200);           // Memulai komunikasi serial
  SPI.begin(6, 2, 7);             // SPI.begin(SCK, MISO, MOSI) sesuai pin yang dihubungkan ke ESP32-S3
  mfrc522.PCD_Init();             // Inisialisasi RC522
  pinMode(BUZZER_PIN, OUTPUT);    // Set pin buzzer sebagai output
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

  // Menampilkan UID kartu
  Serial.print("UID Kartu: ");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();

  // Memberhentikan komunikasi dengan kartu yang sudah dibaca
  mfrc522.PICC_HaltA();
}
