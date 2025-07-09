# 📄 Penjelasan Langkah demi Langkah: `sketch.ino`

File `sketch.ino` adalah inti dari proyek ESP8266 ini. Program ini bertanggung jawab untuk:

- Menghubungkan ESP8266 ke WiFi
- Menjalankan web server lokal
- Menyediakan halaman web untuk OTA dan kontrol
- Mendukung upload firmware langsung dari browser (OTA)
- Merespons permintaan pengguna melalui berbagai endpoint URL

---

## 🧩 1. Import Library

```cpp
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
```

Library ini digunakan untuk:
- Menghubungkan ESP8266 ke jaringan WiFi
- Menjalankan web server berbasis HTTP

---

## 📶 2. Konfigurasi WiFi

```cpp
const char* ssid = "your-ssid";
const char* password = "your-password";
```

SSID dan password WiFi didefinisikan secara hardcoded agar ESP8266 bisa terhubung otomatis saat dinyalakan.

---

## 🌐 3. Inisialisasi Web Server

```cpp
ESP8266WebServer server(80);
```

Membuat server lokal pada port 80 (port HTTP standar). Semua permintaan dari browser akan ditangani oleh objek `server`.

---

## 🛠️ 4. Fungsi `setup()`

```cpp
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  }
  server.on("/", ...);
  server.on("/upload", ...);
  server.on("/reboot", ...);
  server.on("/ota", ...);
  server.onNotFound(...);
  server.begin();
}
```

### Penjelasan:
- `Serial.begin(115200)`: Memulai komunikasi serial.
- `WiFi.begin(...)`: Menghubungkan ke WiFi.
- `server.on(...)`: Menentukan routing untuk halaman tertentu.
- `server.begin()`: Menyalakan server agar mulai menerima permintaan HTTP.

---

## 🔁 5. Fungsi `loop()`

```cpp
void loop() {
  server.handleClient();
}
```

Server akan terus mengecek apakah ada permintaan masuk dari client (browser) dan meresponsnya sesuai rute yang sudah didefinisikan.

---

## 📁 6. Routing Web Server

```cpp
server.on("/", []() {
  server.send(200, "text/html", index_html);
});
```

### Rute Penting:
| Endpoint     | Fungsi                                      |
|--------------|---------------------------------------------|
| `/`          | Halaman utama (`index.html`)                |
| `/upload`    | Halaman upload firmware (`upload.html`)     |
| `/reboot`    | Menampilkan halaman reboot lalu restart     |
| `/ota`       | Menampilkan halaman OTA                     |
| *(lainnya)*  | Menampilkan `error.html` (jika tidak cocok) |

---

## 💡 7. Halaman HTML dari PROGMEM

Semua halaman web disimpan di flash memory menggunakan `PROGMEM`, lalu dikirim ke browser menggunakan:

```cpp
server.send(200, "text/html", index_html);
```

---

## 🔁 8. OTA Update

Rute `/upload` mengaktifkan fitur Over-The-Air Update. Firmware `.bin` dapat diunggah melalui halaman web, dan ESP akan:

1. Menyimpan file sementara
2. Me-restart setelah upload sukses

---

## 🧠 9. Rute Reboot

```cpp
server.on("/reboot", []() {
  server.send(200, "text/html", reboot_html);
  delay(3000);
  ESP.restart();
});
```

Ketika pengguna mengakses `/reboot`, halaman `reboot.html` ditampilkan lalu ESP akan restart otomatis setelah 3 detik.

---

## 📌 10. Penanganan Error

```cpp
server.onNotFound([]() {
  server.send(404, "text/html", error_html);
});
```

Jika pengguna mengakses URL yang tidak dikenali, maka server akan menampilkan halaman `error.html`.

---

## 📦 11. Struktur Folder Terkait

```
sketch/
├── sketch.ino
├── index.html
├── style.css
├── upload.html
├── upload.css
├── reboot.html
├── ota.html
├── error.html
```

---

## 📌 Catatan Penting

- Semua file `.html` akan disimpan sebagai `const char[]` di file header `.h` dan di-serve dari `PROGMEM`
- Tidak menggunakan SPIFFS atau LittleFS — hanya `PROGMEM`
- Kompatibel dengan board: **NodeMCU ESP8266 v2 (ESP-12E)**

---

## 🔗 Referensi

- [ESP8266 Arduino Core](https://github.com/esp8266/Arduino)
- [ESP8266 WebServer Documentation](https://arduino-esp8266.readthedocs.io/en/latest/esp8266webserver.html)
- [OTA Web Update Tutorial](https://randomnerdtutorials.com/esp8266-nodemcu-ota-over-the-air-arduino/)