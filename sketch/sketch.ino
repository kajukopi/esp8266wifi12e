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

const char* ssid = "karimroy";
const char* password = "09871234";

const char* botToken = "7826449058:AAHZgPVpCdmwHsmCO6D9wIsnpZj3fOyjXWM";
String chatId = "7891041281";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);

const int servoPin = D5;
const int ledPin = LED_BUILTIN;
const int relayPin = D6;

unsigned long lastCheck = 0;
unsigned long lcdRestoreTime = 0;
unsigned long lastStatusDisplay = 0;

String logBuffer = "";
String currentIp = "";
String lastServoStatus = "Servo -";

void addLog(String msg) {
  Serial.println(msg);
  logBuffer += msg + "\n";
  if (logBuffer.length() > 1500)
    logBuffer = logBuffer.substring(logBuffer.length() - 1500);
}

String getSignalStrength() {
  int rssi = WiFi.RSSI();
  String bar = "";

  char block = (char)255; // karakter blok penuh

  if (rssi > -50)       bar = String(block) + block + block + block + block;
  else if (rssi > -60)  bar = String(block) + block + block + block;
  else if (rssi > -70)  bar = String(block) + block + block;
  else if (rssi > -80)  bar = String(block) + block;
  else if (rssi > -90)  bar = String(block);
  else                  bar = "-----"; // tidak ada sinyal

  return String(rssi) + " dBm (" + bar + ")";
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
  if (millis() - lastCheck > 5000) {
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
        } else if (text == "/relay_on") {
          digitalWrite(relayPin, HIGH);
          showTempMessage("Relay On");
          bot.sendMessage(chatId, "✅ Relay dinyalakan", "");
        } else if (text == "/relay_off") {
          digitalWrite(relayPin, LOW);
          showTempMessage("Relay Off");
          bot.sendMessage(chatId, "❌ Relay dimatikan", "");
        } else if (text.startsWith("/servo_")) {
          int val = constrain(text.substring(7).toInt(), 0, 100);
          myServo.writeMicroseconds(map(val, 0, 100, 544, 2400));
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
  lcd.backlight();
  lcd.setCursor(0, 0); lcd.print("ESP8266");
  lcd.setCursor(0, 1); lcd.print("Web Server");
  delay(2000);

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Connecting WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    lcd.setCursor(0, 1); lcd.print("Searching...");
    delay(1000);
  }

  currentIp = WiFi.localIP().toString();
  addLog("✅ WiFi Connected! IP: " + currentIp);
  displayIp();

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT); digitalWrite(ledPin, HIGH);
  pinMode(relayPin, OUTPUT); digitalWrite(relayPin, LOW);

  myServo.attach(servoPin);
  myServo.writeMicroseconds(544);  // Paling kiri (0°)

  server.on("/", []() {
    server.send_P(200, "text/html", WEB_page);
  });

  server.on("/setServo", []() {
    if (server.hasArg("percent")) {
      int val = constrain(server.arg("percent").toInt(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2400));
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

  server.on("/toggleRelay", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      digitalWrite(relayPin, (state == "on") ? HIGH : LOW);
      showTempMessage("Relay " + state);
      server.send(200, "text/plain", "Relay " + state);
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

  httpUpdater.setup(&server, "/update");
  server.begin();

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