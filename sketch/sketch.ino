#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <ArduinoOTA.h>

const char* ssid = "karimroy";
const char* password = "09871234";

ESP8266WebServer server(80);
ESP8266HTTPUpdateServer httpUpdater;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  httpUpdater.setup(&server);
  server.on("/", HTTP_GET, []() {
    server.send(200, "text/html", R"rawliteral(
      <h1>ESP8266 OTA Web Updater</h1>
      <p><a href="/update">Update Firmware</a></p>
    )rawliteral");
  });

  server.begin();
  ArduinoOTA.begin(); // Optional OTA via Arduino IDE
}

void loop() {
  server.handleClient();
  ArduinoOTA.handle();
}

// Edit 4