/*
 * LittleFSExplorer - File Operations Example
 * 
 * This example demonstrates file operations with LittleFSExplorer.
 * 
 * Features:
 * - Create and delete files
 * - Create directories
 * - Read and write files
 * - Rename files
 * - List files
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

// Create test files
void createTestFiles() {
  Serial.println("Creating test files...");
  
  // Create config.txt
  File file = LittleFS.open("/config.txt", "w");
  if (file) {
    file.println("SSID=MyWiFi");
    file.println("Password=12345678");
    file.println("ServerIP=192.168.1.100");
    file.println("Port=8080");
    file.close();
    Serial.println("  /config.txt created");
  }
  
  // Create data/sensor.txt
  LittleFS.mkdir("/data");
  file = LittleFS.open("/data/sensor.txt", "w");
  if (file) {
    file.println("Sensor 1: 23.5C");
    file.println("Sensor 2: 45.2%");
    file.println("Sensor 3: 1013.25hPa");
    file.close();
    Serial.println("  /data/sensor.txt created");
  }
  
  // Create logs/app.log
  LittleFS.mkdir("/logs");
  file = LittleFS.open("/logs/app.log", "w");
  if (file) {
    file.println("[2024-01-01 12:00:00] Application started");
    file.println("[2024-01-01 12:00:01] WiFi connected");
    file.println("[2024-01-01 12:00:02] Sensors read");
    file.close();
    Serial.println("  /logs/app.log created");
  }
  
  // Create web/index.html
  LittleFS.mkdir("/web");
  file = LittleFS.open("/web/index.html", "w");
  if (file) {
    file.println("<!DOCTYPE html>");
    file.println("<html><head><title>ESP Test</title></head>");
    file.println("<body><h1>Test Page</h1>");
    file.println("<p>This page was uploaded with LittleFS Explorer.</p>");
    file.println("</body></html>");
    file.close();
    Serial.println("  /web/index.html created");
  }
  
  Serial.println("Test files ready!");
}

// Main page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>File Operations</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;margin:40px;background:#f5f5f5;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);margin-bottom:20px;}";
  html += "h1{color:#333;}";
  html += ".btn{padding:10px 20px;border:none;border-radius:5px;cursor:pointer;font-size:14px;margin:5px;}";
  html += ".btn-primary{background:#e94560;color:white;}";
  html += ".btn-success{background:#4ecdc4;color:white;}";
  html += "pre{background:#1a1a2e;color:#4ecdc4;padding:15px;border-radius:5px;overflow-x:auto;}";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h1>File Operations Example</h1>";
  html += "<p>File operations with LittleFSExplorer</p>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>Test Files</h2>";
  html += "<pre>";
  html += "/<br>";
  html += "├── config.txt<br>";
  html += "├── data/<br>";
  html += "│   └── sensor.txt<br>";
  html += "├── logs/<br>";
  html += "│   └── app.log<br>";
  html += "└── web/<br>";
  html += "    └── index.html<br>";
  html += "</pre>";
  html += "<button class='btn btn-primary' onclick='createFiles()'>Create Test Files</button>";
  html += "<button class='btn btn-success' onclick='listFiles()'>List Files</button>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>File List</h2>";
  html += "<pre id='fileList'>Loading...</pre>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>File Explorer</h2>";
  html += "<p>Go to <a href='/fs'>LittleFS Explorer</a> to manage files</p>";
  html += "</div>";
  html += "<script>";
  html += "function createFiles(){";
  html += "  fetch('/api/create-test-files').then(()=>listFiles());";
  html += "}";
  html += "function listFiles(){";
  html += "  fetch('/api/list-all').then(r=>r.json()).then(d=>{";
  html += "    var html='';";
  html += "    d.forEach(f=>{html+=f.path+' ('+f.size+' bytes)<br>';});";
  html += "    document.getElementById('fileList').innerHTML=html||'No files found';";
  html += "  });";
  html += "}";
  html += "listFiles();";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

// API: Create test files
void handleCreateTestFiles() {
  createTestFiles();
  server.send(200, "application/json", "{\"success\":true}");
}

// API: List all files
void handleListAll() {
  String json = "[";
  
  Dir dir = LittleFS.openDir("/");
  bool first = true;
  
  while (dir.next()) {
    if (!first) json += ",";
    first = false;
    
    json += "{\"path\":\"" + dir.fileName() + "\",";
    json += "\"size\":" + String(dir.fileSize()) + "}";
  }
  
  json += "]";
  server.send(200, "application/json", json);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting File Operations Example...");
  
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS!");
    return;
  }
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  
  // Create test files
  createTestFiles();
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/create-test-files", HTTP_GET, handleCreateTestFiles);
  server.on("/api/list-all", HTTP_GET, handleListAll);
  
  explorer.begin(&server);
  server.begin();
  
  Serial.println("File Operations ready!");
  Serial.println("Navigate to http://" + WiFi.localIP().toString() + "/ in your browser");
}

void loop() {
  server.handleClient();
}
