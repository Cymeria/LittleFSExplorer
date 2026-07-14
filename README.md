# LittleFSExplorer

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino](https://img.shields.io/badge/Arduino-IDE_2.x-blue.svg)](https://www.arduino.cc/en/software)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-supported-green.svg)](https://platformio.org/)
[![ESP8266](https://img.shields.io/badge/ESP8266-supported-orange.svg)](https://www.espressif.com/en/products/socs/esp8266)
[![ESP32](https://img.shields.io/badge/ESP32-supported-blue.svg)](https://www.espressif.com/en/products/socs/esp32)

A modern web-based file explorer and editor library for ESP8266/ESP32 with LittleFS.

## Features

- Modern and responsive web interface
- Dark/Light theme support (user preference saved)
- File tree browser
- Text file editor (textarea-based)
- File upload (drag & drop supported)
- Binary file upload (images, etc.)
- File download
- File delete and rename
- Directory creation
- Image preview
- Storage usage info
- ESP8266 and ESP32 support
- JSON API endpoints

## Screenshot

```
┌─────────────────────────────────────────────────────────────┐
│  📁 LittleFS Explorer                              [🌓 Theme]│
├──────────────┬──────────────────────────────────────────────┤
│ FILES        │  🏠 Root / data                              │
│ ┌──────────┐ │  ⬆ parent folder                           │
│ │ + 📁 ↻   │ │─────────────────────────────────────────────│
│ ├──────────┤ │  📄 sensor.txt    [⬇ Download] [💾 Save]   │
│ │ ⬆ parent │ │─────────────────────────────────────────────│
│ │ 📁 data/ │ │  Sensor 1: 23.5C                           │
│ │ 📁 logs/ │ │  Sensor 2: 45.2%                           │
│ │ 📁 web/  │ │  Sensor 3: 1013.25hPa                      │
│ │ 📄 config│ │                                             │
│ ├──────────┤ │                                             │
│ │ 45KB/    │ │                                             │
│ │ 256KB    │ │                                             │
│ └──────────┘ │                                             │
└──────────────┴──────────────────────────────────────────────┘
```

## Installation

### Arduino IDE 2

1. Open Arduino IDE 2
2. Go to `Sketch > Include Library > Add .ZIP Library...`
3. Select this library's zip file

### PlatformIO

Add to `platformio.ini`:

```ini
[env:esp8266]
platform = espressif8266
board = nodemcuv2
framework = arduino
lib_deps =
    LittleFSExplorer

[env:esp32]
platform = espressif32
board = esp32dev
framework = arduino
lib_deps =
    LittleFSExplorer
```

### Manual Installation

1. Download this repository
2. Copy `LittleFSExplorer` folder to `Arduino/libraries`
3. Restart Arduino IDE

## Quick Start

```cpp
#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WebServer.h>
#elif defined(ESP32)
  #include <WebServer.h>
#endif

#include <LittleFS.h>
#include <LittleFSExplorer.h>

#ifdef ESP8266
  ESP8266WebServer server(80);
#elif defined(ESP32)
  WebServer server(80);
#endif

LittleFSExplorer explorer;

void setup() {
  Serial.begin(115200);
  
  // Initialize LittleFS
  if (!LittleFS.begin()) {
    Serial.println("LittleFS initialization failed!");
    LittleFS.format();
    LittleFS.begin();
  }
  
  // Initialize LittleFSExplorer
  explorer.begin(&server);
  
  // Start web server
  server.begin();
  
  Serial.println("Ready!");
  Serial.println("Navigate to /fs in your browser");
}

void loop() {
  server.handleClient();
}
```

## API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/fs` | GET | Main page (web interface) |
| `/fs/api/list?path=/` | GET | List directory contents |
| `/fs/api/read?path=/file.txt` | GET | Read file contents |
| `/fs/api/write` | POST | Write file |
| `/fs/api/upload` | POST | Upload file (multipart) |
| `/fs/api/download?path=/file.txt` | GET | Download file |
| `/fs/api/delete` | POST | Delete file |
| `/fs/api/mkdir` | POST | Create directory |
| `/fs/api/rename` | POST | Rename file |
| `/fs/api/info` | GET | System info |

### API Examples

```javascript
// List files
fetch('/fs/api/list?path=/')
  .then(r => r.json())
  .then(data => console.log(data));

// Read file
fetch('/fs/api/read?path=/config.txt')
  .then(r => r.json())
  .then(data => console.log(data.content));

// Write file
fetch('/fs/api/write', {
  method: 'POST',
  headers: {'Content-Type': 'application/x-www-form-urlencoded'},
  body: 'path=/test.txt&content=Hello World'
});
```

## Examples

The library includes 4 example applications:

### 1. BasicExplorer
Basic LittleFSExplorer usage. Connects to WiFi and serves the file explorer.

```cpp
#include <LittleFSExplorer.h>
// See examples/BasicExplorer/BasicExplorer.ino
```

### 2. WebServer
LittleFSExplorer with other web server endpoints.

```cpp
// System status, sensor data, LED control
// See examples/WebServer/WebServer.ino
```

### 3. DataLogger
Logs sensor data to LittleFS and displays via web interface.

```cpp
// Periodic data logging, CSV files
// See examples/DataLogger/DataLogger.ino
```

### 4. FileOperations
File operation examples (create, delete, read, write).

```cpp
// Test file creation, file listing
// See examples/FileOperations/FileOperations.ino
```

## Uploading LittleFS

### ESP8266

1. In Arduino IDE: `Tools > ESP8266 LittleFS Data Upload`
2. Or use `LittleFSUploader` plugin

### ESP32

1. In Arduino IDE: `Tools > ESP32 Sketch Data Upload`
2. Or: `python -m esptool --port COM3 write_flash 0x200000 littlefs.bin`

## Customization

### Custom Prefix

```cpp
// Use /files instead of /fs
explorer.begin(&server, "/files");
```

### Multi-language

The web interface is in English by default. To change the language, edit `webpage.h`.

## Troubleshooting

### LittleFS Failed to Initialize

```cpp
if (!LittleFS.begin()) {
  Serial.println("Formatting LittleFS...");
  LittleFS.format();
  LittleFS.begin();
}
```

### Large File Upload Issues

If you have issues uploading large files, increase the web server body size limit:

```cpp
// ESP8266
ESP8266WebServer server(80);
server.bodySizeLimit(32 * 1024); // 32KB limit

// ESP32 - has larger default limit
WebServer server(80);
```

### Binary File Upload

Binary files (images, etc.) are uploaded using multipart/form-data. This method works for all file sizes and types. If you still have issues:

1. Check the browser developer tools (F12) Network tab
2. See which request failed
3. Check Serial Monitor for server-side error messages

### Insufficient Memory

ESP8266 may have memory issues with large files. You can reduce the 64KB limit in `handleReadFile` function.

## Contributing

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License

MIT License - See [LICENSE](LICENSE) file for details.

## Acknowledgments

- [Arduino](https://www.arduino.cc/) - Software platform
- [PlatformIO](https://platformio.org/) - Development environment
- [LittleFS](https://github.com/littlefs-project/littlefs) - File system
- [ESP8266](https://github.com/esp8266/Arduino) - ESP8266 Arduino core
- [ESP32](https://github.com/espressif/arduino-esp32) - ESP32 Arduino core
