# 🚀 ESP8266 Web OTA + Kontrol Servo + LED + Telegram Bot

Proyek ini mendokumentasikan pengembangan sistem kontrol berbasis web dan Telegram untuk Lolin NodeMCU V1.0 (ESP8266 12E). Fitur mencakup pembaruan firmware OTA, kontrol perangkat (servo & LED) via web dan Telegram, serta otomatisasi build firmware menggunakan GitHub Actions.

---

## 📌 Tujuan Proyek

- 🔁 Kompilasi firmware otomatis via GitHub Actions  
- 🌐 Update firmware OTA melalui browser  
- 🧭 Kontrol servo & LED via UI web  
- 🤖 Kontrol servo & LED via Telegram Bot  
- 📶 Menampilkan IP address & kekuatan sinyal WiFi  
- 📱 Antarmuka web modern dan responsif (Bootstrap)

---

## 🛠️ Proses Pengembangan

### 1. Inisialisasi Proyek
Struktur direktori:
- `sketch/sketch.ino` → Kode utama ESP8266
- `.github/workflows/compile.yml` → Workflow GitHub Actions

### 2. Integrasi GitHub Actions
GitHub workflow akan:
- Install board ESP8266
- Install library: `UniversalTelegramBot`, `ArduinoJson`
- Compile `.ino`
- Upload firmware `.bin` sebagai artifact dan GitHub Release

### 3. OTA Update via Web
Menggunakan `ESP8266HTTPUpdateServer` dan `ArduinoOTA`:
- Update langsung dari browser
- Halaman khusus di `/update`

### 4. UI Kontrol Web
- Servo dikontrol via slider (range 0–100%)
- LED dikontrol via switch (active LOW)
- Tampilan IP address dan kekuatan sinyal
- Dibangun dengan **Bootstrap 5** dan kompatibel HP

### 5. Telegram Bot Integration
- Kirim perintah ke bot:
  - `/led_on` dan `/led_off`
  - `/servo_XX` (contoh: `/servo_30`)
  - `/status` (IP dan sinyal WiFi)
- Token dan chat ID dikonfigurasi aman
- Menggunakan `UniversalTelegramBot` & HTTPS

### 6. Kompilasi Otomatis dan Penamaan Firmware
- Nama file otomatis: `esp8266wifi12e-YYYY-MM-DD-HHMM.bin`

---

## 🤖 Telegram Bot Commands

| Perintah       | Fungsi                                 |
|----------------|----------------------------------------|
| `/led_on`      | Nyalakan LED bawaan ESP                |
| `/led_off`     | Matikan LED bawaan ESP                 |
| `/servo_50`    | Gerakkan servo ke 50%                  |
| `/status`      | Tampilkan IP dan kekuatan sinyal WiFi  |

---

## 📱 Aplikasi Android Pendukung

| Aplikasi             | Kegunaan                                    |
|----------------------|---------------------------------------------|
| **ESP8266 Loader**   | Upload .bin ke ESP via serial atau OTG       |
| **GitHub (App)**     | Memicu build, memantau status                |
| **Telegram**         | Mengontrol ESP via chat                      |
| **Google Chrome**    | Mengakses UI dan halaman OTA                 |
| **Notepad / Code**   | Edit cepat langsung dari HP                  |

---

## 📦 Fitur-Fitur

- ✅ Kompilasi firmware otomatis via GitHub Actions  
- ✅ OTA update melalui web browser  
- ✅ Kontrol servo dan LED dari UI web  
- ✅ Kontrol servo dan LED dari Telegram  
- ✅ UI responsif berbasis Bootstrap  
- ✅ Monitoring IP dan sinyal WiFi  

---

## 🔮 Rencana Pengembangan

- Simpan posisi terakhir servo di EEPROM  
- Tambah proteksi password untuk OTA page  
- Tambahkan tombol reboot otomatis setelah OTA  
- Mode gelap (dark mode) untuk UI  
- Log kontrol ke Telegram atau server eksternal  

---

## 👤 Pengembang

Dikembangkan oleh **Karim Roy Tampubolon**

---

## 🗓️ Timeline Singkat

| Tanggal         | Aktivitas                                    |
|------------------|-----------------------------------------------|
| **1 Juli 2025**  | Setup awal OTA dan GitHub Actions             |
| **2 Juli 2025**  | UI web, kontrol servo & LED                   |
| **3 Juli 2025**  | Telegram Bot + dokumentasi final              |
| **11 Juli 2025** | Integrasi otomatisasi library di GitHub       |

---

## 🔥 BONUS: ESP8266 Cheat Sheet

### ⚡ Upload OTA via Web
```cpp
ArduinoOTA.handle(); // di loop()
```
Update firmware dari browser, bahkan lewat HP!

### 🤐 Proteksi OTA
```cpp
ArduinoOTA.setPassword("rahasiamu");
```

### 📶 Cek Sinyal WiFi
```cpp
WiFi.RSSI(); // dBm
```

### 📍 Simpan ke EEPROM
```cpp
EEPROM.write(0, nilai);
EEPROM.commit();
```

### 🔋 Hemat Daya (Deep Sleep)
```cpp
ESP.deepSleep(60e6); // 60 detik
```

### 🌐 DNS Redirect (Captive Portal)
```cpp
dnsServer.start(53, "*", IPAddress(192,168,4,1));
```

### 📁 Upload File SPIFFS
Di Arduino IDE: Tools → **ESP8266 Sketch Data Upload**

---

## ✅ Repositori & Demo

📁 [GitHub Repo](https://github.com/kajukopi/esp8266wifi12e)  
📦 Firmware terbaru: [Releases](https://github.com/kajukopi/esp8266wifi12e/releases)

---

Terima kasih telah mengeksplorasi proyek ini!  
Silakan gunakan atau modifikasi untuk pembelajaran dan pengembangan IoT kamu sendiri.