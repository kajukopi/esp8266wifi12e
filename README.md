# ðŸ“˜ ESP8266 Web Server OTA + Servo Control - Changelog

Proyek ini berkembang secara bertahap dari nol melalui interaksi dengan ChatGPT, dimulai dari setup GitHub Actions hingga tercipta sistem OTA + kontrol servo dan LED berbasis browser.

---

## ðŸŸ¢ Tahap Awal
**Tujuan:**  
Upload file `.bin` ke Lolin NodeMCU v1.0 (ESP8266 12E) via GitHub Actions + OTA

**Struktur proyek awal:**
```
.
â”œâ”€â”€ sketch/
â”‚   â””â”€â”€ sketch.ino
â””â”€â”€ .github/
    â””â”€â”€ workflows/
        â””â”€â”€ compile.yml
```

---

## ðŸ› ï¸ Tahap 1 â€“ GitHub Actions Setup

### âœ… Setup Arduino CLI
```yaml
- name: Install ESP8266 Core
  run: |
    arduino-cli core update-index
    arduino-cli core install esp8266:esp8266
```

### âœ… Compile + Upload Artifact
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

## ðŸ“… Tahap 2 â€“ Rename Firmware Otomatis dengan Tanggal

### âœ… Tambahkan tanggal ke nama file:
```yaml
- name: Get date
  id: date
  run: echo "today=$(date +%F)" >> "$GITHUB_OUTPUT"

- name: Rename file
  run: mv build/sketch.ino.bin build/firmware-${{ steps.date.outputs.today }}.bin
```

---

## ðŸŒ Tahap 3 â€“ Web Server + OTA Dasar

### âœ… Tambahkan OTA + HTTP Update Server:
```cpp
ESP8266HTTPUpdateServer httpUpdater;
ArduinoOTA.begin();
httpUpdater.setup(&server);
```

---

## âš™ï¸ Tahap 4 â€“ Kontrol Servo via Browser Slider

### âœ… Tambahkan Servo pada Pin D5:
```cpp
Servo myServo;
myServo.attach(D5);

void handleSetServo() {
  int percent = server.arg("percent").toInt();
  int angle = map(percent, 0, 100, 0, 170);
  myServo.write(angle);
}
```

### âœ… Web UI:
```html
<input type="range" id="slider" min="0" max="100" oninput="updateSlider(this.value)">
```

---

## ðŸ”§ Tahap 5 â€“ Servo Hanya Bergerak 90Â°

### âœ… Solusi: Gunakan `writeMicroseconds()`:
```cpp
int micro = map(percent, 0, 100, 500, 2500);
myServo.writeMicroseconds(micro);
```

---

## ðŸŽ¨ Tahap 6 â€“ UI Lebih Rapi dengan Bootstrap

### âœ… Integrasi Bootstrap + Tombol OTA:
```html
<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
<a href="/update" class="btn btn-primary">Firmware Update</a>
```

---

## ðŸ’¡ Tahap 7 â€“ Toggle LED

### âœ… Tambahkan kontrol LED:
```cpp
const int ledPin = LED_BUILTIN;
digitalWrite(ledPin, (state == "on") ? LOW : HIGH); // Active LOW
```

### âœ… Web UI:
```html
<input type="checkbox" onchange="toggleLED(this.checked)">
```

---

## ðŸ“± Tahap 8 â€“ Mobile Responsive + Info IP/WiFi

### âœ… Tampilkan IP & Sinyal:
```cpp
<p><strong>IP Address:</strong> %IP%</p>
<p><strong>WiFi Strength:</strong> %SIGNAL%</p>

String getSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  return String(rssi) + " dBm (Good/Excellent/etc)";
}
```

---

## âœ… Final Code Fitur Lengkap

| Fitur                  | Status |
|------------------------|--------|
| OTA Update via Web     | âœ…     |
| Servo Slider Control   | âœ…     |
| LED Toggle Switch      | âœ…     |
| IP Address Display     | âœ…     |
| WiFi Signal Display    | âœ…     |
| Responsive UI (Mobile) | âœ…     |

---

## ðŸ”® Rencana Selanjutnya (Opsional)

- ðŸ’¾ Simpan posisi terakhir servo (EEPROM)
- ðŸ” Reboot otomatis setelah OTA update
- ðŸ“ Logging aktivitas (servo & LED)
- ðŸŒ Scan & pilih WiFi via Web
- ðŸŒ™ Dark mode UI toggle

---

ðŸ› ï¸ Dibuat bersama ChatGPT â€“ OpenAI  
ðŸ“† Progress: Dari 1 Juli 2025 sampai tuntas  