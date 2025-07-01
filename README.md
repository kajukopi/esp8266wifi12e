```markdown
# 📘 ESP8266 Web Server OTA + Servo Control - Changelog

Proyek ini berkembang secara bertahap dari nol melalui interaksi dengan ChatGPT, dimulai dari setup GitHub Actions hingga tercipta sistem OTA + kontrol servo dan LED berbasis browser.

---

## 🟢 Tahap Awal
**Tujuan:**  
Upload file `.bin` ke Lolin NodeMCU v1.0 (ESP8266 12E) via GitHub Actions + OTA

**Struktur proyek awal:**
```
.
├── sketch/
│   └── sketch.ino
└── .github/
    └── workflows/
        └── compile.yml
```

---

## 🛠️ Tahap 1 – GitHub Actions Setup

### ✅ Setup Arduino CLI dan ESP8266 Core
```yaml
- name: Setup Arduino CLI
  run: |
    arduino-cli core update-index
    arduino-cli core install esp8266:esp8266
```

### ✅ Compile + Upload Artifact
```yaml
- name: Compile sketch
  run: |
    arduino-cli compile --fqbn esp8266:esp8266:nodemcuv2 sketch --output-dir build

- name: Upload artifact
  uses: actions/upload-artifact@v4
  with:
    name: firmware
    path: build/sketch.ino.bin
```

---

## 📅 Tahap 2 – Rename Firmware Otomatis dengan Tanggal

### ✅ Menambahkan langkah tanggal dan rename nama file:
```yaml
- name: Get date
  id: date
  run: echo "today=$(date +%F)" >> "$GITHUB_OUTPUT"

- name: Rename file
  run: mv build/sketch.ino.bin build/firmware-${{ steps.date.outputs.today }}.bin
```

---

## 🌐 Tahap 3 – Web Server + OTA Dasar

### ✅ Tambahkan OTA + HTTP Update Server:
```cpp
ESP8266HTTPUpdateServer httpUpdater;
ArduinoOTA.begin();
httpUpdater.setup(&server);
```

---

## ⚙️ Tahap 4 – Kontrol Servo via Browser Slider

### ✅ Tambahkan Servo pada Pin D5:
```cpp
Servo myServo;
myServo.attach(D5);

void handleSetServo() {
  int percent = server.arg("percent").toInt();
  int angle = map(percent, 0, 100, 0, 170);
  myServo.write(angle);
}
```

### ✅ Web UI:
```html
<input type="range" id="slider" min="0" max="100" oninput="updateSlider(this.value)">
```

---

## 🔧 Tahap 5 – Servo Hanya Bergerak 90°

### ✅ Solusi: Gunakan `writeMicroseconds()`:
```cpp
int micro = map(percent, 0, 100, 500, 2500);
myServo.writeMicroseconds(micro);
```

---

## 🎨 Tahap 6 – UI Lebih Rapi dengan Bootstrap

### ✅ Integrasi Bootstrap + Tombol OTA:
```html
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
<a href="/update" class="btn btn-primary">Firmware Update</a>
```

---

## 💡 Tahap 7 – Toggle LED

### ✅ Tambahkan kontrol LED:
```cpp
const int ledPin = LED_BUILTIN;
digitalWrite(ledPin, (state == "on") ? LOW : HIGH); // Active LOW
```

### ✅ Web UI:
```html
<input type="checkbox" onchange="toggleLED(this.checked)">
```

---

## 📱 Tahap 8 – Mobile Responsive + Info IP/WiFi

### ✅ Tampilkan IP & Sinyal:
```cpp
<p><strong>IP Address:</strong> %IP%</p>
<p><strong>WiFi Strength:</strong> %SIGNAL%</p>

String getSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  return String(rssi) + " dBm (Good/Excellent/etc)";
}
```

---

## ✅ Final Code Fitur Lengkap

| Fitur                  | Status |
|------------------------|--------|
| OTA Update via Web     | ✅     |
| Servo Slider Control   | ✅     |
| LED Toggle Switch      | ✅     |
| IP Address Display     | ✅     |
| WiFi Signal Display    | ✅     |
| Responsive UI (Mobile) | ✅     |

---

## 🔮 Rencana Selanjutnya (Opsional)

- 💾 Simpan posisi terakhir servo (EEPROM)
- 🔁 Reboot otomatis setelah OTA update
- 📝 Logging aktivitas (servo & LED)
- 🌐 Scan & pilih WiFi via Web
- 🌙 Dark mode UI toggle

---

🛠️ Dibuat bersama ChatGPT – OpenAI  
📆 Progress: Dari 1 Juli 2025 sampai tuntas
```