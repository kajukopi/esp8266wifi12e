#include <ESP8266WiFi.h>

// Replace with your WiFi credentials
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Built-in LED on NodeMCU is on GPIO2 (D4)
const int ledPin = 2;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH); // LED off initially (inverted logic)

  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  digitalWrite(ledPin, LOW); // LED on
  delay(1000);
  digitalWrite(ledPin, HIGH); // LED off
  delay(1000);
}

// Edit File 4
