/*
 * LittleFSExplorer - WebServer Example
 * 
 * This example demonstrates how to use LittleFSExplorer alongside other
 * web server endpoints. The same server hosts both the file explorer
 * and custom API endpoints.
 * 
 * Features:
 * - LittleFSExplorer file explorer (/fs)
 * - Temperature sensor endpoint (/api/temperature)
 * - LED control endpoint (/api/led)
 * - System status endpoint (/api/status)
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

const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#ifdef ESP8266
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#endif

LittleFSExplorer explorer;

// LED pin
#ifdef ESP8266
  #define LED_PIN 2
#elif defined(ESP32)
  #define LED_PIN 2
#endif

bool ledState = false;

// Temperature reading (example)
float readTemperature() {
  // Replace with your actual sensor reading
  // Example: return analogRead(A0) * 0.1;
  return 22.5 + random(0, 100) / 10.0;
}

// Main page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>ESP Web Server</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;margin:40px;background:#f5f5f5;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);margin-bottom:20px;}";
  html += "h1{color:#333;}";
  html += ".btn{padding:10px 20px;border:none;border-radius:5px;cursor:pointer;font-size:16px;}";
  html += ".btn-primary{background:#e94560;color:white;}";
  html += ".btn-primary:hover{background:#d63851;}";
  html += ".status{font-size:24px;font-weight:bold;color:#4ecdc4;}";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h1>ESP Web Server</h1>";
  html += "<p>LittleFSExplorer example</p>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>Dashboard</h2>";
  html += "<p>Temperature: <span class='status' id='temp'>--</span> &deg;C</p>";
  html += "<p>LED: <span class='status' id='led'>--</span></p>";
  html += "<button class='btn btn-primary' onclick='toggleLed()'>Toggle LED</button>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>File Explorer</h2>";
  html += "<p><a href='/fs'>LittleFS Explorer</a> - Click to manage files</p>";
  html += "</div>";
  html += "<script>";
  html += "function getData(){";
  html += "  fetch('/api/status').then(r=>r.json()).then(d=>{";
  html += "    document.getElementById('temp').textContent=d.temperature.toFixed(1);";
  html += "    document.getElementById('led').textContent=d.led?'On':'Off';";
  html += "  });";
  html += "}";
  html += "function toggleLed(){";
  html += "  fetch('/api/led').then(()=>getData());";
  html += "}";
  html += "setInterval(getData,2000);getData();";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

// API: Temperature
void handleTemperature() {
  float temp = readTemperature();
  String json = "{\"temperature\":" + String(temp, 1) + "}";
  server.send(200, "application/json", json);
}

// API: LED control
void handleLed() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState ? HIGH : LOW);
  
  // Save last state to LittleFS
  File file = LittleFS.open("/led_state.txt", "w");
  if (file) {
    file.print(ledState ? "1" : "0");
    file.close();
  }
  
  String json = "{\"led\":" + String(ledState ? "true" : "false") + "}";
  server.send(200, "application/json", json);
}

// API: System status
void handleStatus() {
  FSInfo fs_info;
  LittleFS.info(fs_info);
  
  String json = "{";
  json += "\"temperature\":" + String(readTemperature(), 1) + ",";
  json += "\"led\":" + String(ledState ? "true" : "false") + ",";
  json += "\"uptime\":" + String(millis() / 1000) + ",";
  json += "\"freeHeap\":" + String(ESP.getFreeHeap()) + ",";
  json += "\"fsUsed\":" + String(fs_info.usedBytes) + ",";
  json += "\"fsTotal\":" + String(fs_info.totalBytes);
  json += "}";
  
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting LittleFSExplorer WebServer Example...");
  
  // LED setup
  pinMode(LED_PIN, OUTPUT);
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS!");
    return;
  }
  Serial.println("LittleFS initialized");
  
  // Load last LED state
  File file = LittleFS.open("/led_state.txt", "r");
  if (file) {
    ledState = file.readString().toInt() == 1;
    digitalWrite(LED_PIN, ledState ? HIGH : LOW);
    file.close();
  }
  
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
  
  // Define endpoints
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/temperature", HTTP_GET, handleTemperature);
  server.on("/api/led", HTTP_GET, handleLed);
  server.on("/api/status", HTTP_GET, handleStatus);
  
  // Initialize LittleFSExplorer
  explorer.begin(&server);
  Serial.println("LittleFSExplorer initialized");
  
  // Start web server
  server.begin();
  Serial.println("HTTP server started");
  Serial.println("Navigate to http://" + WiFi.localIP().toString() + "/ in your browser");
}

void loop() {
  server.handleClient();
}
