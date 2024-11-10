#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <Ethernet.h>

#define SS_PIN 10     
#define RST_PIN 9       
#define BUZZER_PIN 19   
#define SDA_PIN 5    
#define SCL_PIN 8     
#define CS_PIN_ETH 16 
#define CS_PIN_TFT 17  

MFRC522 mfrc522(SS_PIN, RST_PIN);  
LiquidCrystal_I2C lcd(0x27, 16, 2); 
RTC_DS3231 rtc;                 
EthernetClient client;         

DateTime sekarang;            
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip;              

void setup() {
  Serial.begin(115200);
  SPI.begin(6, 2, 7);    
  mfrc522.PCD_Init();  
  pinMode(BUZZER_PIN, OUTPUT); 

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

  // Inisialisasi Ethernet
  Ethernet.init(CS_PIN_ETH);
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Gagal mendapatkan IP melalui DHCP, menggunakan IP default.");
  //   Ethernet.begin(mac, IPAddress(192, 168, 1, 177)); // Set IP default jika DHCP gagal
  // }

  ip = Ethernet.localIP(); 
  tampilkanPesanAwal();
  Serial.println("Tempatkan kartu RFID dekat dengan modul...");
}

void loop() {
  sekarang = rtc.now();
  // Serial.print(ip);

  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  digitalWrite(BUZZER_PIN, HIGH);
  delay(200);                    
  digitalWrite(BUZZER_PIN, LOW); 

  // Tampilkan UID Kartu RFID pada LCD dan Serial Monitor
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
  tampilkanPesanAwal();  // Menampilkan pesan awal setelah delay

  mfrc522.PICC_HaltA();   // Menghentikan pembacaan kartu
}

void tampilkanPesanAwal() {
  lcd.clear();
  lcd.setCursor(3, 4);
  lcd.print("Tempatkan kartu");
}

void tampilkanIPdanWaktu() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("IP:");
  lcd.print(ip);

  lcd.setCursor(0, 1);
  lcd.print("Jam: ");
  lcd.print(sekarang.hour());
  lcd.print(":");
  lcd.print(sekarang.minute() < 10 ? "0" : "");
  lcd.print(sekarang.minute());
}