#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ====== WIFI & TELEGRAM SETUP ======
const char* ssid = "karimroy";
const char* password = "09871234";

const char* botToken = "7826449058:AAHZgPVpCdmwHsmCO6D9wIsnpZj3fOyjXWM";
String chatId = "7891041281";

WiFiClientSecure secured_client;
UniversalTelegramBot bot(botToken, secured_client);
unsigned long lastCheck = 0;

// ====== SERVER & PIN SETUP ======
ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

Servo myServo;
const int servoPin = D5; // GPIO14
const int ledPin = LED_BUILTIN; // GPIO2 (active LOW)

// ====== UTILITY ======
String getSignalStrength() {
  int32_t rssi = WiFi.RSSI();
  String strength;
  if (rssi > -50) strength = "Excellent";
  else if (rssi > -60) strength = "Very Good";
  else if (rssi > -70) strength = "Good";
  else if (rssi > -80) strength = "Fair";
  else strength = "Weak";
  return String(rssi) + " dBm (" + strength + ")";
}

// ====== HTML HANDLERS ======
void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="en">
    <head>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <title>ESP8266 Control</title>
      <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
      <style>
        body { padding: 2rem; }
        .slider-label, .switch { margin-top: 2rem; }
      </style>
    </head>
    <body class="container text-center">
      <h1 class="mb-3">ESP8266 Control Panel</h1>
      <p><strong>IP Address:</strong> %IP%</p>
      <p><strong>WiFi Strength:</strong> %SIGNAL%</p>

      <div class="mt-4">
        <label class="form-label">Servo Angle: <span id="angleValue">0%</span></label>
        <input type="range" class="form-range" id="slider" min="0" max="100" value="0" oninput="updateSlider(this.value)">
      </div>

      <div class="form-check form-switch switch">
        <input class="form-check-input" type="checkbox" id="ledSwitch" onchange="toggleLED(this.checked)">
        <label class="form-check-label" for="ledSwitch">Built-in LED</label>
      </div>

      <a href="/update" class="btn btn-primary mt-4">Firmware Update</a>

      <script>
        function updateSlider(value) {
          document.getElementById('angleValue').innerText = value + "%";
          fetch("/setServo?percent=" + value);
        }

        function toggleLED(state) {
          fetch("/toggleLED?state=" + (state ? "on" : "off"));
        }
      </script>
    </body>
    </html>
  )rawliteral";

  html.replace("%IP%", WiFi.localIP().toString());
  html.replace("%SIGNAL%", getSignalStrength());
  server.send(200, "text/html", html);
}

void handleSetServo() {
  if (server.hasArg("percent")) {
    int percent = server.arg("percent").toInt();
    int micro = map(percent, 0, 100, 500, 2500);
    myServo.writeMicroseconds(micro);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing 'percent'");
  }
}

void handleToggleLED() {
  if (server.hasArg("state")) {
    String state = server.arg("state");
    digitalWrite(ledPin, (state == "on") ? LOW : HIGH);
    server.send(200, "text/plain", "LED " + state);
  } else {
    server.send(400, "text/plain", "Missing 'state'");
  }
}

void handleUpdatePage() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>Firmware Update</title>
      <meta name="viewport" content="width=device-width, initial-scale=1">
      <link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet">
      <style> body { padding: 2rem; } </style>
    </head>
    <body class="container text-center">
      <h2>Firmware Update</h2>
      <form method="POST" action="/update" enctype="multipart/form-data">
        <input type="file" name="firmware" class="form-control my-3">
        <input type="submit" value="Upload" class="btn btn-success">
      </form>
      <a href="/" class="btn btn-secondary mt-4">⬅ Back to Home</a>
    </body>
    </html>
  )rawliteral");
}

// ====== TELEGRAM BOT ======
void handleTelegramBot() {
  if (millis() - lastCheck > 10000) {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while (numNewMessages) {
      for (int i = 0; i < numNewMessages; i++) {
        String text = bot.messages[i].text;
        String from = bot.messages[i].from_name;

        Serial.println("Telegram | Dari: " + from + ", Pesan: " + text);

        if (text == "/led_on") {
          digitalWrite(ledPin, LOW);
          bot.sendMessage(chatId, "✅ LED dinyalakan!", "");
        } else if (text == "/led_off") {
          digitalWrite(ledPin, HIGH);
          bot.sendMessage(chatId, "❌ LED dimatikan!", "");
        } else if (text.startsWith("/servo_")) {
          int percent = text.substring(7).toInt();
          percent = constrain(percent, 0, 100);
          int micro = map(percent, 0, 100, 500, 2500);
          myServo.writeMicroseconds(micro);
          bot.sendMessage(chatId, "🔄 Servo diatur ke " + String(percent) + "%", "");
        } else if (text == "/status") {
          String msg = "📡 *Status ESP8266*\n";
          msg += "🆔 IP: `" + WiFi.localIP().toString() + "`\n";
          msg += "📶 Signal: `" + getSignalStrength() + "`";
          bot.sendMessage(chatId, msg, "Markdown");
        } else {
          bot.sendMessage(chatId, "❓ Perintah tidak dikenal", "");
        }
      }
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    lastCheck = millis();
  }
}

// ====== SETUP ======
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  myServo.attach(servoPin);
  myServo.writeMicroseconds(500);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED off

  server.on("/", handleRoot);
  server.on("/setServo", handleSetServo);
  server.on("/toggleLED", handleToggleLED);
  server.on("/update", HTTP_GET, handleUpdatePage);

  httpUpdater.setup(&server, "/update");

  secured_client.setInsecure();
  server.begin();
  ArduinoOTA.begin();
}

// ====== LOOP ======
void loop() {
  server.handleClient();
  ArduinoOTA.handle();
  handleTelegramBot();
}