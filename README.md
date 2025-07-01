# 🚀 ESP8266 Web OTA + Servo + LED Control

This project is a complete documentation of building a responsive web-based control system for a Lolin NodeMCU V1.0 (ESP8266 12E), featuring OTA (Over-the-Air) firmware updates and browser-based device control including a servo and built-in LED.

---

## 📌 Project Objectives

- Automate firmware `.bin` upload to ESP8266 using GitHub Actions.
- Enable OTA firmware updates through a browser interface.
- Control a servo motor using a web slider.
- Toggle the onboard LED from the web interface.
- Display the ESP8266’s IP address and WiFi signal strength.
- Build a responsive and mobile-friendly web interface using Bootstrap.

---

## 🛠️ Development Process

### 1. Project Initialization
The project started with a basic structure containing the main Arduino sketch file (`sketch.ino`) and a GitHub Actions workflow (`compile.yml`) to automate builds.

### 2. GitHub Actions Setup
Configured `arduino-cli` in GitHub Actions to:
- Add the ESP8266 board manager URL.
- Install the ESP8266 core.
- Compile the Arduino sketch.
- Upload the resulting `.bin` file as an artifact.

### 3. Automated Firmware Naming
The firmware file was renamed dynamically to include the build date, making it easier to track and manage.

### 4. Basic Web Server & OTA Integration
The ESP8266 was configured to host a web server that allows OTA updates using `ESP8266HTTPUpdateServer` and `ArduinoOTA`.

### 5. Adding Servo Control
A servo was connected to pin D5 and controlled via an HTML range slider from the web interface.

### 6. Improving Servo Range
Initially, the servo only rotated 90°. The control logic was adjusted to expand the range up to ~170° with better responsiveness.

### 7. Web Interface Enhancement
The user interface was improved using Bootstrap, making it clean, responsive, and easy to use, including navigation buttons and spacing.

### 8. LED Toggle Feature
A toggle switch was added to control the built-in LED via the web interface.

### 9. Network Info Display
The device’s IP address and WiFi signal strength were added to the web UI for user reference.

### 10. Mobile-Friendly Design
The entire web interface was optimized for mobile browsers, ensuring smooth usage on smartphones and tablets.

---

## 📦 Features

- Build and download firmware `.bin` using GitHub Actions
- Perform OTA firmware updates via web browser
- Control a servo motor with a web slider
- Toggle the built-in LED through the browser
- View IP address and WiFi signal strength
- Responsive UI that works on mobile and desktop

---

## 🔮 Future Improvements (Optional)

- Save the last servo position in EEPROM
- Automatically reboot after successful OTA update
- Add authentication/login for security
- Implement dark mode theme
- Add activity logs and monitoring

---

## 👤 Author

This project was created by **Karim Roy Tampubolon**, in collaboration with ChatGPT – OpenAI.

---

## 🗓️ Timeline Summary

- **July 1, 2025**: Initial setup with GitHub Actions and OTA functionality
- **July 1–2, 2025**: Added servo, LED control, and responsive web UI
- **July 3, 2025**: Final polish and documentation

---

Thanks for reading and exploring this project.  
Feel free to use it as a learning reference or a base for your own IoT development with ESP8266.