#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <WebSocketsServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "index.h"
#include "update.h"

const char* ssid = "karimroy";
const char* password = "09871234";
const char* botToken = "YOUR_BOT_TOKEN"; // ganti
String chatId = "YOUR_CHAT_ID";          // ganti

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long lastCheck = 0;

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;
WebSocketsServer webSocket(81);

Servo myServo;
const int servoPin = D5;
const int ledPin = LED_BUILTIN;

String getSignalStrength() {
  int rssi = WiFi.RSSI();
  if (rssi > -50) return String(rssi) + " dBm (Excellent)";
  if (rssi > -60) return String(rssi) + " dBm (Very Good)";
  if (rssi > -70) return String(rssi) + " dBm (Good)";
  if (rssi > -80) return String(rssi) + " dBm (Fair)";
  return String(rssi) + " dBm (Weak)";
}

void webSocketEvent(uint8_t client, WStype_t type, uint8_t *payload, size_t length) {
  if (type == WStype_TEXT) {
    String msg = String((char*)payload);
    DynamicJsonDocument doc(256);
    if (deserializeJson(doc, msg)) return;

    if (doc.containsKey("servo")) {
      int val = constrain(doc["servo"].as<int>(), 0, 100);
      myServo.writeMicroseconds(map(val, 0, 100, 500, 2500));
    }

    if (doc.containsKey("led")) {
      digitalWrite(ledPin, (doc["led"] == "on") ? LOW : HIGH);
    }
  }
}

void handleRoot() {
  server.send(200, "text/html", MAIN_page);
}

void handleUpdatePage() {
  server.send(200, "text/html", UPDATE_page);
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
          int v = constrain(text.substring(7).toInt(), 0, 100);
          myServo.writeMicroseconds(map(v, 0, 100, 500, 2500));
          bot.sendMessage(chatId, "🎚️ Servo ke " + String(v) + "%", "");
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
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println("\nConnected: " + WiFi.localIP().toString());

  secured_client.setInsecure();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  server.on("/", handleRoot);
  server.on("/update", HTTP_GET, handleUpdatePage);
  httpUpdater.setup(&server, "/update");

  server.begin();
  ArduinoOTA.begin();
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop() {
  server.handleClient();
  webSocket.loop();
  ArduinoOTA.handle();
  handleTelegramBot();

  static unsigned long last = 0;
  if (millis() - last > 5000) {
    String json = "{\"ip\":\"" + WiFi.localIP().toString() + "\",\"signal\":\"" + getSignalStrength() + "\"}";
    webSocket.broadcastTXT(json);
    last = millis();
  }
}