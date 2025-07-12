#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <FirebaseESP8266.h>
#include <UniversalTelegramBot.h>
#include "webpage.h"  // Halaman kontrol dan OTA

// WiFi
const char* ssid = "karimroy";
const char* password = "09871234";

// Telegram
const char* botToken = "YOUR_TELEGRAM_BOT_TOKEN";
String chatId = "YOUR_CHAT_ID";

// Firebase Realtime DB
#define FIREBASE_HOST "payunghitam-default-rtdb.asia-southeast1.firebasedatabase.app"
#define FIREBASE_AUTH "NyUsJ10Dn8STiQACqtSFttiGCqvE1kHNRXg9EBin"
FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
String firebasePath = "/device_status";

// ESP
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
Servo myServo;

const int servoPin = D5;
const int ledPin = LED_BUILTIN;
unsigned long lastCheck = 0;

// Ambil sinyal WiFi dalam teks
String getSignalStrength() {
  int rssi = WiFi.RSSI();
  if (rssi > -50) return String(rssi) + " dBm (Excellent)";
  if (rssi > -60) return String(rssi) + " dBm (Very Good)";
  if (rssi > -70) return String(rssi) + " dBm (Good)";
  if (rssi > -80) return String(rssi) + " dBm (Fair)";
  return String(rssi) + " dBm (Weak)";
}

// Kirim data ke Firebase
void sendToFirebase() {
  FirebaseJson json;
  json.set("ip", WiFi.localIP().toString());
  json.set("signal", getSignalStrength());
  json.set("millis", millis());

  if (Firebase.setJSON(fbdo, firebasePath.c_str(), json)) {
    Serial.println("📤 Sent to Firebase");
  } else {
    Serial.print("❌ Firebase Error: ");
    Serial.println(fbdo.errorReason());
  }
}

// Handle Telegram Command
void handleTelegramBot() {
  if (millis() - lastCheck > 10000) {
    int numNew = bot.getUpdates(bot.last_message_received + 1);
    while (numNew) {
      for (int i = 0; i < numNew; i++) {
        String text = bot.messages[i].text;

        if (text == "/led_on") {
          digitalWrite(ledPin, LOW);
          bot.sendMessage(chatId, "✅ LED dinyalakan", "");
        } else if (text == "/led_off") {
          digitalWrite(ledPin, HIGH);
          bot.sendMessage(chatId, "❌ LED dimatikan", "");
        } else if (text.startsWith("/servo_")) {
          int val = constrain(text.substring(7).toInt(), 0, 100);
          myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
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
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected! IP: " + WiFi.localIP().toString());

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF default
  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  // Firebase Init
  config.host = FIREBASE_HOST;
  config.api_key = FIREBASE_AUTH;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // OTA & Web Server
  server.on("/", []() {
    server.send_P(200, "text/html", WEB_page);
  });

  httpUpdater.setup(&server, "/update");

  server.on("/setServo", []() {
    if (server.hasArg("percent")) {
      int val = constrain(server.arg("percent").toInt(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
      sendToFirebase();  // Update Firebase setelah perubahan
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing percent");
    }
  });

  server.on("/toggleLED", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      digitalWrite(ledPin, (state == "on") ? LOW : HIGH);
      sendToFirebase();  // Update Firebase setelah perubahan
      server.send(200, "text/plain", "LED " + state);
    } else {
      server.send(400, "text/plain", "Missing state");
    }
  });

  server.on("/status", HTTP_GET, []() {
    String json = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"signal\":\"" + getSignalStrength() + "\"}";
    server.send(200, "application/json", json);
  });

  server.begin();
  ArduinoOTA.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleTelegramBot();
}