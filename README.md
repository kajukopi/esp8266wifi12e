# 🚀 ESP8266 Web OTA + Kontrol Servo + LED

Proyek ini mendokumentasikan pengembangan sistem kontrol berbasis web yang responsif untuk Lolin NodeMCU V1.0 (ESP8266 12E), dengan fitur pembaruan firmware OTA (Over-the-Air) dan kontrol perangkat melalui browser. Sistem ini memungkinkan kontrol motor servo dan LED bawaan melalui antarmuka web yang intuitif.

---

## 📌 Tujuan Proyek

- Mengotomatisasi upload firmware `.bin` ke ESP8266 menggunakan GitHub Actions.
- Mengaktifkan pembaruan firmware OTA melalui antarmuka web.
- Mengontrol motor servo menggunakan slider HTML.
- Menghidupkan/mematikan LED onboard dari switch di browser.
- Menampilkan alamat IP dan kekuatan sinyal WiFi.
- Membangun antarmuka yang bersih, responsif, dan ramah seluler menggunakan Bootstrap.

---

## 🛠️ Proses Pengembangan

### 1. Inisialisasi Proyek
Menyiapkan struktur proyek:
- `sketch.ino` untuk kode Arduino.
- Workflow GitHub Actions (`compile.yml`) untuk otomatisasi.

### 2. Konfigurasi GitHub Actions
Mengatur CI/CD GitHub untuk:
- Instal `arduino-cli`.
- Menambahkan index board ESP8266.
- Meng-compile kode dan mengekspor `.bin`.
- Mengunggah firmware sebagai artifact yang dapat diunduh.

### 3. Penamaan Firmware Otomatis
Setiap build firmware diberi nama berdasarkan tanggal saat itu agar mudah dalam pengelolaan versi.

### 4. Web Server & OTA Dasar
Mengaktifkan pembaruan firmware OTA melalui `ESP8266HTTPUpdateServer` dan `ArduinoOTA`, yang disajikan lewat web server lokal.

### 5. Implementasi Kontrol Servo
Menghubungkan motor servo ke pin D5 dan mengaktifkan kontrol melalui slider di web.

### 6. Perbaikan Jangkauan Servo
Meningkatkan gerakan servo dari 90° terbatas menjadi jangkauan ~170° yang lebih responsif dengan menyesuaikan logika kontrol.

### 7. Penyempurnaan Antarmuka
Menggunakan Bootstrap untuk membuat tampilan yang bersih, modern, dan responsif lengkap dengan tombol dan jarak yang nyaman.

### 8. Fitur Saklar LED
Menambahkan switch di UI web untuk mengontrol LED bawaan ESP8266 (aktif LOW).

### 9. Tampilkan Info Jaringan
Menampilkan alamat IP dan kekuatan sinyal WiFi ESP8266 di antarmuka web.

### 10. Optimasi untuk Browser HP
Mengoptimalkan layout antarmuka agar nyaman digunakan di browser seluler.

---

## 📱 Aplikasi yang Digunakan

Berikut daftar aplikasi Android (APK) yang digunakan selama pengembangan dan pengujian:

| Aplikasi           | Kegunaan                                         |
|--------------------|--------------------------------------------------|
| **ESP8266 Loader** | Upload firmware `.bin` ke ESP8266 via OTG/serial |
| **GitHub (App)**   | Mengelola repository, memicu aksi otomatis       |
| **Google Chrome**  | Mengakses antarmuka web & halaman OTA            |
| **Notepad**        | Edit cepat & mencatat selama pengkodean          |

---

## 📦 Fitur-Fitur

- ✅ Kompilasi firmware via GitHub Actions  
- ✅ Update firmware OTA lewat browser  
- ✅ Kontrol servo dengan slider web  
- ✅ Saklar LED bawaan via web  
- ✅ Tampilkan IP address dan kekuatan sinyal WiFi  
- ✅ Antarmuka responsif dengan Bootstrap  

---

## 🔮 Rencana Pengembangan

- Menyimpan posisi terakhir servo di EEPROM  
- Auto-reboot setelah OTA  
- Proteksi password untuk halaman OTA  
- Mode gelap (dark mode)  
- Log aktivitas servo & LED ke memori atau server  

---

## 👤 Pengembang

Dikembangkan oleh **Karim Roy Tampubolon**

---

## 🗓️ Ringkasan Timeline

- **1 Juli 2025**: Setup GitHub Actions, server OTA, kompilasi awal  
- **1–2 Juli 2025**: Implementasi servo, toggle LED, UI, tampilan seluler  
- **3 Juli 2025**: Finalisasi, info jaringan, changelog, dokumentasi  

---

Terima kasih telah mengeksplorasi proyek ini!  
Silakan gunakan atau modifikasi untuk pembelajaran dan pengembangan IoT kamu sendiri.