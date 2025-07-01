# 🚀 ESP8266 Web OTA + Servo + LED Control

This project documents the development of a responsive web-based control system for the Lolin NodeMCU V1.0 (ESP8266 12E), featuring OTA (Over-the-Air) firmware updates and device control via a browser. The system allows control of a servo motor and built-in LED through an intuitive web interface.

---

## 📌 Project Objectives

- Automate firmware `.bin` upload to ESP8266 using GitHub Actions.
- Enable OTA firmware updates through a web interface.
- Control a servo motor using an HTML slider.
- Toggle the onboard LED from a browser switch.
- Display IP address and WiFi signal strength.
- Build a clean, responsive, and mobile-friendly interface using Bootstrap.

---

## 🛠️ Development Process

### 1. Project Initialization
Set up project structure:
- `sketch.ino` for Arduino code.
- GitHub Actions workflow (`compile.yml`) for automation.

### 2. GitHub Actions Setup
Configured GitHub CI/CD to:
- Install `arduino-cli`.
- Add ESP8266 board index.
- Compile code and export `.bin`.
- Upload firmware as a downloadable artifact.

### 3. Auto-Naming Firmware with Date
Each firmware build is renamed with the current date to make it easier to manage multiple versions.

### 4. Basic Web Server & OTA
Enabled OTA firmware updates via `ESP8266HTTPUpdateServer` and `ArduinoOTA`, served over a local web server.

### 5. Servo Control Implementation
Connected a servo motor to D5 and enabled control through a web-based slider.

### 6. Servo Range Fix
Improved servo movement from limited 90° to a responsive ~170° range by adjusting control logic.

### 7. UI Enhancements
Used Bootstrap to create a clean, modern, and responsive layout including buttons and spacing for better user experience.

### 8. LED Toggle Feature
Added a toggle switch in the web UI to control the ESP8266's built-in LED (active LOW).

### 9. Network Info Display
Displayed ESP8266's IP address and WiFi signal strength on the web UI.

### 10. Mobile Browser Optimization
Optimized the interface layout for mobile browsers, making it user-friendly on small screens.

---

## 📱 Apps Used

Here’s a list of Android applications (APKs) used during development and testing:

| Application      | Purpose                                 |
|------------------|------------------------------------------|
| **ESP8266 Loader** | Upload `.bin` firmware to ESP8266 via OTG/serial |
| **GitHub (App)**  | Manage repository, trigger actions      |
| **Google Chrome** | Access web interface & OTA update page  |
| **Notepad**       | Quick edits and notes during coding     |

---

## 📦 Features

- ✅ Firmware compilation via GitHub Actions
- ✅ OTA firmware update via browser
- ✅ Servo control with web slider
- ✅ Built-in LED toggle with switch
- ✅ Display IP address and WiFi RSSI
- ✅ Fully responsive interface with Bootstrap

---

## 🔮 Future Enhancements

- Save last known servo position in EEPROM
- Auto-reboot after OTA update
- Add password protection for OTA page
- Add dark mode support
- Log servo & LED activity to memory or server

---

## 👤 Author

Developed by **Karim Roy Tampubolon**  

---

## 🗓️ Timeline Summary

- **July 1, 2025**: Setup GitHub Actions, OTA server, initial compile
- **July 1–2, 2025**: Implemented servo, LED toggle, UI, mobile view
- **July 3, 2025**: Final polish, network info, changelog, documentation

---

Thanks for exploring this project!  
Feel free to use or modify it for your own IoT learning and development.