#ifndef WEBPAGE_H
#define WEBPAGE_H

const char INDEX_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>LittleFS Explorer</title>
<style>
:root {
  --bg-primary: #1a1a2e;
  --bg-secondary: #16213e;
  --bg-tertiary: #0f3460;
  --text-primary: #e4e4e4;
  --text-secondary: #a0a0a0;
  --accent: #e94560;
  --accent-hover: #ff6b81;
  --border: #2a2a4a;
  --success: #4ecdc4;
  --warning: #ffe66d;
  --danger: #ff6b6b;
  --scrollbar-bg: #1a1a2e;
  --scrollbar-thumb: #3a3a5a;
}

[data-theme="light"] {
  --bg-primary: #f5f5f5;
  --bg-secondary: #ffffff;
  --bg-tertiary: #e8e8e8;
  --text-primary: #2d2d2d;
  --text-secondary: #666666;
  --accent: #e94560;
  --accent-hover: #d63851;
  --border: #ddd;
  --success: #2ecc71;
  --warning: #f39c12;
  --danger: #e74c3c;
  --scrollbar-bg: #f5f5f5;
  --scrollbar-thumb: #ccc;
}

* {
  margin: 0;
  padding: 0;
  box-sizing: border-box;
}

body {
  font-family: -apple-system, BlinkMacSystemFont, 'Segoe UI', Roboto, Oxygen, Ubuntu, sans-serif;
  background: var(--bg-primary);
  color: var(--text-primary);
  height: 100vh;
  overflow: hidden;
}

::-webkit-scrollbar {
  width: 8px;
  height: 8px;
}

::-webkit-scrollbar-track {
  background: var(--scrollbar-bg);
}

::-webkit-scrollbar-thumb {
  background: var(--scrollbar-thumb);
  border-radius: 4px;
}

::-webkit-scrollbar-thumb:hover {
  background: var(--accent);
}

.header {
  display: flex;
  justify-content: space-between;
  align-items: center;
  padding: 12px 20px;
  background: var(--bg-secondary);
  border-bottom: 1px solid var(--border);
  height: 56px;
}

.header h1 {
  font-size: 18px;
  font-weight: 600;
  display: flex;
  align-items: center;
  gap: 10px;
}

.header h1::before {
  content: "📁";
  font-size: 20px;
}

.header-actions {
  display: flex;
  gap: 10px;
  align-items: center;
}

.theme-toggle {
  background: var(--bg-tertiary);
  border: 1px solid var(--border);
  color: var(--text-primary);
  padding: 8px 12px;
  border-radius: 6px;
  cursor: pointer;
  font-size: 14px;
  transition: all 0.2s;
}

.theme-toggle:hover {
  background: var(--accent);
  color: white;
}

.container {
  display: flex;
  height: calc(100vh - 56px);
}

