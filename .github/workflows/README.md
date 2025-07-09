### 📖 Penjelasan Langkah demi Langkah

- **`name`**: Menentukan nama workflow yang muncul di tab Actions GitHub.
- **`on.push.branches`**: Workflow dijalankan otomatis saat ada push ke branch `main`.
- **`workflow_dispatch`**: Workflow bisa dijalankan secara manual melalui UI GitHub.
- **`runs-on`**: Menentukan runner GitHub, dalam hal ini menggunakan Ubuntu terbaru.
- **`actions/checkout@v4`**: Mengambil seluruh isi repository ke dalam runner agar bisa diproses.
- **`arduino/setup-arduino-cli@v1`**: Menginstal Arduino CLI ke environment runner.
- **`arduino-cli config init`**: Membuat file konfigurasi awal `arduino-cli.yaml`.
- **`arduino-cli config add board_manager.additional_urls`**: Menambahkan URL board manager untuk ESP8266.
- **`arduino-cli core update-index && core install`**: Memperbarui dan menginstal core board ESP8266.
- **`arduino-cli compile`**: Mengompilasi sketch yang ada di folder `sketch/` dengan board NodeMCU v2.
- **`echo "today=$(date +%F)"`**: Mengambil tanggal saat ini dan menyimpannya sebagai variabel `today`.
- **`mv build/sketch.ino.bin ...`**: Mengganti nama file firmware agar lebih informatif dan menyertakan tanggal.
- **`actions/upload-artifact@v4`**: Mengunggah file `.bin` ke GitHub sebagai artifact yang bisa kamu download.

---

### 📁 Output

- **File firmware hasil kompilasi**: `firmware-YYYY-MM-DD.bin`  
- **Lokasi**: Folder `build/`  
- **Cara unduh**: GitHub → Tab **Actions** → Pilih workflow terbaru → Download **artifact**

---

### 📌 Catatan Penting

- File `.ino` harus berada di dalam folder `sketch/` dan bernama `sketch.ino`
- Gunakan board **NodeMCU 1.0 (ESP-12E Module)**  
  FQBN: `esp8266:esp8266:nodemcuv2`
- Kamu bisa menyesuaikan board, nama folder, atau struktur proyek sesuai kebutuhan

---

### 🔗 Referensi

- [Arduino CLI Documentation](https://arduino.github.io/arduino-cli)
- [ESP8266 Core GitHub](https://github.com/esp8266/Arduino)
- [GitHub Actions Documentation](https://docs.github.com/en/actions)