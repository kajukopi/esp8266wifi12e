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

void handleRoot() {
  server.send(200, "text/html", R"rawliteral(
    <!DOCTYPE html>
    <html>
    <head>
      <title>ESP8266 Servo Control</title>
      <style>
        body { font-family: sans-serif; text-align: center; padding: 2em; }
        input[type=range] { width: 80%; }
      </style>
    </head>
    <body>
      <h1>ESP8266 Servo Control</h1>
      <label for="slider">Angle: <span id="angleValue">0%</span></label><br>
      <input type="range" id="slider" min="0" max="100" value="0" oninput="updateSlider(this.value)">
      <script>
        function updateSlider(value) {
          document.getElementById('angleValue').innerText = value + "%";
          fetch("/setServo?percent=" + value);
        }
      </script>
    </body>
    </html>
  )rawliteral");
}

void handleSetServo() {
  if (server.hasArg("percent")) {
    int percent = server.arg("percent").toInt();
    int angle = map(percent, 0, 100, 0, 170); // 0–100% → 0–170°
    myServo.write(angle);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Missing 'percent' parameter");
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); Serial.print(".");
  }
  Serial.println("\nConnected! IP: " + WiFi.localIP().toString());

  myServo.attach(servoPin); // D5 = GPIO14
  myServo.write(0);

  httpUpdater.setup(&server);

  server.on("/", handleRoot);
  server.on("/setServo", handleSetServo);

  server.begin();
  ArduinoOTA.begin();
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}