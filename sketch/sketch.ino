#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <FirebaseESP8266.h>
#include "webpage.h"  // Web UI HTML

// WiFi credentials
const char* ssid = "karimroy";
const char* password = "09871234";

// Telegram credentials
const char* botToken = "YOUR_BOT_TOKEN";
String chatId = "YOUR_CHAT_ID";

// Firebase
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// Firebase constants
const String firebasePath = "/device_status";

// Web & OTA
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

// Hardware
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

void sendToFirebase() {
  FirebaseJson json;
  json.set("ip", WiFi.localIP().toString());
  json.set("signal", getSignalStrength());
  json.set("millis", millis());

  if (Firebase.RTDB.setJSON(&fbdo, firebasePath.c_str(), &json)) {
    Serial.println("📤 Sent to Firebase");
  } else {
    Serial.print("❌ Firebase Error: ");
    Serial.println(fbdo.errorReason());
  }
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
  Serial.println("\n✅ Connected to WiFi");
  Serial.println("🆔 IP: " + WiFi.localIP().toString());

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // LED OFF default
  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  // Firebase setup
  config.api_key = "AIzaSyCjRBls8ocobTwkZdbt44TrHWLg2O42A9c";
  config.database_url = "https://payunghitam-default-rtdb.asia-southeast1.firebasedatabase.app/";
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Web UI
  server.on("/", []() {
    server.send_P(200, "text/html", WEB_page);
  });

  httpUpdater.setup(&server, "/update");

  server.on("/setServo", []() {
    if (server.hasArg("percent")) {
      int val = constrain(server.arg("percent").toInt(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
      server.send(200, "text/plain", "OK");
    } else {
      server.send(400, "text/plain", "Missing percent");
    }
  });

  server.on("/toggleLED", []() {
    if (server.hasArg("state")) {
      String state = server.arg("state");
      digitalWrite(ledPin, (state == "on") ? LOW : HIGH);
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

  static unsigned long lastSend = 0;
  if (millis() - lastSend > 10000) {
    sendToFirebase();
    lastSend = millis();
  }
}