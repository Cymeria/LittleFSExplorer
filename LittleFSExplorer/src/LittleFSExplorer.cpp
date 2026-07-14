#include "LittleFSExplorer.h"
#include "webpage.h"

#ifdef ESP8266
  #include <StreamString.h>
#endif

LittleFSExplorer::LittleFSExplorer() : _server(nullptr), _prefix("/fs") {}

LittleFSExplorer::~LittleFSExplorer() {}

void LittleFSExplorer::begin(WebServerType* server) {
  begin(server, "/fs");
}

void LittleFSExplorer::begin(WebServerType* server, const char* prefix) {
  _server = server;
  _prefix = String(prefix);

  if (_prefix.endsWith("/")) {
    _prefix.remove(_prefix.length() - 1);
  }

  // Ana sayfa
  _server->on(_prefix.c_str(), HTTP_GET, [this]() {
    handleRoot();
  });

  // API endpointleri
  _server->on((_prefix + "/api/list").c_str(), HTTP_GET, [this]() {
    handleListFiles();
  });

  _server->on((_prefix + "/api/read").c_str(), HTTP_GET, [this]() {
    handleReadFile();
  });

  _server->on((_prefix + "/api/write").c_str(), HTTP_POST, [this]() {
    handleWriteFile();
  });

  // Multipart file upload - onUpload ile dosya parçalarını al
  _server->on((_prefix + "/api/upload").c_str(), HTTP_POST, 
    [this]() {
      // Upload tamamlandiktan sonra
      _server->send(200, "application/json", "{\"success\":true}");
    },
    [this]() {
      // Her chunk icin cagrilir
      HTTPUpload& upload = _server->upload();
      
      if (upload.status == UPLOAD_FILE_START) {
        String filename = "/" + upload.filename;
        if (!filename.startsWith("/")) filename = "/" + filename;
        
        _uploadFile = LittleFS.open(filename, "w");
      } else if (upload.status == UPLOAD_FILE_WRITE) {
        if (_uploadFile) {
          _uploadFile.write(upload.buf, upload.currentSize);
        }
      } else if (upload.status == UPLOAD_FILE_END) {
        if (_uploadFile) {
          _uploadFile.close();
        }
      }
    }
  );

  // Raw POST ile upload (alternatif)
  _server->on((_prefix + "/api/uploadraw").c_str(), HTTP_POST, [this]() {
    handleUploadFileRaw();
  });

  _server->on((_prefix + "/api/download").c_str(), HTTP_GET, [this]() {
    handleDownloadFile();
  });

  _server->on((_prefix + "/api/delete").c_str(), HTTP_POST, [this]() {
    handleDeleteFile();
  });

  _server->on((_prefix + "/api/mkdir").c_str(), HTTP_POST, [this]() {
    handleMkdir();
  });

  _server->on((_prefix + "/api/rename").c_str(), HTTP_POST, [this]() {
    handleRename();
  });

  _server->on((_prefix + "/api/info").c_str(), HTTP_GET, [this]() {
    handleFileInfo();
  });
}

void LittleFSExplorer::handleRoot() {
  _server->send_P(200, "text/html", INDEX_HTML);
}

String escapeJsonString(const String& input) {
  String output = "";
  output.reserve(input.length() + 16);
  for (unsigned int i = 0; i < input.length(); i++) {
    char c = input.charAt(i);
    switch (c) {
      case '"':  output += "\\\""; break;
      case '\\': output += "\\\\"; break;
      case '\n': output += "\\n"; break;
      case '\r': output += "\\r"; break;
      case '\t': output += "\\t"; break;
      case '\b': output += "\\b"; break;
      case '\f': output += "\\f"; break;
      default:
        if (c < 0x20) {
          char buf[8];
          snprintf(buf, sizeof(buf), "\\u%04x", (unsigned char)c);
          output += buf;
        } else {
          output += c;
        }
        break;
    }
  }
  return output;
}

