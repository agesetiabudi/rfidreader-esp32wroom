# Proyek ESP32 dengan RFID, Ethernet, TFT Display, dan RTC

Proyek ini menggunakan ESP32 untuk menghubungkan berbagai modul seperti RFID, Ethernet, TFT Display, dan RTC. Sistem ini menggunakan SPI untuk komunikasi dengan RFID, Ethernet, dan TFT Display. Selain itu, RTC digunakan untuk menampilkan waktu yang terakurat pada LCD.

## Tabel Pin

Berikut adalah tabel pin untuk menghubungkan berbagai modul ke ESP32:

| Modul            | Pin        | Keterangan                              |
|------------------|------------|-----------------------------------------|
| **SPI (SCK, MISO, MOSI)** | 6, 2, 7   | Pin untuk SCK, MISO, MOSI               |
| **RFID (MFRC522)** | CS: 10, RST: 9 | Pin Chip Select (CS) dan Reset (RST) untuk RFID |
| **Ethernet (W5500)** | CS: 16     | Pin Chip Select (CS) untuk Ethernet     |
| **TFT Display**    | CS: 17     | Pin Chip Select (CS) untuk TFT Display  |
| **Buzzer**         | 19         | Pin untuk Buzzer                        |
| **I2C SDA & SCL**  | SDA: 5, SCL: 8 | Pin untuk I2C (SDA dan SCL) untuk RTC dan LCD |
