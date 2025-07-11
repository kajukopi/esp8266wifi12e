#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "index.h"
#include "update.h"

// WIFI & TELEGRAM
const char* ssid = "karimroy";
const char* password = "09871234";
const char* botToken = "YOUR_BOT_TOKEN";
String chatId = "YOUR_CHAT_ID";

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long lastCheck = 0;

// SERVER
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WebSocketsServer webSocket = WebSocketsServer(81);

Servo myServo;
const int servoPin = D5;
const int ledPin = LED_BUILTIN;

String getSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  if (rssi > -50) return String(rssi) + " dBm (Excellent)";
  if (rssi > -60) return String(rssi) + " dBm (Very Good)";
  if (rssi > -70) return String(rssi) + " dBm (Good)";
  if (rssi > -80) return String(rssi) + " dBm (Fair)";
  return String(rssi) + " dBm (Weak)";
}

// HANDLERS
void handleRoot() {
  String html = MAIN_page;
  html.replace("%IP%", WiFi.localIP().toString());
  server.send(200, "text/html", html);
}

void handleUpdatePage() {
  server.send(200, "text/html", UPDATE_page);
}

void handleTelegramBot() {
  if (millis() - lastCheck > 10000) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String text = bot.messages[i].text;
        if (text == "/led_on") {
          digitalWrite(ledPin, LOW);
          bot.sendMessage(chatId, "✅ LED dinyalakan!", "");
        } else if (text == "/led_off") {
          digitalWrite(ledPin, HIGH);
          bot.sendMessage(chatId, "❌ LED dimatikan!", "");
        } else if (text.startsWith("/servo_")) {
          int percent = constrain(text.substring(7).toInt(), 0, 100);
          int micro = map(percent, 0, 100, 500, 2500);
          myServo.writeMicroseconds(micro);
          bot.sendMessage(chatId, "🔄 Servo ke " + String(percent) + "%", "");
        } else if (text == "/status") {
          String msg = "📡 *ESP Status*\n";
          msg += "🆔 IP: `" + WiFi.localIP().toString() + "`\n";
          msg += "📶 Signal: `" + getSignalStrength() + "`";
          bot.sendMessage(chatId, msg, "Markdown");
        } else {
          bot.sendMessage(chatId, "❓ Tidak dikenal", "");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastCheck = millis();
  }
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t len) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    if (msg.startsWith("{")) {
      DynamicJsonDocument doc(256);
      deserializeJson(doc, msg);
      if (doc.containsKey("servo")) {
        int val = constrain(doc["servo"].as<int>(), 0, 100);
        myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
      }
      if (doc.containsKey("led")) {
        String state = doc["led"];
        digitalWrite(ledPin, (state == "on") ? LOW : HIGH);
      }
    }
  }
}

// SETUP
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nWiFi connected: " + WiFi.localIP().toString());

  secured_client.setInsecure();

  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  server.on("/", handleRoot);
  server.on("/update", HTTP_GET, handleUpdatePage);
  httpUpdater.setup(&server, "/update");
  server.begin();

  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  ArduinoOTA.begin();
}

// LOOP
void loop() {
  server.handleClient();
  webSocket.loop();
  ArduinoOTA.handle();
  handleTelegramBot();

  static unsigned long lastPush = 0;
  if (millis() - lastPush > 3000) {
    String msg = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"signal\":\"" + getSignalStrength() + "\"}";
    webSocket.broadcastTXT(msg);
    lastPush = millis();
  }
}