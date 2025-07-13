#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "webpage.h"

// WiFi
const char* ssid = "karimroy";
const char* password = "09871234";

// Telegram
const char* botToken = "7826449058:AAHZgPVpCdmwHsmCO6D9wIsnpZj3fOyjXWM";
String chatId = "7891041281";

// Inisialisasi
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2); // Ubah ke 0x3F jika tidak tampil

const int servoPin = D5;
const int ledPin = LED_BUILTIN;
unsigned long lastCheck = 0;
unsigned long lcdRestoreTime = 0;
unsigned long lastStatusDisplay = 0;

String logBuffer = "";
String currentIp = "";
String lastServoStatus = "Servo -";

void addLog(String msg) {
  Serial.println(msg);
  logBuffer += msg + "\n";
  if (logBuffer.length() > 1500) logBuffer = logBuffer.substring(logBuffer.length() - 1500);
}

String getSignalStrength() {
  int rssi = WiFi.RSSI();
  if (rssi > -50) return String(rssi) + " dBm (Excellent)";
  if (rssi > -60) return String(rssi) + " dBm (Very Good)";
  if (rssi > -70) return String(rssi) + " dBm (Good)";
  if (rssi > -80) return String(rssi) + " dBm (Fair)";
  return String(rssi) + " dBm (Weak)";
}

void displayIp() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(currentIp);
}

void showTempMessage(String line1, String line2 = "") {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  lcdRestoreTime = millis() + 3000;
}

void handleTelegramBot() {
  if (millis() - lastCheck > 10000) {
    int numNew = bot.getUpdates(bot.last_message_received + 1);
    while (numNew) {
      for (int i = 0; i < numNew; i++) {
        String text = bot.messages[i].text;
        if (text == "/led_on") {
          digitalWrite(ledPin, LOW);
          showTempMessage("LED On", lastServoStatus);
          bot.sendMessage(chatId, "✅ LED dinyalakan", "");
        } else if (text == "/led_off") {
          digitalWrite(ledPin, HIGH);
          showTempMessage("LED Off", lastServoStatus);
          bot.sendMessage(chatId, "❌ LED dimatikan", "");
        } else if (text.startsWith("/servo_")) {
          int val = constrain(text.substring(7).toInt(), 0, 100);
          myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
          lastServoStatus = "Servo " + String(val) + "%";
          showTempMessage(lastServoStatus);
          bot.sendMessage(chatId, "🎚️ Servo ke " + String(val) + "%", "");
        } else if (text == "/status") {
          String msg = "📡 *ESP8266 Status*\n";
          msg += "🆔 IP: `" + WiFi.localIP().toString() + "`\n";
          msg += "📶 Signal: `" + getSignalStrength() + "`";
          bot.sendMessage(chatId, msg, "Markdown");
        }
      }
      numNew = bot.getUpdates(bot.last_message_received + 1);
    }
    lastCheck = millis();
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  // lcd.noBacklight(); // Mematikan backlight
  lcd.backlight();   // Menyalakan backlight

  // Tampilan awal
  lcd.setCursor(0, 0);
  lcd.print("ESP8266");
  lcd.setCursor(0, 1);
  lcd.print("Web Server");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 1);
    lcd.print("Searching");
    for (int i = 0; i < 3; i++) {
      lcd.print(".");
      delay(500);
    }
    lcd.setCursor(9, 1);
    lcd.print("   ");
  }

  currentIp = WiFi.localIP().toString();
  addLog("✅ WiFi Connected! IP: " + currentIp);
  displayIp();

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  // Web
  server.on("/", []() {
    server.send_P(200, "text/html", WEB_page);
  });

  httpUpdater.setup(&server, "/update");

  server.on("/setServo", []() {
    if (server.hasArg("percent")) {
      int val = constrain(server.arg("percent").toInt(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
      lastServoStatus = "Servo " + String(val) + "%";
      showTempMessage(lastServoStatus);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing percent");
    }
  });

  server.on("/toggleLED", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      digitalWrite(ledPin, (state == "on") ? LOW : HIGH);
      showTempMessage("LED " + state, lastServoStatus);
      server.send(200, "text/plain", "LED " + state);
    } else {
      server.send(400, "text/plain", "Missing state");
    }
  });

  server.on("/status", HTTP_GET, []() {
    String json = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"signal\":\"" + getSignalStrength() + "\"}";
    server.send(200, "application/json", json);
  });

  server.on("/log", HTTP_GET, []() {
    server.send(200, "text/plain", logBuffer);
  });

  server.begin();

  // OTA
  ArduinoOTA.onStart([]() {
    showTempMessage("OTA Update Start");
  });

  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    lcd.setCursor(0, 1);
    lcd.print("Progress: ");
    lcd.print((progress * 100) / total);
    lcd.print("%   ");
  });

  ArduinoOTA.onEnd([]() {
    showTempMessage("OTA Update Done");
    delay(2000);
    displayIp();
  });

  ArduinoOTA.onError([](ota_error_t error) {
    showTempMessage("OTA Error:", String(error));
  });

  ArduinoOTA.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleTelegramBot();

  if (lcdRestoreTime > 0 && millis() > lcdRestoreTime) {
    lcdRestoreTime = 0;
    displayIp();
  }

  if (lcdRestoreTime == 0 && millis() - lastStatusDisplay > 10000) {
    lastStatusDisplay = millis();
    String rssiStr = getSignalStrength();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Signal Strength");
    lcd.setCursor(0, 1);
    lcd.print(rssiStr.substring(0, 16));
    lcdRestoreTime = millis() + 2000;
  }
}