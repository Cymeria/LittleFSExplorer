/*
 * LittleFSExplorer - Data Logger Example
 * 
 * This example demonstrates logging sensor data to LittleFS
 * and viewing it via the web interface.
 * 
 * Features:
 * - Periodic sensor data logging
 * - CSV file creation
 * - View data via web interface
 * - Clear old data
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

// Sensor pins
#define SENSOR_PIN A0
#define LED_PIN 2

// Logging settings
unsigned long lastLogTime = 0;
const unsigned long LOG_INTERVAL = 60000; // 1 minute
int logCount = 0;

// Sensor reading
float readSensor() {
  int raw = analogRead(SENSOR_PIN);
  return raw * (3.3 / 1023.0); // Voltage conversion
}

// Generate filename
String getLogFileName() {
  return "/log_" + String(logCount) + ".csv";
}

// Create new log file
void createNewLogFile() {
  String filename = getLogFileName();
  File file = LittleFS.open(filename, "w");
  if (file) {
    file.println("timestamp,uptime_ms,sensor_value");
    file.close();
    Serial.println("New log file: " + filename);
  }
}

// Log data
void logData() {
  String filename = getLogFileName();
  File file = LittleFS.open(filename, "a");
  
  if (file) {
    float sensorValue = readSensor();
    unsigned long uptime = millis();
    
    file.print(uptime);
    file.print(",");
    file.print(uptime);
    file.print(",");
    file.println(sensorValue, 2);
    
    file.close();
    
    Serial.println("Data logged: " + String(sensorValue, 2) + "V");
    digitalWrite(LED_PIN, !digitalRead(LED_PIN)); // Blink LED
    
    logCount++;
  } else {
    Serial.println("Failed to open file!");
  }
}

// Main page
void handleRoot() {
  String html = "<!DOCTYPE html><html><head>";
  html += "<meta charset='UTF-8'>";
  html += "<meta name='viewport' content='width=device-width, initial-scale=1.0'>";
  html += "<title>Data Logger</title>";
  html += "<style>";
  html += "body{font-family:Arial,sans-serif;margin:40px;background:#f5f5f5;}";
  html += ".card{background:white;padding:20px;border-radius:10px;box-shadow:0 2px 10px rgba(0,0,0,0.1);margin-bottom:20px;}";
  html += "h1{color:#333;}";
  html += ".btn{padding:10px 20px;border:none;border-radius:5px;cursor:pointer;font-size:16px;margin:5px;}";
  html += ".btn-primary{background:#e94560;color:white;}";
  html += ".btn-danger{background:#ff6b6b;color:white;}";
  html += ".status{font-size:18px;font-weight:bold;color:#4ecdc4;}";
  html += "table{width:100%;border-collapse:collapse;margin-top:10px;}";
  html += "th,td{padding:8px;text-align:left;border-bottom:1px solid #ddd;}";
  html += "th{background:#f5f5f5;}";
  html += "</style></head><body>";
  html += "<div class='card'>";
  html += "<h1>Data Logger</h1>";
  html += "<p>Data logging system with LittleFSExplorer</p>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>Status</h2>";
  html += "<p>Sensor: <span class='status' id='sensor'>--</span> V</p>";
  html += "<p>Total Logs: <span class='status' id='count'>--</span></p>";
  html += "<p>Last Log: <span class='status' id='lastLog'>--</span></p>";
  html += "<button class='btn btn-primary' onclick='manualLog()'>Log Now</button>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>Recent Data</h2>";
  html += "<table><thead><tr><th>Time(ms)</th><th>Value(V)</th></tr></thead>";
  html += "<tbody id='dataTable'></tbody></table>";
  html += "</div>";
  html += "<div class='card'>";
  html += "<h2>File Management</h2>";
  html += "<p><a href='/fs'>LittleFS Explorer</a> - Manage log files</p>";
  html += "<button class='btn btn-danger' onclick='clearLogs()'>Clear All Logs</button>";
  html += "</div>";
  html += "<script>";
  html += "function getData(){";
  html += "  fetch('/api/logger/status').then(r=>r.json()).then(d=>{";
  html += "    document.getElementById('sensor').textContent=d.sensor.toFixed(2);";
  html += "    document.getElementById('count').textContent=d.logCount;";
  html += "    document.getElementById('lastLog').textContent=new Date(d.lastLog*1000).toLocaleTimeString();";
  html += "  });";
  html += "  fetch('/api/logger/data?limit=10').then(r=>r.json()).then(d=>{";
  html += "    var html='';";
  html += "    d.forEach(r=>{html+='<tr><td>'+r.uptime+'</td><td>'+r.value+'</td></tr>';});";
  html += "    document.getElementById('dataTable').innerHTML=html;";
  html += "  });";
  html += "}";
  html += "function manualLog(){fetch('/api/logger/log').then(()=>getData());}";
  html += "function clearLogs(){if(confirm('All logs will be deleted!'))fetch('/api/logger/clear').then(()=>getData());}";
  html += "setInterval(getData,2000);getData();";
  html += "</script></body></html>";
  
  server.send(200, "text/html", html);
}

// API: Logger status
void handleLoggerStatus() {
  String json = "{";
  json += "\"sensor\":" + String(readSensor(), 2) + ",";
  json += "\"logCount\":" + String(logCount) + ",";
  json += "\"lastLog\":" + String(millis() / 1000);
  json += "}";
  server.send(200, "application/json", json);
}

// API: Recent data
void handleLoggerData() {
  int limit = 10;
  if (server.hasArg("limit")) {
    limit = server.arg("limit").toInt();
  }
  
  String json = "[";
  
  // Read last log file
  String filename = getLogFileName();
  File file = LittleFS.open(filename, "r");
  
  if (file) {
    int lineCount = 0;
    String lines[limit];
    
    while (file.available() && lineCount < limit + 1) {
      String line = file.readStringUntil('\n');
      line.trim();
      if (line.length() > 0 && !line.startsWith("timestamp")) {
        if (lineCount < limit) {
          lines[lineCount] = line;
        }
        lineCount++;
      }
    }
    file.close();
    
    // Add in reverse order (newest first)
    bool first = true;
    for (int i = lineCount - 1; i >= 0 && i < limit; i--) {
      if (!first) json += ",";
      first = false;
      
      int commaIndex = lines[i].lastIndexOf(',');
      if (commaIndex > 0) {
        String uptime = lines[i].substring(0, commaIndex);
        String value = lines[i].substring(commaIndex + 1);
        json += "{\"uptime\":" + uptime + ",\"value\":" + value + "}";
      }
    }
  }
  
  json += "]";
  server.send(200, "application/json", json);
}

// API: Manual log
void handleLoggerLog() {
  logData();
  server.send(200, "application/json", "{\"success\":true}");
}

// API: Clear logs
void handleLoggerClear() {
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    String filename = dir.fileName();
    if (filename.startsWith("/log_") && filename.endsWith(".csv")) {
      LittleFS.remove(filename);
      Serial.println("Deleted: " + filename);
    }
  }
  
  logCount = 0;
  createNewLogFile();
  
  server.send(200, "application/json", "{\"success\":true}");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting Data Logger Example...");
  
  pinMode(LED_PIN, OUTPUT);
  pinMode(SENSOR_PIN, INPUT);
  
  if (!LittleFS.begin()) {
    Serial.println("Failed to initialize LittleFS!");
    return;
  }
  
  // Find existing log count
  Dir dir = LittleFS.openDir("/");
  while (dir.next()) {
    String filename = dir.fileName();
    if (filename.startsWith("/log_") && filename.endsWith(".csv")) {
      int num = filename.substring(5, filename.length() - 4).toInt();
      if (num >= logCount) logCount = num + 1;
    }
  }
  
  if (logCount == 0) {
    createNewLogFile();
  } else {
    logCount--;
    logCount++;
    createNewLogFile();
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
  
  server.on("/", HTTP_GET, handleRoot);
  server.on("/api/logger/status", HTTP_GET, handleLoggerStatus);
  server.on("/api/logger/data", HTTP_GET, handleLoggerData);
  server.on("/api/logger/log", HTTP_GET, handleLoggerLog);
  server.on("/api/logger/clear", HTTP_GET, handleLoggerClear);
  
  explorer.begin(&server);
  server.begin();
  
  Serial.println("Data Logger ready!");
}

void loop() {
  server.handleClient();
  
  unsigned long currentMillis = millis();
  if (currentMillis - lastLogTime >= LOG_INTERVAL) {
    lastLogTime = currentMillis;
    logData();
  }
}
