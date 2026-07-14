/*
 * LittleFSExplorer - Basic Example
 * 
 * Basic example for ESP8266/ESP32 LittleFS file explorer.
 * Navigate to http://<ESP_IP>/fs in your browser to access the file explorer.
 * 
 * Setup:
 * 1. Select ESP8266 or ESP32 board in Arduino IDE
 * 2. Upload LittleFS using LittleFSUploader
 * 3. Compile and upload this code
 * 4. Find the IP address in Serial Monitor
 */

#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WiFi.h>
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <WebServer.h>
#endif

#include <LittleFS.h>
#include <LittleFSExplorer.h>

// WiFi credentials - change these to your own
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#ifdef ESP8266
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#endif

LittleFSExplorer explorer;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starting LittleFS Explorer...");

  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS!");
    Serial.println("Formatting LittleFS...");
    LittleFS.format();
    LittleFS.begin();
  }
  Serial.println("LittleFS initialized");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize LittleFSExplorer
  explorer.begin(&server);
  Serial.println("LittleFS Explorer initialized");
  Serial.println("Navigate to http://" + WiFi.localIP().toString() + "/fs in your browser");

  // Start web server
  server.begin();
  Serial.println("HTTP server started");
}

void loop() {
  server.handleClient();
}
