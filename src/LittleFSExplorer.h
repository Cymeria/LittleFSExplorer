#ifndef LITTLEFS_EXPLORER_H
#define LITTLEFS_EXPLORER_H

#include <Arduino.h>

#ifdef ESP8266
  #include <ESP8266WebServer.h>
  #include <LittleFS.h>
  typedef ESP8266WebServer WebServerType;
#elif defined(ESP32)
  #include <WebServer.h>
  #include <LittleFS.h>
  typedef WebServer WebServerType;
#else
  #error "This library only supports ESP8266 and ESP32"
#endif

class LittleFSExplorer {
public:
  LittleFSExplorer();
  ~LittleFSExplorer();

  void begin(WebServerType* server);
  void begin(WebServerType* server, const char* prefix);

private:
  WebServerType* _server;
  String _prefix;
  File _uploadFile;

  void handleRoot();
  void handleListFiles();
  void handleReadFile();
  void handleWriteFile();
  void handleUploadFile();
  void handleUploadFileRaw();
  void handleDownloadFile();
  void handleDeleteFile();
  void handleMkdir();
  void handleRename();
  void handleFileInfo();

  String formatSize(size_t bytes);
  String getContentType(const String& filename);
};

#endif
