## 📂 Penjelasan Folder `sketch/`

Folder `sketch/` berisi kode utama proyek ESP8266, yang akan dikompilasi menjadi firmware `.bin` melalui workflow GitHub Actions. File `.ino` di dalamnya adalah inti dari program yang akan di-flash ke NodeMCU ESP8266.

---

### 📁 Daftar Isi

| File | Deskripsi |
|------|-----------|
| [`sketch.ino`](./sketch.ino) | File utama Arduino sketch, berisi kode program untuk ESP8266. |
| [`index.html`](./index.html) | Halaman web lokal yang ditampilkan oleh web server ESP8266. |
| [`style.css`](./style.css) | File CSS untuk mempercantik tampilan halaman HTML. |
| [`reboot.html`](./reboot.html) | Halaman web yang muncul saat ESP8266 direstart. |
| [`upload.html`](./upload.html) | Halaman untuk mengunggah firmware baru (OTA update). |
| [`upload.css`](./upload.css) | Tampilan khusus untuk halaman upload firmware. |
| [`ota.html`](./ota.html) | Halaman utama untuk fitur OTA (Over-the-Air) upload firmware. |
| [`error.html`](./error.html) | Halaman error default jika ada kesalahan akses. |

---

### 📄 Penjelasan Setiap File

#### `sketch.ino`
Program utama Arduino yang:
- Menghubungkan ESP8266 ke jaringan WiFi.
- Membuat web server lokal.
- Menyediakan halaman web interaktif untuk mengontrol LED dan servo.
- Mendukung upload firmware via browser (OTA update).

#### `index.html`
- Halaman web default yang ditampilkan ketika IP ESP8266 diakses.
- Berisi kontrol UI seperti tombol dan slider untuk mengendalikan perangkat seperti LED dan servo.

#### `style.css`
- File gaya (CSS) untuk mempercantik tampilan halaman `index.html`.
- Mengatur layout, warna, font, dan animasi UI.

#### `reboot.html`
- Halaman sederhana yang ditampilkan ketika ESP8266 selesai restart.
- Memberikan informasi atau animasi bahwa device sedang reboot.

#### `upload.html`
- Halaman yang memungkinkan pengguna untuk memilih dan mengunggah file `.bin` firmware baru langsung lewat browser.
- Digunakan untuk fitur OTA update (update tanpa kabel).

#### `upload.css`
- Gaya tampilan untuk halaman `upload.html`.
- Mengatur agar halaman upload tampil simpel dan responsif.

#### `ota.html`
- Halaman utama untuk fitur OTA update.
- Biasanya digunakan untuk mengarahkan pengguna ke proses upload firmware.

#### `error.html`
- Halaman fallback jika halaman tidak ditemukan (`404`), atau ada error lain saat akses web server.
- Meningkatkan UX dengan memberikan informasi kesalahan yang lebih jelas.

---

### 🔧 Catatan Tambahan

- Semua file HTML dan CSS ini akan disajikan langsung dari ESP8266 melalui server lokal berbasis `ESP8266WebServer`.
- File HTML akan dibaca dari `PROGMEM` (memori flash) dan ditampilkan ke browser pengguna.
- Folder `sketch/` ini adalah tempat yang dibaca oleh `arduino-cli compile` saat proses build di GitHub Actions.

---

### 📦 Tips

- Pastikan semua file sudah direferensikan dengan benar di dalam `sketch.ino` menggunakan `server.on()` dan `server.send()`.
- Gunakan tool seperti `ESPAsyncWebServer` atau `ESP8266WebServer` untuk performa dan kestabilan yang lebih baik.
- Untuk menyimpan HTML/CSS ke dalam `PROGMEM`, kamu bisa menggunakan macro `F()` atau konversi ke string header (`.h`).

---

### 🔗 Referensi

- [ESP8266WebServer Library](https://arduino-esp8266.readthedocs.io/en/latest/esp8266webserver.html)
- [Arduino OTA Update](https://arduino-esp8266.readthedocs.io/en/latest/ota_updates/readme.html)
- [FS Upload Tool (SPIFFS)](https://randomnerdtutorials.com/install-esp8266-filesystem-uploader-arduino-ide/)