.sidebar {
  width: 280px;
  background: var(--bg-secondary);
  border-right: 1px solid var(--border);
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.sidebar-header {
  padding: 12px 16px;
  border-bottom: 1px solid var(--border);
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.sidebar-header h2 {
  font-size: 14px;
  font-weight: 600;
  color: var(--text-secondary);
  text-transform: uppercase;
  letter-spacing: 0.5px;
}

.sidebar-actions {
  display: flex;
  gap: 6px;
}

.btn-icon {
  background: var(--bg-tertiary);
  border: 1px solid var(--border);
  color: var(--text-primary);
  width: 28px;
  height: 28px;
  border-radius: 4px;
  cursor: pointer;
  display: flex;
  align-items: center;
  justify-content: center;
  font-size: 14px;
  transition: all 0.2s;
}

.btn-icon:hover {
  background: var(--accent);
  color: white;
}

.file-tree {
  flex: 1;
  overflow-y: auto;
  padding: 8px 0;
}

.file-item {
  display: flex;
  align-items: center;
  padding: 8px 16px;
  cursor: pointer;
  transition: background 0.15s;
  gap: 8px;
  font-size: 14px;
}

.file-item:hover {
  background: var(--bg-tertiary);
}

.file-item.active {
  background: var(--accent);
  color: white;
}

.file-item .icon {
  font-size: 16px;
  width: 20px;
  text-align: center;
}

.file-item .name {
  flex: 1;
  white-space: nowrap;
  overflow: hidden;
  text-overflow: ellipsis;
}

.file-item .size {
  font-size: 12px;
  color: var(--text-secondary);
}

.file-item.active .size {
  color: rgba(255,255,255,0.7);
}

.file-item.parent-folder {
  color: var(--text-secondary);
  font-style: italic;
}

.main-content {
  flex: 1;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.toolbar {
  display: flex;
  align-items: center;
  padding: 10px 16px;
  background: var(--bg-secondary);
  border-bottom: 1px solid var(--border);
  gap: 10px;
}

.breadcrumb {
  display: flex;
  align-items: center;
  flex: 1;
  gap: 4px;
  font-size: 14px;
  overflow-x: auto;
  white-space: nowrap;
}

.breadcrumb-item {
  color: var(--text-secondary);
  cursor: pointer;
  padding: 4px 8px;
  border-radius: 4px;
  transition: all 0.2s;
}

.breadcrumb-item:hover {
  background: var(--bg-tertiary);
  color: var(--text-primary);
}

.breadcrumb-item:last-child {
  color: var(--text-primary);
  font-weight: 500;
}

.breadcrumb-separator {
  color: var(--text-secondary);
}

.toolbar-actions {
  display: flex;
  gap: 8px;
}

.btn {
  padding: 8px 16px;
  border-radius: 6px;
  font-size: 13px;
  font-weight: 500;
  cursor: pointer;
  transition: all 0.2s;
  border: none;
  display: flex;
  align-items: center;
  gap: 6px;
}

.btn-primary {
  background: var(--accent);
  color: white;
}

.btn-primary:hover {
  background: var(--accent-hover);
}

.btn-success {
  background: var(--success);
  color: white;
}

.btn-success:hover {
  background: #3dbdb5;
}

.btn-secondary {
  background: var(--bg-tertiary);
  color: var(--text-primary);
  border: 1px solid var(--border);
}

.btn-secondary:hover {
  background: var(--accent);
  color: white;
  border-color: var(--accent);
}

.btn-danger {
  background: var(--danger);
  color: white;
}

.btn-danger:hover {
  background: #c0392b;
}

.content-area {
  flex: 1;
  display: flex;
  overflow: hidden;
}

.editor-panel {
  flex: 1;
  display: flex;
  flex-direction: column;
  overflow: hidden;
}

.editor-header {
  display: flex;
  align-items: center;
  justify-content: space-between;
  padding: 10px 16px;
  background: var(--bg-secondary);
  border-bottom: 1px solid var(--border);
}

.editor-header-left {
  display: flex;
  align-items: center;
  gap: 10px;
}

.editor-header .filename {
  font-weight: 500;
  display: flex;
  align-items: center;
  gap: 8px;
}

.editor-header .status {
  font-size: 12px;
  color: var(--success);
}

.editor-header-actions {
  display: flex;
  gap: 8px;
}

.editor-content {
  flex: 1;
  overflow: hidden;
}

.editor-content textarea {
  width: 100%;
  height: 100%;
  background: var(--bg-primary);
  color: var(--text-primary);
  border: none;
  padding: 16px;
  font-family: 'Consolas', 'Monaco', 'Courier New', monospace;
  font-size: 14px;
  line-height: 1.6;
  resize: none;
  outline: none;
}

.editor-content textarea::placeholder {
  color: var(--text-secondary);
}

.drop-zone {
  flex: 1;
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
  color: var(--text-secondary);
  gap: 16px;
}

.drop-zone.dragover {
  background: var(--bg-tertiary);
  border: 2px dashed var(--accent);
}

.drop-zone .icon {
  font-size: 64px;
  opacity: 0.5;
}

.drop-zone .text {
  font-size: 16px;
}

.drop-zone .hint {
  font-size: 12px;
  opacity: 0.7;
}

.preview-content {
  flex: 1;
  display: flex;
  align-items: center;
  justify-content: center;
  padding: 20px;
  background: var(--bg-primary);
  overflow: auto;
}

.preview-content img {
  max-width: 100%;
  max-height: 100%;
  object-fit: contain;
  border-radius: 8px;
  box-shadow: 0 4px 20px rgba(0,0,0,0.3);
}

.storage-info {
  padding: 12px 16px;
  background: var(--bg-secondary);
  border-top: 1px solid var(--border);
  font-size: 12px;
  color: var(--text-secondary);
}

.storage-bar {
  height: 4px;
  background: var(--bg-tertiary);
  border-radius: 2px;
  margin-top: 8px;
  overflow: hidden;
}

.storage-bar-fill {
  height: 100%;
  background: var(--accent);
  transition: width 0.3s;
}

.modal-overlay {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  bottom: 0;
  background: rgba(0,0,0,0.6);
  display: none;
  align-items: center;
  justify-content: center;
  z-index: 1000;
}

.modal-overlay.active {
  display: flex;
}

.modal {
  background: var(--bg-secondary);
  border-radius: 12px;
  padding: 24px;
  min-width: 320px;
  max-width: 90%;
  box-shadow: 0 20px 60px rgba(0,0,0,0.3);
}

.modal h3 {
  margin-bottom: 16px;
  font-size: 18px;
}

.modal input {
  width: 100%;
  padding: 10px 14px;
  background: var(--bg-primary);
  border: 1px solid var(--border);
  border-radius: 6px;
  color: var(--text-primary);
  font-size: 14px;
  margin-bottom: 16px;
  outline: none;
}

.modal input:focus {
  border-color: var(--accent);
}

.modal-actions {
  display: flex;
  justify-content: flex-end;
  gap: 10px;
}

.toast {
  position: fixed;
  bottom: 20px;
  right: 20px;
  padding: 12px 20px;
  border-radius: 8px;
  color: white;
  font-size: 14px;
  z-index: 2000;
  animation: slideIn 0.3s ease;
}

.toast.success { background: var(--success); }
.toast.error { background: var(--danger); }
.toast.warning { background: var(--warning); color: #333; }

@keyframes slideIn {
  from { transform: translateX(100%); opacity: 0; }
  to { transform: translateX(0); opacity: 1; }
}

.context-menu {
  position: fixed;
  background: var(--bg-secondary);
  border: 1px solid var(--border);
  border-radius: 8px;
  padding: 6px 0;
  min-width: 160px;
  box-shadow: 0 10px 40px rgba(0,0,0,0.3);
  z-index: 1500;
  display: none;
}

.context-menu.active {
  display: block;
}

.context-menu-item {
  padding: 8px 16px;
  cursor: pointer;
  font-size: 14px;
  display: flex;
  align-items: center;
  gap: 10px;
  transition: background 0.15s;
}

.context-menu-item:hover {
  background: var(--bg-tertiary);
}

.context-menu-item.danger {
  color: var(--danger);
}

.context-menu-divider {
  height: 1px;
  background: var(--border);
  margin: 6px 0;
}

@media (max-width: 768px) {
  .sidebar {
    width: 240px;
  }
  
  .toolbar {
    flex-wrap: wrap;
  }
}
</style>
</head>
<body>
<div class="header">
  <h1>LittleFS Explorer</h1>
  <div class="header-actions">
    <button class="theme-toggle" onclick="toggleTheme()">🌓 Theme</button>
  </div>
</div>

<div class="container">
  <div class="sidebar">
    <div class="sidebar-header">
      <h2>Files</h2>
      <div class="sidebar-actions">
        <button class="btn-icon" onclick="createNewFile()" title="New File">+</button>
        <button class="btn-icon" onclick="createNewFolder()" title="New Folder">📁</button>
        <button class="btn-icon" onclick="refreshFiles()" title="Refresh">↻</button>
      </div>
    </div>
    <div class="file-tree" id="fileTree"></div>
    <div class="storage-info">
      <div id="storageText">Loading...</div>
      <div class="storage-bar">
        <div class="storage-bar-fill" id="storageBar" style="width: 0%"></div>
      </div>
    </div>
  </div>

  <div class="main-content">
    <div class="toolbar">
      <div class="breadcrumb" id="breadcrumb"></div>
      <div class="toolbar-actions">
        <button class="btn btn-secondary" onclick="uploadFile()">⬆ Upload</button>
      </div>
    </div>
    <div class="content-area">
      <div class="editor-panel">
        <div class="editor-header">
          <div class="editor-header-left">
            <div class="filename" id="editorFilename">
              <span>📄</span>
              <span>No file selected</span>
            </div>
            <div class="status" id="editorStatus"></div>
          </div>
          <div class="editor-header-actions" id="editorActions" style="display:none">
            <button class="btn btn-secondary" onclick="downloadCurrentFile()">⬇ Download</button>
            <button class="btn btn-success" id="btnSave" onclick="saveCurrentFile()">💾 Save</button>
          </div>
        </div>
        <div class="editor-content">
          <div class="drop-zone" id="dropZone">
            <div class="icon">📂</div>
            <div class="text">Select a file or drag and drop</div>
            <div class="hint">Select a file from the left panel or create a new one</div>
          </div>
          <textarea id="editorTextarea" style="display:none" placeholder="File content..."></textarea>
          <div class="preview-content" id="previewContent" style="display:none">
            <img id="previewImage" src="" alt="Preview">
          </div>
        </div>
      </div>
    </div>
  </div>
</div>

<div class="modal-overlay" id="modalOverlay">
  <div class="modal">
    <h3 id="modalTitle">New File</h3>
    <input type="text" id="modalInput" placeholder="File name">
    <div class="modal-actions">
      <button class="btn btn-secondary" onclick="closeModal()">Cancel</button>
      <button class="btn btn-primary" onclick="confirmModal()">OK</button>
    </div>
  </div>
</div>

<div class="context-menu" id="contextMenu">
  <div class="context-menu-item" onclick="openFile()">📄 Open</div>
  <div class="context-menu-item" onclick="renameFile()">✏️ Rename</div>
  <div class="context-menu-divider"></div>
  <div class="context-menu-item danger" onclick="deleteFile()">🗑️ Delete</div>
</div>

<input type="file" id="fileInput" style="display:none" onchange="handleFileUpload(event)" multiple>

<script>
var currentPath = '/';
var currentFile = null;
var contextMenuTarget = null;
var modalCallback = null;

var previewExtensions = ['jpg', 'jpeg', 'png', 'gif', 'bmp', 'ico', 'webp', 'svg'];

function isPreviewable(filename) {
  var ext = filename.split('.').pop().toLowerCase();
  return previewExtensions.indexOf(ext) !== -1;
}

var api = {
  list: function(path) {
    return fetch('/fs/api/list?path=' + encodeURIComponent(path)).then(function(r) { return r.json(); });
  },
  read: function(path) {
    return fetch('/fs/api/read?path=' + encodeURIComponent(path)).then(function(r) { return r.json(); });
  },
  write: function(path, content) {
    return fetch('/fs/api/write', {
      method: 'POST',
      headers: {'Content-Type': 'application/x-www-form-urlencoded'},
      body: 'path=' + encodeURIComponent(path) + '&content=' + encodeURIComponent(content)
    }).then(function(r) { return r.json(); });
  },
  uploadMultipart: function(path, file) {
    var formData = new FormData();
    formData.append('file', file, path);
    return fetch('/fs/api/upload', {
      method: 'POST',
      body: formData
    }).then(function(r) { return r.json(); });
  },
  delete: function(path) {
    return fetch('/fs/api/delete', {
      method: 'POST',
      headers: {'Content-Type': 'application/x-www-form-urlencoded'},
      body: 'path=' + encodeURIComponent(path)
    }).then(function(r) { return r.json(); });
  },
  mkdir: function(path) {
    return fetch('/fs/api/mkdir', {
      method: 'POST',
      headers: {'Content-Type': 'application/x-www-form-urlencoded'},
      body: 'path=' + encodeURIComponent(path)
    }).then(function(r) { return r.json(); });
  },
  rename: function(oldPath, newPath) {
    return fetch('/fs/api/rename', {
      method: 'POST',
      headers: {'Content-Type': 'application/x-www-form-urlencoded'},
      body: 'oldPath=' + encodeURIComponent(oldPath) + '&newPath=' + encodeURIComponent(newPath)
    }).then(function(r) { return r.json(); });
  },
  info: function() {
    return fetch('/fs/api/info').then(function(r) { return r.json(); });
  },
  download: function(path) {
    return '/fs/api/download?path=' + encodeURIComponent(path);
  }
};

function init() {
  var savedTheme = localStorage.getItem('theme') || 'dark';
  document.documentElement.setAttribute('data-theme', savedTheme);
  refreshFiles();
  loadStorageInfo();
  setupEventListeners();
}

function setupEventListeners() {
  var textarea = document.getElementById('editorTextarea');
  textarea.addEventListener('input', function() {
    document.getElementById('editorStatus').textContent = 'Modified';
    document.getElementById('editorStatus').style.color = 'var(--warning)';
  });

  document.addEventListener('click', function() {
    document.getElementById('contextMenu').classList.remove('active');
  });

  var dropZone = document.getElementById('dropZone');
  dropZone.addEventListener('dragover', function(e) {
    e.preventDefault();
    dropZone.classList.add('dragover');
  });
  dropZone.addEventListener('dragleave', function() {
    dropZone.classList.remove('dragover');
  });
  dropZone.addEventListener('drop', function(e) {
    e.preventDefault();
    dropZone.classList.remove('dragover');
    if (e.dataTransfer.files.length > 0) {
      for (var i = 0; i < e.dataTransfer.files.length; i++) {
        uploadFileDirect(e.dataTransfer.files[i]);
      }
    }
  });
}

function toggleTheme() {
  var current = document.documentElement.getAttribute('data-theme');
  var next = current === 'dark' ? 'light' : 'dark';
  document.documentElement.setAttribute('data-theme', next);
  localStorage.setItem('theme', next);
}

async function refreshFiles() {
  try {
    var data = await api.list(currentPath);
    renderFileTree(data.entries);
    renderBreadcrumb();
  } catch (e) {
    showToast('Failed to load files', 'error');
  }
}

function renderFileTree(entries) {
  var tree = document.getElementById('fileTree');
  tree.innerHTML = '';

  if (currentPath !== '/') {
    var parentDiv = document.createElement('div');
    parentDiv.className = 'file-item parent-folder';
    parentDiv.innerHTML = '<span class="icon">⬆</span><span class="name">..parent folder</span>';
    parentDiv.addEventListener('click', function() {
      var parts = currentPath.split('/').filter(function(p) { return p; });
      parts.pop();
      var parentPath = parts.length > 0 ? '/' + parts.join('/') + '/' : '/';
      navigateTo(parentPath);
    });
    tree.appendChild(parentDiv);
  }

  entries.sort(function(a, b) {
    if (a.type !== b.type) return a.type === 'directory' ? -1 : 1;
    return a.name.localeCompare(b.name);
  });

  entries.forEach(function(entry) {
    var div = document.createElement('div');
    div.className = 'file-item';
    div.setAttribute('data-name', entry.name);
    div.setAttribute('data-type', entry.type);

    var icon = entry.type === 'directory' ? '📁' : getFileIcon(entry.name);
    var size = entry.type === 'file' ? formatSize(entry.size) : '';

    div.innerHTML = '<span class="icon">' + icon + '</span>' +
      '<span class="name">' + entry.name + '</span>' +
      '<span class="size">' + size + '</span>';

    div.addEventListener('click', function() {
      if (entry.type === 'directory') {
        navigateTo(currentPath + entry.name + '/');
      } else {
        openFileByName(currentPath + entry.name);
      }
    });

    div.addEventListener('contextmenu', function(e) {
      e.preventDefault();
      contextMenuTarget = {name: entry.name, type: entry.type, path: currentPath + entry.name};
      showContextMenu(e.clientX, e.clientY);
    });

    if (currentFile && currentPath + entry.name === currentFile) {
      div.classList.add('active');
    }

    tree.appendChild(div);
  });
}

function getFileIcon(filename) {
  var ext = filename.split('.').pop().toLowerCase();
  var icons = {
    'txt': '📝', 'md': '📝', 'log': '📝',
    'html': '🌐', 'htm': '🌐', 'css': '🎨', 'js': '⚡',
    'json': '📋', 'xml': '📋',
    'png': '🖼️', 'jpg': '🖼️', 'jpeg': '🖼️', 'gif': '🖼️', 'bmp': '🖼️',
    'ino': '🔌', 'h': '📄', 'cpp': '📄', 'c': '📄',
    'py': '🐍', 'rb': '💎', 'php': '🐘',
    'zip': '📦', 'gz': '📦', 'tar': '📦',
    'pdf': '📕', 'doc': '📘', 'xls': '📗'
  };
  return icons[ext] || '📄';
}

function formatSize(bytes) {
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';
  return (bytes / 1048576).toFixed(1) + ' MB';
}

function renderBreadcrumb() {
  var breadcrumb = document.getElementById('breadcrumb');
  breadcrumb.innerHTML = '';

  var parts = currentPath.split('/').filter(function(p) { return p; });
  var path = '/';

  var homeItem = document.createElement('span');
  homeItem.className = 'breadcrumb-item';
  homeItem.textContent = '🏠 Root';
  homeItem.onclick = function() { navigateTo('/'); };
  breadcrumb.appendChild(homeItem);

  parts.forEach(function(part) {
    var sep = document.createElement('span');
    sep.className = 'breadcrumb-separator';
    sep.textContent = '/';
    breadcrumb.appendChild(sep);

    path += part + '/';
    var item = document.createElement('span');
    item.className = 'breadcrumb-item';
    item.textContent = part;
    var p = path;
    item.onclick = function() { navigateTo(p); };
    breadcrumb.appendChild(item);
  });
}

function navigateTo(path) {
  currentPath = path;
  refreshFiles();
}

function showEditor() {
  document.getElementById('dropZone').style.display = 'none';
  document.getElementById('editorTextarea').style.display = 'block';
  document.getElementById('previewContent').style.display = 'none';
}

function showPreview() {
  document.getElementById('dropZone').style.display = 'none';
  document.getElementById('editorTextarea').style.display = 'none';
  document.getElementById('previewContent').style.display = 'flex';
}

function showDropZone() {
  document.getElementById('dropZone').style.display = 'flex';
  document.getElementById('editorTextarea').style.display = 'none';
  document.getElementById('previewContent').style.display = 'none';
}

async function openFileByName(path) {
  try {
    var filename = path.split('/').pop();
    currentFile = path;
    
    if (isPreviewable(filename)) {
      showPreview();
      document.getElementById('previewImage').src = api.download(path) + '&t=' + Date.now();
      
      document.getElementById('editorFilename').innerHTML = '<span>' + getFileIcon(filename) + '</span><span>' + filename + '</span>';
      document.getElementById('editorStatus').textContent = 'Preview';
      document.getElementById('editorStatus').style.color = 'var(--success)';
      document.getElementById('editorActions').style.display = 'flex';
      document.getElementById('btnSave').style.display = 'none';
    } else {
      var data = await api.read(path);
      if (data.error) {
        showToast(data.error, 'error');
        return;
      }
      
      showEditor();
      document.getElementById('editorTextarea').value = data.content || '';
      
      document.getElementById('editorFilename').innerHTML = '<span>' + getFileIcon(filename) + '</span><span>' + filename + '</span>';
      document.getElementById('editorStatus').textContent = 'Saved';
      document.getElementById('editorStatus').style.color = 'var(--success)';
      document.getElementById('editorActions').style.display = 'flex';
      document.getElementById('btnSave').style.display = 'flex';
    }
    
    refreshFiles();
  } catch (e) {
    showToast('Failed to open file: ' + e.message, 'error');
  }
}

async function saveCurrentFile() {
  if (!currentFile) return;
  
  var content = document.getElementById('editorTextarea').value;
  try {
    var result = await api.write(currentFile, content);
    if (result.success) {
      document.getElementById('editorStatus').textContent = 'Saved';
      document.getElementById('editorStatus').style.color = 'var(--success)';
      showToast('File saved', 'success');
    } else {
      showToast('Save failed', 'error');
    }
  } catch (e) {
    showToast('Save failed: ' + e.message, 'error');
  }
}

function downloadCurrentFile() {
  if (!currentFile) return;
  window.open(api.download(currentFile), '_blank');
}

function showContextMenu(x, y) {
  var menu = document.getElementById('contextMenu');
  menu.style.left = x + 'px';
  menu.style.top = y + 'px';
  menu.classList.add('active');
}

function openFile() {
  if (contextMenuTarget && contextMenuTarget.type === 'file') {
    openFileByName(contextMenuTarget.path);
  }
}

function renameFile() {
  if (!contextMenuTarget) return;
  showModal('Rename', contextMenuTarget.name, async function(newName) {
    if (newName && newName !== contextMenuTarget.name) {
      var oldPath = contextMenuTarget.path;
      var newPath = currentPath + newName;
      try {
        var result = await api.rename(oldPath, newPath);
        if (result.success) {
          if (currentFile === oldPath) currentFile = newPath;
          refreshFiles();
          showToast('Renamed successfully', 'success');
        } else {
          showToast('Rename failed', 'error');
        }
      } catch (e) {
        showToast('Rename failed', 'error');
      }
    }
  });
}

async function deleteFile() {
  if (!contextMenuTarget) return;
  if (confirm('Delete "' + contextMenuTarget.name + '"?')) {
    try {
      var result = await api.delete(contextMenuTarget.path);
      if (result.success) {
        if (currentFile === contextMenuTarget.path) {
          currentFile = null;
          showDropZone();
          document.getElementById('editorFilename').innerHTML = '<span>📄</span><span>No file selected</span>';
          document.getElementById('editorStatus').textContent = '';
          document.getElementById('editorActions').style.display = 'none';
        }
        refreshFiles();
        showToast('File deleted', 'success');
      } else {
        showToast('Delete failed', 'error');
      }
    } catch (e) {
      showToast('Delete failed', 'error');
    }
  }
}

function createNewFile() {
  showModal('New File', '', async function(name) {
    if (name) {
      var path = currentPath + name;
      try {
        var result = await api.write(path, '');
        if (result.success) {
          openFileByName(path);
          refreshFiles();
          showToast('File created', 'success');
        } else {
          showToast('Creation failed', 'error');
        }
      } catch (e) {
        showToast('Creation failed', 'error');
      }
    }
  });
}

function createNewFolder() {
  showModal('New Folder', '', async function(name) {
    if (name) {
      var path = currentPath + name;
      try {
        var result = await api.mkdir(path);
        if (result.success) {
          refreshFiles();
          showToast('Folder created', 'success');
        } else {
          showToast('Creation failed', 'error');
        }
      } catch (e) {
        showToast('Creation failed', 'error');
      }
    }
  });
}

function uploadFile() {
  document.getElementById('fileInput').click();
}

async function handleFileUpload(event) {
  var files = event.target.files;
  for (var i = 0; i < files.length; i++) {
    await uploadFileDirect(files[i]);
  }
  event.target.value = '';
}

async function uploadFileDirect(file) {
  var path = currentPath + file.name;
  
  try {
    showToast('Uploading ' + file.name + '...', 'warning');
    
    var result = await api.uploadMultipart(path, file);
    
    if (result.success) {
      refreshFiles();
      showToast(file.name + ' uploaded', 'success');
    } else {
      showToast('Upload failed', 'error');
    }
  } catch (e) {
    showToast('Upload failed: ' + e.message, 'error');
  }
}

async function loadStorageInfo() {
  try {
    var info = await api.info();
    var usedPercent = (info.used / info.total * 100).toFixed(1);
    document.getElementById('storageText').textContent = 
      formatSize(info.used) + ' / ' + formatSize(info.total) + ' (' + usedPercent + '%)';
    document.getElementById('storageBar').style.width = usedPercent + '%';
  } catch (e) {
    document.getElementById('storageText').textContent = 'Info unavailable';
  }
}

function showModal(title, defaultValue, callback) {
  document.getElementById('modalTitle').textContent = title;
  document.getElementById('modalInput').value = defaultValue;
  document.getElementById('modalOverlay').classList.add('active');
  document.getElementById('modalInput').focus();
  modalCallback = callback;
}

function closeModal() {
  document.getElementById('modalOverlay').classList.remove('active');
  modalCallback = null;
}

function confirmModal() {
  var value = document.getElementById('modalInput').value.trim();
  if (modalCallback && value) {
    modalCallback(value);
  }
  closeModal();
}

function showToast(message, type) {
  type = type || 'success';
  var toast = document.createElement('div');
  toast.className = 'toast ' + type;
  toast.textContent = message;
  document.body.appendChild(toast);
  setTimeout(function() { toast.remove(); }, 3000);
}

document.getElementById('modalInput').addEventListener('keypress', function(e) {
  if (e.key === 'Enter') confirmModal();
  if (e.key === 'Escape') closeModal();
});

document.addEventListener('keydown', function(e) {
  if ((e.ctrlKey || e.metaKey) && e.key === 's') {
    e.preventDefault();
    saveCurrentFile();
  }
});

init();
</script>
</body>
</html>
)rawliteral";

#endif
