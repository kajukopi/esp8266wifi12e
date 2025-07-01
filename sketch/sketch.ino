#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>
#include <Servo.h>

const char* ssid = "karimroy";
const char* password = "09871234";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

Servo myServo;
const int servoPin = D5; // GPIO14
const int ledPin = D2;   // GPIO4

void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP8266 Control Panel</title>
      <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css">
      <style>
        body { padding: 2rem; }
        .slider-label { margin-top: 2rem; }
        .switch { margin-top: 2rem; }
        .btn-update { margin-top: 2rem; }
      </style>
    </head>
    <body class="container text-center">
      <h1 class="mb-4">ESP8266 Control Panel</h1>

      <div>
        <label for="slider" class="form-label slider-label">Servo Angle: <span id="angleValue">0%</span></label>
        <input type="range" class="form-range" id="slider" min="0" max="100" value="0" oninput="updateSlider(this.value)">
      </div>

      <div class="form-check form-switch switch">
        <input class="form-check-input" type="checkbox" id="ledSwitch" onchange="toggleLED(this.checked)">
        <label class="form-check-label" for="ledSwitch">LED D2</label>
      </div>

      <a href="/update" class="btn btn-primary btn-update">Firmware Update</a>

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
  )rawliteral");
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
    digitalWrite(ledPin, (state == "on") ? HIGH : LOW);
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
      <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css">
      <style>
        body { padding: 2rem; }
      </style>
    </head>
    <body class="container text-center">
      <h2>Firmware Update</h2>
      <p>Upload firmware (.bin) file here:</p>
      <form method="POST" action="/update" enctype="multipart/form-data">
        <input type="file" name="firmware" class="form-control mb-3">
        <input type="submit" value="Upload" class="btn btn-success">
      </form>
      <a href="/" class="btn btn-secondary mt-4">⬅ Kembali ke Halaman Utama</a>
    </body>
    </html>
  )rawliteral");
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected: " + WiFi.localIP().toString());

  myServo.attach(servoPin);
  myServo.writeMicroseconds(500); // awal posisi

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  server.on("/", handleRoot);
  server.on("/setServo", handleSetServo);
  server.on("/toggleLED", handleToggleLED);
  server.on("/update", HTTP_GET, handleUpdatePage);

  httpUpdater.setup(&server, "/update");

  server.begin();
  ArduinoOTA.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}