void LittleFSExplorer::handleListFiles() {
  String path = "/";
  if (_server->hasArg("path")) {
    path = _server->arg("path");
  }

  if (!path.startsWith("/")) {
    path = "/" + path;
  }
  if (!path.endsWith("/") && path.length() > 1) {
    path += "/";
  }

  Dir dir = LittleFS.openDir(path);

  String json = "{\"path\":\"" + escapeJsonString(path) + "\",\"entries\":[";

  bool first = true;
  while (dir.next()) {
    if (!first) json += ",";
    first = false;

    String entryName = dir.fileName();
    
    String cleanPath = path;
    if (!cleanPath.endsWith("/")) cleanPath += "/";
    
    if (entryName.startsWith(cleanPath)) {
      entryName = entryName.substring(cleanPath.length());
    }
    
    int lastSlash = entryName.lastIndexOf('/');
    if (lastSlash >= 0) {
      entryName = entryName.substring(lastSlash + 1);
    }

    bool isDir = dir.isDirectory();
    json += "{\"name\":\"" + escapeJsonString(entryName) + "\",";
    json += "\"type\":\"" + String(isDir ? "directory" : "file") + "\",";
    json += "\"size\":" + String(isDir ? 0 : dir.fileSize()) + ",";
    json += "\"lastModified\":" + String(dir.fileCreationTime());
    json += "}";
  }

  json += "]}";

  _server->send(200, "application/json", json);
}

void LittleFSExplorer::handleReadFile() {
  if (!_server->hasArg("path")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path parameter\"}");
    return;
  }

  String path = _server->arg("path");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  if (!LittleFS.exists(path)) {
    String errorMsg = "{\"error\":\"File not found: " + escapeJsonString(path) + "\"}";
    _server->send(404, "application/json", errorMsg);
    return;
  }

  File file = LittleFS.open(path, "r");
  if (!file) {
    _server->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
    return;
  }

  size_t fileSize = file.size();
  
  if (fileSize > 65536) {
    file.close();
    _server->send(413, "application/json", "{\"error\":\"File too large (max 64KB)\"}");
    return;
  }

  String content = file.readString();
  file.close();

  String json = "{\"path\":\"" + escapeJsonString(path) + "\",\"content\":\"" + escapeJsonString(content) + "\",\"size\":" + String(fileSize) + "}";

  _server->send(200, "application/json", json);
}

void LittleFSExplorer::handleWriteFile() {
  if (!_server->hasArg("path") || !_server->hasArg("content")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path or content parameter\"}");
    return;
  }

  String path = _server->arg("path");
  String content = _server->arg("content");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  File file = LittleFS.open(path, "w");
  if (!file) {
    _server->send(500, "application/json", "{\"error\":\"Failed to create file\"}");
    return;
  }

  file.print(content);
  file.close();

  _server->send(200, "application/json", "{\"success\":true}");
}

void LittleFSExplorer::handleUploadFile() {
  _server->send(200, "application/json", "{\"success\":true}");
}

// Raw POST ile upload (base64 verisi icin - daha kucuk dosyalar icin)
void LittleFSExplorer::handleUploadFileRaw() {
  if (!_server->hasArg("path")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path parameter\"}");
    return;
  }

  String path = _server->arg("path");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  String content = _server->arg("plain");
  
  if (content.length() == 0) {
    _server->send(400, "application/json", "{\"error\":\"No data received\"}");
    return;
  }

  File file = LittleFS.open(path, "w");
  if (!file) {
    _server->send(500, "application/json", "{\"error\":\"Failed to create file\"}");
    return;
  }

  file.write((const uint8_t*)content.c_str(), content.length());
  file.close();

  String json = "{\"success\":true,\"size\":" + String(content.length()) + "}";
  _server->send(200, "application/json", json);
}

void LittleFSExplorer::handleDownloadFile() {
  if (!_server->hasArg("path")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path parameter\"}");
    return;
  }

  String path = _server->arg("path");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  if (!LittleFS.exists(path)) {
    _server->send(404, "application/json", "{\"error\":\"File not found\"}");
    return;
  }

  File file = LittleFS.open(path, "r");
  if (!file) {
    _server->send(500, "application/json", "{\"error\":\"Failed to open file\"}");
    return;
  }

  String contentType = getContentType(path);
  _server->streamFile(file, contentType);
  file.close();
}

