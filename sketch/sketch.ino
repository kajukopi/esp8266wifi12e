#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <FirebaseESP8266.h>
#include "webpage.h"

// WiFi
const char* ssid = "karimroy";
const char* password = "09871234";

// Telegram Bot
const char* botToken = "YOUR_BOT_TOKEN";
String chatId = "YOUR_CHAT_ID";

// Firebase
#define FIREBASE_HOST "payunghitam-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "NyUsJ10Dn8STiQACqtSFttiGCqvE1kHNRXg9EBin"

FirebaseData fbdo;

// Pin dan objek
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
Servo myServo;
const int servoPin = D5;
const int ledPin = LED_BUILTIN;

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long lastCheck = 0;

String getSignalStrength() {
  int rssi = WiFi.RSSI();
  if (rssi > -50) return String(rssi) + " dBm (Excellent)";
  if (rssi > -60) return String(rssi) + " dBm (Very Good)";
  if (rssi > -70) return String(rssi) + " dBm (Good)";
  if (rssi > -80) return String(rssi) + " dBm (Fair)";
  return String(rssi) + " dBm (Weak)";
}

void handleTelegramBot() {
  if (millis() - lastCheck > 10000) {
    int numNew = bot.getUpdates(bot.last_message_received + 1);
    while (numNew) {
      for (int i = 0; i < numNew; i++) {
        String text = bot.messages[i].text;

        if (text == "/led_on") {
          digitalWrite(ledPin, LOW);
          bot.sendMessage(chatId, "✅ LED dinyalakan", "");
          Firebase.setBool(fbdo, "/status/led", true);
        } else if (text == "/led_off") {
          digitalWrite(ledPin, HIGH);
          bot.sendMessage(chatId, "❌ LED dimatikan", "");
          Firebase.setBool(fbdo, "/status/led", false);
        } else if (text.startsWith("/servo_")) {
          int val = constrain(text.substring(7).toInt(), 0, 100);
          myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
          bot.sendMessage(chatId, "🎚️ Servo ke " + String(val) + "%", "");
          Firebase.setInt(fbdo, "/status/servo", val);
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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF
  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  // Firebase
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);

  // OTA
  ArduinoOTA.begin();

  // Webserver
  server.on("/", []() {
    server.send_P(200, "text/html", WEB_page);
  });

  httpUpdater.setup(&server, "/update");

  server.on("/setServo", []() {
    if (server.hasArg("percent")) {
      int val = constrain(server.arg("percent").toInt(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
      Firebase.setInt(fbdo, "/status/servo", val);
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing percent");
    }
  });

  server.on("/toggleLED", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      bool isOn = (state == "on");
      digitalWrite(ledPin, isOn ? LOW : HIGH);
      Firebase.setBool(fbdo, "/status/led", isOn);
      server.send(200, "text/plain", "LED " + state);
    } else {
      server.send(400, "text/plain", "Missing state");
    }
  });

  server.on("/status", HTTP_GET, []() {
    bool ledState = (digitalRead(ledPin) == LOW);
    int servoVal = map(myServo.readMicroseconds(), 500, 2500, 0, 100);

    String json = "{";
    json += "\"ip\":\"" + WiFi.localIP().toString() + "\",";
    json += "\"signal\":\"" + getSignalStrength() + "\",";
    json += "\"led\":" + String(ledState ? "true" : "false") + ",";
    json += "\"servo\":" + String(servoVal);
    json += "}";

    server.send(200, "application/json", json);
  });

  server.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleTelegramBot();
}