void LittleFSExplorer::handleDeleteFile() {
  if (!_server->hasArg("path")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path parameter\"}");
    return;
  }

  String path = _server->arg("path");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  if (!LittleFS.exists(path)) {
    _server->send(404, "application/json", "{\"error\":\"File not found\"}");
    return;
  }

  bool success = LittleFS.remove(path);

  if (success) {
    _server->send(200, "application/json", "{\"success\":true}");
  } else {
    _server->send(500, "application/json", "{\"error\":\"Failed to delete file\"}");
  }
}

void LittleFSExplorer::handleMkdir() {
  if (!_server->hasArg("path")) {
    _server->send(400, "application/json", "{\"error\":\"Missing path parameter\"}");
    return;
  }

  String path = _server->arg("path");

  if (!path.startsWith("/")) {
    path = "/" + path;
  }

  bool success = LittleFS.mkdir(path);

  if (success) {
    _server->send(200, "application/json", "{\"success\":true}");
  } else {
    _server->send(500, "application/json", "{\"error\":\"Failed to create directory\"}");
  }
}

void LittleFSExplorer::handleRename() {
  if (!_server->hasArg("oldPath") || !_server->hasArg("newPath")) {
    _server->send(400, "application/json", "{\"error\":\"Missing oldPath or newPath parameter\"}");
    return;
  }

  String oldPath = _server->arg("oldPath");
  String newPath = _server->arg("newPath");

  if (!oldPath.startsWith("/")) oldPath = "/" + oldPath;
  if (!newPath.startsWith("/")) newPath = "/" + newPath;

  bool success = LittleFS.rename(oldPath, newPath);

  if (success) {
    _server->send(200, "application/json", "{\"success\":true}");
  } else {
    _server->send(500, "application/json", "{\"error\":\"Failed to rename file\"}");
  }
}

void LittleFSExplorer::handleFileInfo() {
  FSInfo fs_info;
  LittleFS.info(fs_info);

  String json = "{";
  json += "\"total\":" + String(fs_info.totalBytes) + ",";
  json += "\"used\":" + String(fs_info.usedBytes) + ",";
  json += "\"free\":" + String(fs_info.totalBytes - fs_info.usedBytes) + ",";
  json += "\"blockSize\":" + String(fs_info.blockSize) + ",";
  json += "\"pageSize\":" + String(fs_info.pageSize);
  json += "}";

  _server->send(200, "application/json", json);
}

String LittleFSExplorer::formatSize(size_t bytes) {
  if (bytes < 1024) return String(bytes) + " B";
  else if (bytes < 1048576) return String(bytes / 1024.0, 1) + " KB";
  else return String(bytes / 1048576.0, 1) + " MB";
}

String LittleFSExplorer::getContentType(const String& filename) {
  if (filename.endsWith(".html")) return "text/html";
  else if (filename.endsWith(".css")) return "text/css";
  else if (filename.endsWith(".js")) return "application/javascript";
  else if (filename.endsWith(".json")) return "application/json";
  else if (filename.endsWith(".png")) return "image/png";
  else if (filename.endsWith(".jpg") || filename.endsWith(".jpeg")) return "image/jpeg";
  else if (filename.endsWith(".gif")) return "image/gif";
  else if (filename.endsWith(".svg")) return "image/svg+xml";
  else if (filename.endsWith(".ico")) return "image/x-icon";
  else if (filename.endsWith(".txt")) return "text/plain";
  else if (filename.endsWith(".xml")) return "text/xml";
  else if (filename.endsWith(".pdf")) return "application/pdf";
  else if (filename.endsWith(".zip")) return "application/zip";
  else if (filename.endsWith(".gz")) return "application/gzip";
  else if (filename.endsWith(".bin")) return "application/octet-stream";
  else if (filename.endsWith(".hex")) return "application/octet-stream";
  else return "application/octet-stream";
}
